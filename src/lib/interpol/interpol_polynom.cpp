#include "interpol_polynom.h"

using namespace interpol;

polynom::polynom ()
{}

double polynom::eval(const double x) const
{
  return operator ()(x);
}

unsigned int polynom::get_add_size ()
{
  return 0;
}

additional_array_size polynom::get_add_type ()
{
  return additional_array_size::x_size;
}

polynom::~polynom () {}
