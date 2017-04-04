#ifndef MATRIX_TRI_DIAG_H
#define MATRIX_TRI_DIAG_H


#include <vector>

class matrix_tri_diag
{
private:
  unsigned int m_n;
  std::vector<double> m_vals;
  const double m_EPS = 1e-16;
public:
  matrix_tri_diag ();
  matrix_tri_diag (const unsigned int n);
  ~matrix_tri_diag ();
  void set_row (const unsigned int i, const double d1, const double d2, const double d3);
  void gauss_wo_pivot (std::vector<double> &rhs);
};

#endif // MATRIX_TRI_DIAG_H
