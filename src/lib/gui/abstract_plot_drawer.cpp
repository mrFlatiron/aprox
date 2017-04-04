#include "abstract_plot_drawer.h"
#include "abstract_plot_model.h"
#include "graphics_view.h"

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
  m_scale = scale;
  redraw_all ();
}

int abstract_plot_drawer::scale () const
{
  return m_scale;
}

void abstract_plot_drawer::set_centered ()
{
  m_view->centerOn (0, 0);
  m_view->ensureVisible (0, 0, 50, 50);
}

void abstract_plot_drawer::set_default ()
{
  if (!m_view)
    m_view = new graphics_view;
  m_x_min = 0;
  m_x_max = 2;
  m_scale = 200;
  m_smooth = 5;
  calculate_pivot_points ();
  setBackgroundBrush (QBrush(QColor (Qt::lightGray)));
}

QPen abstract_plot_drawer::set_pen (const int graph_num) const
{
  QPen pen;
  pen.setColor (m_plot_model->paint_config (graph_num, graph_role::color).value<QColor> ());
  pen.setWidth (m_plot_model->paint_config (graph_num, graph_role::width).toInt ());
  pen.setStyle (Qt::SolidLine);
  pen.setCapStyle (Qt::RoundCap);
  pen.setJoinStyle (Qt::RoundJoin);
  return pen;
}

void abstract_plot_drawer::draw_grid ()
{
  QPointF o (0, 0);
  QPointF x_axis (30 * m_scale, 0);
  QPointF y_axis (0, 5 * m_scale);
  addLine (QLineF (o, x_axis));
  addLine (QLineF (o, y_axis));
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
  first.setX (first.x () * m_scale);
  first.setY (first.y () * m_scale);

  double hx = (b - a)/(m_pivot_count - 1);
  for (int i = 1; i < m_pivot_count; i++)
    {
      x = a + hx * i;

      second = m_plot_model->point_by_x (graph_num, x);

      second.setX (second.x () * m_scale);
      second.setY (second.y () * m_scale);

      addLine (first.x (), first.y (), second.x (), second.y (), pen);
      first = second;
    }
}

void abstract_plot_drawer::redraw_all ()
{
  clear ();
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
