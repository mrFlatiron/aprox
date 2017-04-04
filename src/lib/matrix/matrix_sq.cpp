#include "matrix_sq.h"
#include <cstdio>
#include <cmath>

matrix_sq::matrix_sq() : matrix_simple ()
{

}

matrix_sq::matrix_sq (const unsigned int n) : matrix_simple (n, n),
  m_n (n)
{

}

matrix_sq::matrix_sq (const unsigned int n, const std::vector<double> &vals) : matrix_simple (n, n, vals),
  m_n (n)
{

}

matrix_sq::~matrix_sq()
{

}

int matrix_sq::gauss_solve_corrupt_wo_pivot (std::vector<double> &rhs)
{
  if (m_n != rhs.size ())
    {
      fprintf (stderr, "sizes do not match, rhs.size = %lu, m_n = %u\n", rhs.size (), m_n);
      return -1;
    }

  for (unsigned int i_main = 0; i_main < m_n; i_main++)
    {
      double pivot = m_vals[i_main * m_n + i_main];
      if (fabs (pivot) < EPS)
        {
          fprintf (stderr, "cannot solve\n");
          return -1;
        }
      for (unsigned int j = i_main + 1; j < m_n; j++)
        m_vals[i_main * m_n + j] /= pivot;
      rhs[i_main] /= pivot;
      for (unsigned int i = i_main + 1; i < m_n; i++)
        {
          double coef = m_vals[i * m_n + i_main];
          for (unsigned int j = i_main + 1; j < m_n; j++)
            m_vals[i * m_n + j] -= m_vals[i_main * m_n + j] * coef;
          rhs[i] -= rhs[i_main] * coef;
        }
    }

  for (unsigned int i_main = m_n - 1; i_main > 0; i_main--)
    {
      for (int i = i_main - 1; i >= 0; i--)
        {
          rhs[i] -= rhs[i_main] * m_vals[i * m_n + i_main];
        }
    }
  return 0;
}
