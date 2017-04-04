#ifndef INTERPOL_FACTORY_H
#define INTERPOL_FACTORY_H

#include <cstdio>
#include <memory>
#include <vector>
#include <functional>
#include "interpol_polynom.h"
#include "interpol_tools.h"

namespace interpol
{
  enum class polynom_type
  {
    newton_mult_nodes,
    c_spline_w_derivs,
    COUNT
  };

  template <class T>
  T *create_polynom (FILE *fin, const unsigned int x_size)
  {
    if (x_size < 2)
      {
        fprintf (stderr, "factory error: points count must be > 1\n");
        return nullptr;
      }

    std::vector<double> xes;
    std::vector<double> ys;
    std::vector<double> additional;

    auto error = read_to_vector (fin, xes, x_size);
    error += read_to_vector (fin, ys, x_size);
    if (error)
      return nullptr;

    unsigned int add_size;
    additional_array_size type =   T::get_add_type ();
                                  //additional_array_size::x_size;

    switch (type)
      {
      case additional_array_size::x_size:
        add_size = x_size;
        break;
      case additional_array_size::const_size:
        add_size = T::get_add_size ();
        break;
      default:
        fprintf (stderr, "SHOULDN'T HAPPEN");
        return nullptr;
      }

    read_to_vector (fin, additional, add_size);

    return new T *(xes, ys, additional);
  }

  template<class T>
  T *create_polynom (const char *filename, const unsigned int x_size)
  {
    FILE *fin = fopen (filename, "r");

    if (!fin)
      {
        fprintf (stderr, "ERROR opening the file %s\n", filename);
        return nullptr;
      }

    T *to_return =  create_polynom<T> (fin, x_size);
    fclose (fin);
    return to_return;
  }

  template<class T>
  T *create_polynom (const double a, const double b,
                                     const unsigned int points_count,
                                     std::function<double(const double)> f,
                                     const std::vector<double> &additional)
  {
    if (points_count < 2)
      {
        fprintf (stderr, "factory error: points count must be > 1\n");
        return nullptr;
      }
    return  new T (a, b, points_count, f, additional);
  }

}
#endif // INTERPOL_FACTORY_H
