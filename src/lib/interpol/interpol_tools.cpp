#include "interpol_tools.h"

int interpol::read_to_vector (FILE *fin, std::vector<double> &vec, const unsigned int size)
{
  for (unsigned int j = 0; j < size; j++)
    {
      double val;
      if (fscanf (fin, "%lf", &val) == 1)
        vec.push_back (val);
      else
        {
          if (ferror (fin))
            fprintf (stderr, "ERROR reading file:expected double\n");
          else
            fprintf (stderr, "ERROR not enough data in file \n");
          return -1;
        }
    }
  return 0;
}
