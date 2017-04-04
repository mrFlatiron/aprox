#ifndef MATRIX_SIMPLE_H
#define MATRIX_SIMPLE_H
#include <vector>

class matrix_simple
{
protected:
  unsigned int m_rows;
  unsigned int m_cols;
  std::vector<double> m_vals;
public:
  matrix_simple ();
  matrix_simple (const unsigned int rows, const unsigned int cols);
  matrix_simple (const unsigned int rows, const unsigned int cols, const std::vector<double> &vals);
  double operator[] (const unsigned int index) const;
  double &operator [] (const unsigned int index);
  virtual ~matrix_simple ();
};

#endif // MATRIX_SIMPLE_H
