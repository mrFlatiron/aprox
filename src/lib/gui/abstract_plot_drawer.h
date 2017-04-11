#ifndef ABSTRACT_PLOT_DRAWER_H
#define ABSTRACT_PLOT_DRAWER_H
#include <QWidget>
#include <QPaintEvent>

class abstract_plot_model;
class QPaintEvent;
class graph_painter;

class abstract_plot_drawer : public QWidget
{
  Q_OBJECT

private:
 graph_painter *m_painter = nullptr;
 double m_x_scale = 1;
 double m_y_scale = 1;
 double m_x_shift;
 double m_y_shift;
 int m_smooth;
 int m_pivot_count;
 double m_x_min;
 double m_x_max;
 double m_y_max;
 double m_y_min;
 const double m_axis_max = 1000;
 abstract_plot_model *m_plot_model = nullptr;

public:
  abstract_plot_drawer (QWidget *parent);
  virtual ~abstract_plot_drawer ();
  abstract_plot_model *get_model ();
  void set_model (abstract_plot_model *model);
protected:
  virtual void paintEvent (QPaintEvent *event) override;
private:
  void calculate_pivot_points ();
  void calculate_axis_length (double &l_axis, double &r_axis, double &u_axis, double &b_axis);
  void set_default ();
  QPen set_pen (const int graph_num) const;
  void draw_grid ();
  void draw_graph (const int graph_num);
  void redraw_all ();
  QPointF get_by_x (const int graph_num, const double x) const;
  void calclulate_scale ();
private slots:
  void on_model_changed ();

};


#endif // ABSTRACT_PLOT_DRAWER_H
