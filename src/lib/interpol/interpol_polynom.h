#ifndef INTERPOL_POLYNOM_H
#define INTERPOL_POLYNOM_H
#include <vector>
#include <functional>
namespace interpol
{
  enum class additional_array_size
  {
    x_size,
    const_size
  };

  class polynom
  {
  public:
    polynom ();
    virtual ~polynom ();
    virtual double operator () (const double x) const = 0;
    virtual int get_points_count () const = 0;
    virtual void interpolate_function (const double a, const double b,
                              const unsigned int points_count,
                              std::function<double(const double)> f,
                              const std::vector<double> &additional) {}
    virtual void interpolate_points (const std::vector<double> &xes, const std::vector<double> &ys,
                                     const std::vector<double> &derivs_) {}
    double eval (const double x) const; //operator () alias;
    static unsigned int get_add_size ();
    static additional_array_size get_add_type ();
  };
}

#endif // INTERPOL_POLYNOM_H
