#include "interpol_polynom.h"

using namespace interpol;

polynom::polynom ()
{}

double polynom::eval (const double x) const
{
  return operator ()(x);
}

unsigned int polynom::get_add_size () const
{
  return 0;
}

additional_array_size polynom::get_add_type () const
{
  return additional_array_size::x_size;
}

polynom::~polynom () {}

void polynom::interpolate_function (const double a, const double b,
                                   const unsigned int points_count,
                                   std::function<double (const double)> f,
                                   const std::vector<double> &additional)
{
  (void)a;
  (void)b;
  (void)points_count;
  (void)f;
  (void)additional;
}

void polynom::interpolate_function (const double a_, const double b_,
                                   const unsigned int points_count,
                                   std::function<double (const double)> f,
                                   std::function<double (const double)> d)
{
  (void)a_;
  (void)b_;
  (void)points_count;
  (void)f;
  (void)d;
}
