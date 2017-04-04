﻿#include "interpol_newton_mult_nodes.h"
#include <cmath>
#include <cstdio>
using namespace interpol;

newton_mult_nodes::newton_mult_nodes ()
{

}

newton_mult_nodes::~newton_mult_nodes () {}

newton_mult_nodes::newton_mult_nodes (const double *three_in_one,
                                     const unsigned int size_of_one)
{
  m_points_count = size_of_one;
  const double *it = three_in_one;
  for (unsigned int i = 0; i < m_points_count; i++, it++)
    {
      m_xes.push_back (it[0]);
      m_div_difs.push_back (it[m_points_count]);
      m_div_difs.push_back (it[2 * m_points_count]);
    }
  find_bounds ();
  compute_div_difs ();
}

newton_mult_nodes::newton_mult_nodes (const std::vector<double> &xes, const std::vector<double> &ys,
                                      const std::vector<double> &derivs)
{
  m_xes = xes;
  m_points_count = m_xes.size ();
  for (unsigned int i = 0; i < m_points_count; i++)
    {
      m_div_difs.push_back (ys[i]);
      m_div_difs.push_back (derivs[i]);
    }
  find_bounds ();
  compute_div_difs ();
}

double newton_mult_nodes::operator () (const double x) const
{
  return horner_sum (x);
}

additional_array_size newton_mult_nodes::get_add_type ()
{
  return additional_array_size::x_size;
}

bool newton_mult_nodes::is_in_range(const double x) const
{
  return (x >= m_x_min && x <= m_x_max) ? true : false;
}

int newton_mult_nodes::get_points_count() const
{
  return (int)m_points_count;
}

void newton_mult_nodes::find_bounds ()
{
  if (m_points_count == 0)
    return;
  m_x_min = m_xes[0];
  m_x_max = m_xes[0];
  for (unsigned int i = 1; i < m_points_count; i++)
    {
      if (m_xes[i] > m_x_max)
        m_x_max = m_xes[i];
      if (m_xes[i] < m_x_min)
        m_x_min = m_xes[i];
    }
}

void newton_mult_nodes::compute_div_difs ()
{
  double f = m_div_difs[0];
  unsigned int x_pos = 0;
  for (unsigned int store_pos = 2; store_pos < 2 * m_points_count; store_pos += 2, x_pos++)
    {
      if (fabs (m_xes[x_pos] - m_xes[x_pos + 1]) < EPS)
        {
          fprintf (stderr, "equal dots error\n");
          return;
        }
      double s = m_div_difs[store_pos];
      m_div_difs[store_pos] = (s - f) / (m_xes[x_pos + 1] - m_xes[x_pos]);
      f = s;
    }

  for (unsigned int k = 3; k <= 2 * m_points_count; k++)
    {
      f = m_div_difs[k - 2];
      for (unsigned int store_pos = k - 1; store_pos < 2 * m_points_count; store_pos++)
        {
          double x2 = m_xes[store_pos / 2];
          double x1 = m_xes[store_pos / 2 - (k - 1) / 2];
          double s = m_div_difs[store_pos];
          m_div_difs[store_pos] = (s - f) / (x2 - x1);
          f = s;
        }
    }
}

double newton_mult_nodes::horner_sum (const double x) const
{

  double ret = 0;

  for (auto pos = 2 * m_points_count - 1; pos >= 1; pos--)
    {
      ret = (ret + m_div_difs[pos]) * (x - m_xes[(pos - 1)/ 2]);
    }

  ret += m_div_difs[0];

  return ret;
}


