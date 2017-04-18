#ifndef INTERPOL_TOOLS_H
#define INTERPOL_TOOLS_H
#include <cstdio>
#include <vector>

class QString;

namespace interpol
{
  enum class polynom_type
  {
    newton_mult_nodes,
    c_spline_w_derivs,
    COUNT
  };

  int read_to_vector (FILE *fin, std::vector<double> &vec, const unsigned int size);
  std::vector<QString> get_interpol_labels ();
  QString type_to_string (const polynom_type type);
  QString type_to_str (const polynom_type type);
}

#endif // INTERPOL_TOOLS_H
