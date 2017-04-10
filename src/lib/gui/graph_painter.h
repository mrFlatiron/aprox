#ifndef GRAPH_PAINTER_H
#define GRAPH_PAINTER_H

#include <QPainter>
#include "abstract_plot_model.h"

class abstract_plot_model;

class graph_painter : public QPainter
{
private:
  double m_x_scale;
  double m_y_scale;
  double m_smooth = 10;
  double m_y_min;
  double m_y_max;
  double m_x_min;
  double m_x_max;
  int m_pivot_count;
  abstract_plot_model *m_plot_model = nullptr;
public:
  graph_painter ();
  graph_painter (QPaintDevice *);
  void draw_axis ();
  void draw_graph (const int graph_num);
  void redraw_all ();
  void set_model (abstract_plot_model *model);
  virtual ~graph_painter ();
private:
  void calculate_pivot_count ();
//  void calculate_axis_length (double &l_axis, double &r_axis, double &u_axis, double &b_axis);
  void calculate_window_rect ();
  void calculate_graph_vert_bounds (const int graph_num, double &y_max, double &loc_max);
  QPen set_pen (const int graph_num) const;
private slots:
  void on_model_changed ();
};

#endif // GRAPH_PAINTER_H
