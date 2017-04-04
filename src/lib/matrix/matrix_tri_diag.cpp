#include "matrix_tri_diag.h"
#include <cmath>
#include <cstdio>

matrix_tri_diag::matrix_tri_diag () :
  m_n (0),
  m_vals ({})
{

}

matrix_tri_diag::matrix_tri_diag (const unsigned int n) :
  m_n (n),
  m_vals (4 + 3 * (n - 2), 0)
{

}

matrix_tri_diag::~matrix_tri_diag ()
{

}

void matrix_tri_diag::set_row (const unsigned int i, const double d1, const double d2, const double d3)
{
  if (i == 0)
    {
      m_vals[0] = d2;
      m_vals[1] = d3;
      return;
    }
  if (i == m_n - 1)
    {
      m_vals[2 + (i - 1) * 3] = d1;
      m_vals[3 + (i - 1) * 3] = d2;
      return;
    }
  auto index = 2 + (i - 1) * 3;
  m_vals[index] = d1;
  index++;
  m_vals[index] = d2;
  index++;
  m_vals[index] = d3;
}

void matrix_tri_diag::gauss_wo_pivot (std::vector<double> &rhs)
{
  if (m_n == 0)
    return;
  if (m_n == 1)
    {
      if (fabs (m_vals[0]) < m_EPS)
        fprintf (stderr, "NO SOLUTION\n");
      else
        rhs[0] /= m_vals[0];
    return;
    }
  unsigned int i_main = 0;
  double coef = m_vals[i_main];
  if (fabs(coef) < m_EPS)
    {
      fprintf (stderr, "NO SOLUTION\n");
      return;
    }
  else
    {
      m_vals[i_main + 1] /= coef;
      rhs[i_main] /= coef;
    }
  coef = m_vals[i_main + 2];
  m_vals[i_main + 3] -= m_vals[i_main + 1] * coef;
  rhs[i_main + 1] -= rhs[i_main] * coef;
  unsigned int index = 3;
  for (i_main = 1; i_main < m_n - 1; i_main++, index += 3)
    {
      coef = m_vals[index];
      if (fabs(coef) < m_EPS)
        {
          fprintf (stderr, "NO SOLUTION\n");
          return;
        }
      else
        {
          m_vals[index + 1] /= coef;
          rhs[i_main] /= coef;
        }
      coef = m_vals[index + 2];
      m_vals[index + 3] -= m_vals[index + 1] * coef;
      rhs[i_main + 1] -= rhs[i_main] * coef;
    }
  coef = m_vals[index];
  if (fabs(coef) < m_EPS)
    {
      fprintf (stderr, "NO SOLUTION\n");
      return;
    }
  else
    rhs[i_main] /= coef;
  index -= 2;
  for (i_main = m_n - 1; i_main >= 1; i_main--)
    {
      rhs[i_main - 1] -= rhs[i_main] * m_vals[index];
      index -= 3;
    }
}
