#include "interpol_c_splines_w_derivs.h"
#include "interpol_newton_mult_nodes.h"
#include "interpol_factory.h"
#include "matrix/matrix_sq.h"
#include "matrix/matrix_tri_diag.h"
#include <algorithm>
#include <cstdio>

using namespace interpol;

c_splines_w_derivs::c_splines_w_derivs ()
{

}

c_splines_w_derivs::~c_splines_w_derivs ()
{
}

c_splines_w_derivs::c_splines_w_derivs (const std::vector<double> &xes_,
                                       const std::vector<double> &ys,
                                       const std::vector<double> &derivs_) :
  m_coefs ()
{
  interpolate_points (xes_, ys, derivs_);
}

c_splines_w_derivs::c_splines_w_derivs (const double a_, const double b_, const unsigned int size,
                                        std::function<double(const double)> f,
                                        const std::vector<double> &additional) :
  m_coefs ()
{
  interpolate_function (a_, b_, size, f, additional);
}

void c_splines_w_derivs::compute_derivs (const std::vector<double> &xes, const std::vector<double> &ys, std::vector<double> &derivs,
                                         const double d1, const double dn)
{
  matrix_tri_diag matrix (m_points_count);
  std::vector<double> rhs (m_points_count);
  rhs[0] = d1;
  rhs[m_points_count - 1] = dn;
  matrix.set_row (0, 0, 1, 0);
  matrix.set_row (m_points_count - 1, 0, 1, 0);
  double x_dif = xes[1] - xes[0];
  double div_dif = (ys[1] - ys[0]) / x_dif;
  double buf;
  for (unsigned int i = 1; i < m_points_count - 1; i++)
    {
      buf = xes[i + 1] - xes[i];

      if (fabs (buf) < 1e-16)
      {
          printf ("zero division\n");
          std::abort ();
      }
      matrix.set_row (i, buf, 2 * (xes[i + 1] - xes[i - 1]), x_dif);

      rhs[i] = 3 * div_dif * (buf);
      div_dif = (ys[i + 1] - ys[i]) / buf;
      rhs[i] += 3 * div_dif * x_dif;

      x_dif = buf;
    }

  matrix.gauss_wo_pivot (rhs);
  derivs = std::move (rhs);
}

void c_splines_w_derivs::compute_loc_polynoms (const std::vector<double> &xes, const std::vector<double> &ys,
                                               const std::vector<double> &derivs)
{
  m_coefs.clear ();
  m_coefs.resize (4 * m_points_count - 4);
  m_xes = xes;
  for (unsigned int i = 0; i < m_points_count - 1; i++)
    {
      int coef_iter = i * 4;
      double c1, c2, c3, c4;
      double d1 = derivs[i];
      double d2 = derivs[i + 1];
      double y1 = ys[i];
      double y2 = ys[i + 1];
      double x1 = m_xes[i];
      double x2 = m_xes[i + 1];
      double x_dif = x2 - x1;
      double div_dif = (y2 - y1) / x_dif;
      c1 = y1;
      c2 = d1;
      c3 = (3 * div_dif - 2 * d1 - d2) / x_dif;
      c4 = (d1 + d2 - 2 * div_dif) / x_dif / x_dif;
      m_coefs[coef_iter] = c1;
      m_coefs[coef_iter + 1] = c2;
      m_coefs[coef_iter + 2] = c3;
      m_coefs[coef_iter + 3] = c4;
    }
}

double c_splines_w_derivs::operator () (const double x) const
{
  int i = -1, coef_i;
  if (x >= m_x_max)
    i = m_points_count - 2;
  if (x <= m_x_min)
    i = 0;
  if (i == -1)
    {
      i = bin_search (m_xes.data (), 0, m_points_count - 1, x);
      i--;
    }
//  i = 0;
  coef_i = i * 4;
  double dif = x - m_xes[i];
  return m_coefs[coef_i] + dif * m_coefs[coef_i + 1] +
      m_coefs[coef_i + 2] * dif * dif + m_coefs[coef_i + 3] *
      dif * dif * dif;
}

int c_splines_w_derivs::get_points_count () const
{
  return (int)m_points_count;
}

void c_splines_w_derivs::interpolate_function (const double a_, const double b_,
                                               const unsigned int points_count,
                                               std::function<double (const double)> f,
                                               const std::vector<double> &additional)
{
  double a = a_;
  double b = b_;
  if (a_ > b_)
    {
      a = b_;
      b = a_;
    }
  m_points_count = points_count;
  m_x_min = a;
  m_x_max = b;

  if (points_count < 2)
    {
      fprintf (stderr, "wrong size in constructor\n");
      return;
    }

  double hx = (b - a) / (points_count - 1);
  std::vector<double> xes;
  std::vector<double> ys;
  for (unsigned int i = 0; i < points_count; i++)
    {
      xes.push_back (a + i * hx);
      ys.push_back (f(xes[i]));
    }

  std::vector<double> derivs;
  if (additional.size () < 2)
    {
      fprintf (stderr, "wrong additional size in constructor\n");
      return;
    }
  double d1 = additional[0];
  double dn = additional[1];

  printf ("begin deriv\n");
  compute_derivs (xes, ys, derivs, d1, dn);
  printf ("end deriv\n");
  compute_loc_polynoms (xes, ys, derivs);

}

void c_splines_w_derivs::interpolate_points (const std::vector<double> &xes_,
                                             const std::vector<double> &ys,
                                             const std::vector<double> &derivs_)
{

  std::vector<double> xes = xes_;
  std::sort (xes.begin (), xes.end ());
  m_points_count = xes.size ();

  if (m_points_count < 2)
    {
      fprintf (stderr, "wrong size in constructor\n");
      return;
    }

  m_x_min = xes[0];
  m_x_max = xes[xes.size () - 1];
  if (m_points_count > ys.size () || derivs_.size () < 2)
    {
      fprintf (stderr, "constructor error\n");
      return;
    }
  double d1 = derivs_[0];
  double dn = derivs_[1];
  std::vector<double> derivs (m_points_count);

  compute_derivs (xes, ys, derivs, d1, dn);

  compute_loc_polynoms (xes, ys, derivs);
}

void c_splines_w_derivs::interpolate_function (const double a_, const double b_, const unsigned int points_count, std::function<double (const double)> f, std::function<double (const double)> d)
{
   polynom::interpolate_function (a_, b_, points_count, f, d);
}

unsigned int c_splines_w_derivs::get_add_size () const
{
  return 2;
}

additional_array_size c_splines_w_derivs::get_add_type () const
{
  return additional_array_size::const_size;
}


