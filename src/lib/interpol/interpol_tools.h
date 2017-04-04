#ifndef INTERPOL_TOOLS_H
#define INTERPOL_TOOLS_H
#include <cstdio>
#include <vector>

namespace interpol
{
  int read_to_vector (FILE *fin, std::vector<double> &vec, const unsigned int size);

}

#endif // INTERPOL_TOOLS_H
