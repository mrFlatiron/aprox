#ifndef INTERPOL_NEWTON_MULT_NODES_H
#define INTERPOL_NEWTON_MULT_NODES_H

#include <vector>
#include "interpol_polynom.h"

namespace interpol
{
  const double EPS = 1e-16;
  class newton_mult_nodes : public polynom
  {
  private:
    unsigned int m_points_count;
    std::vector<double> m_xes;
    std::vector<double> m_div_difs; // m_x_size * 2 size
    double m_x_max;
    double m_x_min;
  public:
    newton_mult_nodes ();
    virtual ~newton_mult_nodes ();
//    newton_mult_nodes (const double *three_in_one, const unsigned int size_of_one); //three_in_one = xes+ys+derivs;
    newton_mult_nodes (const std::vector<double> &xes, const std::vector<double> &ys,
                      const std::vector<double> &derivs);
    newton_mult_nodes (const double a_, const double b_,
                       const unsigned int points_count,
                       std::function<double(const double)> f,
                       std::function<double(const double)> d);
    virtual void interpolate_function (const double a_, const double b_,
                              const unsigned int points_count,
                              std::function<double(const double)> f,
                              const std::vector<double> &additional) override;
    virtual void interpolate_function (const double a_, const double b_,
                                       const unsigned int points_count,
                                       std::function<double(const double)> f,
                                       std::function<double(const double)> d);
    virtual void interpolate_points (const std::vector<double> &xes,
                                     const std::vector<double> &ys,
                                     const std::vector<double> &derivs) override;
    double operator () (const double x) const override;
    virtual additional_array_size get_add_type () const override;
    bool is_in_range (const double x) const;
    virtual int get_points_count () const override;
  private:
    void find_bounds ();
    void compute_div_difs ();
    double horner_sum (const double x) const;
  };
}

#endif // INTERPOL_NEWTON_MULT_NODES_H
