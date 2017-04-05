#include "abstract_plot_drawer.h"
#include "abstract_plot_model.h"
#include "graphics_view.h"
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>

abstract_plot_drawer::abstract_plot_drawer (QObject *parent) : QGraphicsScene (parent)
{
  set_default ();
}

abstract_plot_drawer::~abstract_plot_drawer ()
{
 if (m_view)
   delete m_view;
}

void abstract_plot_drawer::set_model (abstract_plot_model *model)
{
  m_plot_model = model;
  connect (m_plot_model, SIGNAL (model_changed ()), this, SLOT (on_model_changed ()));
  m_view->setScene (this);
//  double l = -200;
//  double t = 200;
//  double r = 200;
//  double b = -200;
//  QRectF f(l, -t, r - l, t - b);
//  setSceneRect (f);

  redraw_all ();
//      addLine (l, t, r, b);
//       m_view->centerOn (0, 0);
//      m_view->ensureVisible (f);
//  f = sceneRect ();
}

QGraphicsView *abstract_plot_drawer::get_view ()
{
  return m_view;
}

void abstract_plot_drawer::show () const
{
  m_view->show ();
}

void abstract_plot_drawer::set_scale (const int scale)
{
  if (scale == 0)
    return;
  m_view->set_scale (scale);
  calculate_pivot_points ();
  redraw_all ();
}

int abstract_plot_drawer::scale () const
{
  return m_scale;
}

void abstract_plot_drawer::set_centered ()
{
  m_view->centerOn (0, 0);
  m_view->ensureVisible (QRect (-10, 10, 20, 20));
}

void abstract_plot_drawer::set_default ()
{
  if (!m_view)
    m_view = new graphics_view;
  m_x_min = 0;
  m_x_max = 2;
  m_scale = 30;
  m_view->set_scale (m_scale);
  m_smooth = 5;
  calculate_pivot_points ();
  setBackgroundBrush (QBrush(QColor (230, 230, 230)));
}

QPen abstract_plot_drawer::set_pen (const int graph_num) const
{
  QPen pen;
  pen.setColor (m_plot_model->paint_config (graph_num, graph_role::color).value<QColor> ());
  pen.setWidth (m_plot_model->paint_config (graph_num, graph_role::width).toInt ());
  pen.setStyle (Qt::SolidLine);
  pen.setCapStyle (Qt::RoundCap);
  pen.setJoinStyle (Qt::RoundJoin);
  pen.setCosmetic (true);
  return pen;
}

void abstract_plot_drawer::draw_grid ()
{
  QPen pen;
  pen.setColor(Qt::black);
  pen.setCosmetic (true);
  pen.setStyle (Qt::SolidLine);
  double coef = 80.0 / m_scale / m_scale;
  QPointF o (0, 0);
  QPointF x_axis (m_x_max + 5, 0);
  QPointF y_axis (0, 10);
  QPointF x_axis_arrow_u (m_x_max + 5 - coef, coef);
  QPointF x_axis_arrow_d (m_x_max + 5 - coef, -coef);
  QPointF y_axis_arrow_l (-coef, 10 - coef);
  QPointF y_axis_arrow_r (coef, 10 - coef);
  addLine (QLineF (o, x_axis), pen);
  addLine (QLineF (o, y_axis), pen);
  addLine (QLineF (x_axis_arrow_u, x_axis), pen);
  addLine (QLineF (x_axis_arrow_d, x_axis), pen);
  addLine (QLineF (y_axis_arrow_l, y_axis), pen);
  addLine (QLineF (y_axis_arrow_r, y_axis), pen);
  QGraphicsSimpleTextItem *zero = addSimpleText ("0", QFont ("labels", 1, 1));
  zero->setPos(-5 * coef,  -5 * coef);
  zero->setScale (0.5);
  addLine (1, -coef, 1, coef, pen);
//  addLine (1, -0.5, 1, 0.5, pen);
  addLine (-coef, 1, coef, 1, pen);
}

void abstract_plot_drawer::draw_graph (const int graph_num)
{
  if (!m_plot_model->paint_config (graph_num, graph_role::shown).toBool ())
    return;

  QPen pen = set_pen (graph_num);

  double a, b;
  m_plot_model->bounds (graph_num, a, b);
  if (a < m_x_min)
    m_x_min = a;
  if (b > m_x_max)
    m_x_max = b;

  double x = a;

  QPointF first, second;

  first = m_plot_model->point_by_x (graph_num, x);
  first.setX (first.x ());
  first.setY (first.y ());

  double hx = (b - a)/(m_pivot_count - 1);
  for (int i = 1; i < m_pivot_count; i++)
    {
      x = a + hx * i;

      second = m_plot_model->point_by_x (graph_num, x);

      second.setX (second.x ());
      second.setY (second.y ());

      addLine (first.x (), first.y (), second.x (), second.y (), pen);
      first = second;
    }
}

void abstract_plot_drawer::redraw_all ()
{
  clear ();
  setSceneRect (m_x_min - 1, -1, m_x_max - m_x_min + 11, 10);
  int graphs_count = m_plot_model->graphs_count ();

  for (int i = 0; i < graphs_count; i++)
    draw_graph (i);
  draw_grid ();
}

void abstract_plot_drawer::on_model_changed ()
{
  redraw_all ();
}

void abstract_plot_drawer::calculate_pivot_points ()
{
  int grid_columns = (int)ceil(m_x_max - m_x_min) ;
  m_pivot_count = grid_columns * m_smooth * m_scale;
  if (m_pivot_count < 2)
    m_pivot_count = 2;
}
