#include "abstract_plot_drawer.h"
#include "abstract_plot_model.h"
#include "graph_painter.h"
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>

abstract_plot_drawer::abstract_plot_drawer (QWidget *parent) : QWidget (parent)
{
  set_default ();
}

abstract_plot_drawer::~abstract_plot_drawer ()
{
 if (m_painter)
   {
     delete m_painter;
   }
}

void abstract_plot_drawer::set_model (abstract_plot_model *model)
{
  m_plot_model = model;
  connect (m_plot_model, SIGNAL (model_changed ()), this, SLOT (on_model_changed ()));
//  double l = -200;
//  double t = 200;
//  double r = 200;
//  double b = -200;
//  QRectF f(l, -t, r - l, t - b);
//  setSceneRect (f);

  redraw_all ();
//      m_painter->drawLine (l, t, r, b);
//       m_view->centerOn (0, 0);
//      m_view->ensureVisible (f);
//  f = sceneRect ();
}


//void abstract_plot_drawer::set_scale (const int scale)
//{
//  calculate_pivot_points ();
//  redraw_all ();
//}


void abstract_plot_drawer::paintEvent(QPaintEvent *event)
{
    m_painter->begin (this);
  (void)event;
  int graphs_count = m_plot_model->graphs_count ();

  for (int i = 0; i < graphs_count; i++)
    draw_graph (i);
  draw_grid ();
  m_painter->end ();

}

void abstract_plot_drawer::set_default ()
{
  if (!m_painter)
    m_painter = new graph_painter;
  m_x_min = 0;
  m_x_max = 2;
  m_y_min = 0;
  m_y_max = 2;
  m_smooth = 5;
  calculate_pivot_points ();
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
  pen.setWidth (0);
  m_painter->setPen (pen);
  double l_axis, r_axis, u_axis, b_axis;
  double coef = 5;
  calculate_axis_length (l_axis, r_axis, u_axis, b_axis);
  QPointF o (0, 0);
  QPointF x_axis (width (), 0);
  QPointF y_axis (0, height ());
  QPointF tail_x_axis (-l_axis, 0);
  QPointF tail_y_axis (0, -b_axis);
  QPointF x_axis_arrow_u (r_axis - coef, coef);
  QPointF x_axis_arrow_d (r_axis - coef, -coef);
  QPointF y_axis_arrow_l (-coef, u_axis - coef);
  QPointF y_axis_arrow_r (coef, u_axis - coef);
  m_painter->drawLine (QLineF (o, x_axis));
  m_painter->drawLine (QLineF (o, y_axis));
  m_painter->drawLine (QLineF (tail_x_axis, o));
  m_painter->drawLine (QLineF (tail_y_axis, o));
  m_painter->drawLine (QLineF (x_axis_arrow_u, x_axis));
  m_painter->drawLine (QLineF (x_axis_arrow_d, x_axis));
  m_painter->drawLine (QLineF (y_axis_arrow_l, y_axis));
  m_painter->drawLine (QLineF (y_axis_arrow_r, y_axis));
  m_painter->drawLine (1, -coef, 1, coef);
  m_painter->drawLine (-coef, 1, coef, 1);

  int max = (abs(m_x_max) > abs(m_x_min)) ? m_x_max : m_x_min;
  max = abs (max);
  if (max < abs (m_y_max))
    max = abs(m_y_max);
  if (max < abs (m_y_min))
    max = abs(m_y_min);
}

void abstract_plot_drawer::draw_graph (const int graph_num)
{
  if (!m_plot_model->paint_config (graph_num, graph_role::shown).toBool ())
    return;

  QPen pen = set_pen (graph_num);
  m_painter->setPen (pen);
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

  if (first.y () < m_y_min)
    m_y_min = first.y ();
  if (first.y () > m_y_max)
    m_y_max = first.y ();

  double hx = (b - a)/(m_pivot_count - 1);
  for (int i = 1; i < m_pivot_count; i++)
    {
      x = a + hx * i;


      second = m_plot_model->point_by_x (graph_num, x);

      second.setX (second.x ());
      second.setY (second.y ());

      m_painter->drawLine (first, second);
      first = second;
      if (first.y () < m_y_min)
        m_y_min = first.y ();
      if (first.y () > m_y_max)
        m_y_max = first.y ();
    }
}

void abstract_plot_drawer::redraw_all ()
{
  update ();

}

void abstract_plot_drawer::on_model_changed ()
{
  redraw_all ();
}

void abstract_plot_drawer::calculate_pivot_points ()
{
  int grid_columns = (int)ceil(m_x_max - m_x_min) ;
  m_pivot_count = grid_columns * m_smooth;
  if (m_pivot_count < 2)
    m_pivot_count = 2;
}

void abstract_plot_drawer::calculate_axis_length (double &l_axis, double &r_axis, double &u_axis, double &b_axis)
{
  double pixels_in_x;
  if (fabs (m_x_max - m_x_min) < 1e-15)
    std::abort ();

  pixels_in_x = width () / (m_x_max - m_x_min);

  if (m_x_min > 0)
    l_axis = 0;
  else
    l_axis = pixels_in_x * (-m_x_min);

  if (m_x_max < 0)
    r_axis = 0;
  else
    r_axis = (m_x_max > m_axis_max) ? m_axis_max : m_x_max + 1;

  if (m_y_max < 0)
    u_axis = 2;
  else
    u_axis = (m_y_max > m_axis_max) ? m_axis_max : abs (m_y_max) + 1;

  if (m_y_min > 0)
    b_axis = 0;
  else
    b_axis = (m_y_min < -m_axis_max) ? m_axis_max : abs (m_y_min) + 1;


}

