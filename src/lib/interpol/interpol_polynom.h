#ifndef INTERPOL_POLYNOM_H
#define INTERPOL_POLYNOM_H
#include <vector>
#include <functional>

/*******************
 * To implement your interpolation:
 * * Create a class derived from interpol::polynom
 * * Add its name to enum class interpol::polynom_type in interpol_tools.h (right before COUNT)
 * * Implement pure virtual methods
 * * Implement interpolate_points ()
 * * Implement constructor with the same prototype as interpolate_points ()
 * * Implement constructor () (leave empty for example)
 *
 * * If get_add_type = x_size
 * * * Implement the second interpolate_function (2 functions in arguments)
 * * If = const_size
 * * * Implement get_add_size, which returns expected size of additional
 * * * Implement the first interpolate_function (1 function and 1 vector in args)
 *
 * * Implement constructor with the same prototype as interpolate_function (...) you use
 *
 * * Adjust interpol_plot_model:
 * * * Add 'case your_interpol:' to switches where needed.
 *
 * To create an instance of your class use a T *create_polynom<T> method from interpol_factory
*******************/


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
                              const std::vector<double> &additional);
    virtual void interpolate_function (const double a_, const double b_,
                                       const unsigned int points_count,
                                       std::function<double(const double)> f,
                                       std::function<double(const double)> d);
    virtual void interpolate_points (const std::vector<double> &xes, const std::vector<double> &ys,
                                     const std::vector<double> &additional) = 0;
    double eval (const double x) const; //operator () alias;
    virtual unsigned int get_add_size () const;
    virtual additional_array_size get_add_type () const = 0;
  };
}

#endif // INTERPOL_POLYNOM_H
