#ifndef ABSTRACT_PLOT_DRAWER_H
#define ABSTRACT_PLOT_DRAWER_H
#include <QGraphicsScene>

class abstract_plot_model;
class QGraphicsView;
class graphics_view;

class abstract_plot_drawer : public QGraphicsScene
{
  Q_OBJECT

private:
 graphics_view *m_view = nullptr;
 int m_scale;
 int m_smooth;
 int m_pivot_count;
 double m_x_min;
 double m_x_max;
 abstract_plot_model *m_plot_model = nullptr;

public:
  abstract_plot_drawer (QObject *parent);
  ~abstract_plot_drawer ();
  abstract_plot_model *get_model ();
  void set_model (abstract_plot_model *model);
  QGraphicsView *get_view();
  void show () const;
  void set_scale (const int scale);
  int scale () const;
public slots:
  void set_centered ();
private:
  void calculate_pivot_points ();
  void set_default ();
  QPen set_pen (const int graph_num) const;
  void draw_grid ();
  void draw_graph (const int graph_num);
  void redraw_all ();
  QPointF get_by_x (const int graph_num, const double x) const;
private slots:
  void on_model_changed ();

};


#endif // ABSTRACT_PLOT_DRAWER_H
