#ifndef INTERPOL_PLOT_MODEL_H
#define INTERPOL_PLOT_MODEL_H
#include "abstract_plot_model.h"
#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace interpol
{
  enum class polynom_type;
  class polynom;
}
class interpol_plot_model : public abstract_plot_model
{
  Q_OBJECT
private:
  const double EPS = 1e-15;
  double m_x_min;
  double m_x_max;
  int m_points_count;
  int m_shown_count;
  std::function<double(const double)> m_origin;
  std::vector<std::unique_ptr<interpol::polynom>> m_interpols;
  std::vector<std::vector<double>> m_additional_vectors;
  std::vector<std::function<double(const double)>> m_additional_funcs;
  std::vector<bool> m_interpol_shown;
  bool m_origin_shown = true;
public:
  interpol_plot_model () = delete;
  interpol_plot_model (const double x_min, const double x_max,
                       const int points_count);
  virtual ~interpol_plot_model ();
  void set_meta (const double x_min,
                 const double x_max,
                 const int points_count);
  void add_interpol (const interpol::polynom_type type,
                     const std::vector<double> &additional);
  void add_interpol (const interpol::polynom_type type,
                     std::function<double(const double)> d);
  void set_origin_func (const std::function <double(const double)> &origin);
  int get_points_count () const;
  void set_points_count (const int points_count);
  void reinterpolate ();
  virtual int graphs_count () const override;
  virtual QPointF point_by_x (const int graph_num, const double x) const override;
  virtual QVariant paint_config (const int graph_num, const graph_role role) const override;
  virtual void bounds(const int graph_num, double &left, double &right) const override;

  QColor get_color(const interpol::polynom_type type) const;

public slots:
  void change_visible_graphs (int id, bool shown);
};

#endif // INTERPOL_PLOT_MODEL_H
