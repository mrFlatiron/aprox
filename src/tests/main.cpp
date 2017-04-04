#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "interpol/interpol_newton_mult_nodes.h"
#include "interpol/interpol_c_splines_w_derivs.h"
#include "interpol/interpol_factory.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 4 && argc != 7)
    {
      printf ("Usage: %s <x> <points count> <file> or %s <x> <points_count> <a> <b> <d1> <dn>\n",
              argv[0], argv[0]);
      return 0;
    }
  double x = atof (argv[1]);

  unsigned int size = atoi (argv[2]);
  if (!size)
    {
      fprintf (stderr, "size must be unsigned int > 0\n");
      return 0;
    }
  cswd_ptr spline;
  if (argc == 4)
    spline =
      interpol::create_polynom<interpol::c_splines_w_derivs> (argv[3], size);

  else
    {
      double a = atof (argv[3]);
      double b = atof (argv[4]);
      double d1 = atof (argv[5]);
      double dn = atof (argv[6]);
      spline = interpol::create_polynom<interpol::c_splines_w_derivs>
               (a, b, size, [] (const double x) {return x * x * x;}, {d1, dn});
    }
  if (spline == nullptr)
    {
      fprintf (stderr, "spline was not created\n");
      return 0;
    }
  printf ("f(%f) = %f\n", x, spline->eval (x));
  return 0;
}
