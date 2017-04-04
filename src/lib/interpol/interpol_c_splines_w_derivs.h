#ifndef INTERPOL_C_SPLINES_W_DERIVS_H
#define INTERPOL_C_SPLINES_W_DERIVS_H
#include "interpol_polynom.h"
#include <vector>
#include <memory>
#include <functional>
#include "interpol_newton_mult_nodes.h"


namespace interpol
{
  class c_splines_w_derivs : public polynom
  {
  private:
    unsigned int m_points_count;
    double m_x_min;
    double m_x_max;
    std::vector<std::unique_ptr<newton_mult_nodes>> m_loc_polynoms;
  public:
    c_splines_w_derivs ();
    ~c_splines_w_derivs ();
    c_splines_w_derivs (const std::vector<double> &xes, const std::vector<double> &ys,
                        const std::vector<double> &derivs_);
    c_splines_w_derivs (const double a, const double b, const unsigned int size,
                        std::function<double(const double)> f, const std::vector<double> &additional);
    double operator () (const double x) const override;
    virtual int get_points_count () const override;
    virtual void interpolate_function (const double a_, const double b_,
                              const unsigned int points_count,
                              std::function<double(const double)> f,
                              const std::vector<double> &additional) override;
    virtual void interpolate_points (const std::vector<double> &xes_,
                                     const std::vector<double> &ys,
                                     const std::vector<double> &derivs_) override;
    virtual unsigned int get_add_size () const override;
    virtual additional_array_size get_add_type () const override;
  private:
    void compute_derivs (const std::vector<double> &xes, const std::vector<double> &ys,
                         std::vector<double> &derivs,
                         const double d1,
                         const double dn);
    void compute_loc_polynoms (const std::vector<double> &xes, const std::vector<double> &ys,
                               const std::vector<double> &derivs);

  };
}

typedef std::unique_ptr<interpol::c_splines_w_derivs> cswd_ptr;
#endif // INTERPOL_C_SPLINES_W_DERIVS_H
