#include "interpol_tools.h"
#include "interpol_factory.h"
#include <QString>

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

std::vector<QString> interpol::get_interpol_labels ()
{
  std::vector<QString> labels;
  for (int i = 0; (interpol::polynom_type)i != interpol::polynom_type::COUNT; i++)
    {
      labels.push_back (type_to_string ((interpol::polynom_type)i));
    }
  return labels;
}

QString interpol::type_to_string (const interpol::polynom_type type)
{
  switch (type)
    {
    case interpol::polynom_type::newton_mult_nodes:
      return "Newton with multiple nodes";
    case interpol::polynom_type::c_spline_w_derivs:
      return "Cubic splines with bounds' deriv-s";
    case interpol::polynom_type::COUNT:
      std::abort ();

    }
  return "Add description to get_interpol_labels ()";
}
