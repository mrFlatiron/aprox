#include "test_functions.h"
#include <cmath>

double func_to_aprox (const double x)
{
  return cubic (x);
}

double id (const double x)
{
  return 1;
}

double cubic (const double x)
{
  return x * x * x - 5 * x + 6;
}

double cubic_d (const double x)
{
  return 3 * x * x - 5;
}
