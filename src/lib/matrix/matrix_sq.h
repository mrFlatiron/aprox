#ifndef MATRIX_SQ_H
#define MATRIX_SQ_H
#include "matrix_simple.h"
const double EPS = 1e-16;

class matrix_sq : public matrix_simple
{
protected:
  unsigned int m_n; // = m_rows = m_cols
public:
  matrix_sq ();
  matrix_sq (const unsigned int n);
  matrix_sq (const unsigned int n, const std::vector<double> &vals);
  int gauss_solve_corrupt_wo_pivot (std::vector<double> &rhs); //corrupts itself and rhs. rhs will contain answer
  ~matrix_sq ();
};

#endif // MATRIX_SQ_H
