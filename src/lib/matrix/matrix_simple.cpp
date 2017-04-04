#include "matrix_simple.h"

matrix_simple::matrix_simple () :
  m_rows (0),
  m_cols (0)
{

}

matrix_simple::matrix_simple (const unsigned int rows, const unsigned int cols) :
  m_rows (rows),
  m_cols (cols),
  m_vals (rows * cols, 0)
{

}

matrix_simple::matrix_simple(const unsigned int rows, const unsigned int cols, const std::vector<double> &vals) :
  m_rows (rows),
  m_cols (cols),
  m_vals (vals)
{

}

matrix_simple::~matrix_simple () {}

double matrix_simple::operator[] (const unsigned int index) const
{
  return m_vals[index];
}

double &matrix_simple::operator [](const unsigned int index)
{
  return m_vals[index];
}
