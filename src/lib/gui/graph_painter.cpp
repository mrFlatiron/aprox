#include "graph_painter.h"
#include "abstract_plot_model.h"

graph_painter::graph_painter () : QPainter ()
{

}

graph_painter::graph_painter (QPaintDevice *device) : QPainter (device)
{

}

void graph_painter::draw_axis ()
{
  int d_width = device ()->width ();
  int d_height = device ()->height ();

  int m_axis_shift = 15;

  QPoint o (m_axis_shift, d_height - m_axis_shift);
  QPoint x (d_width - m_axis_shift, d_height - m_axis_shift);
  QPoint y (m_axis_shift, m_axis_shift);

  drawLine (o, x);
  drawLine (o, y);
  drawText (50, d_height, "1");
//  setViewport (10, 10, d_width - 10, d_height - 10);
}

void graph_painter::draw_graph (const int graph_num)
{
  if (!m_plot_model->paint_config (graph_num, graph_role::shown).toBool ())
    return;

  QPen pen = set_pen (graph_num);
  setPen (pen);

  double a = m_x_min;
  double b = m_x_max;


  double x = a;

  QPointF first, second;

  first = m_plot_model->point_by_x (graph_num, x);

  double hx = (b - a) / (m_pivot_count - 1);
  for (int i = 1; i < m_pivot_count; i++)
    {
      x = a + hx * i;

      second = m_plot_model->point_by_x (graph_num, x);

      draw_line (first, second);
      first = second;
    }
}

void graph_painter::redraw_all ()
{
  draw_axis ();
  double x_scale, y_scale;
  QRect rect = calculate_window_rect (x_scale, y_scale);
//  setViewport (m_axis_shift, m_axis_shift, device ()->width () - m_axis_shift, device ()->height () - m_axis_shift);
  setWindow (rect);
  m_x_scale = x_scale;
  m_y_scale = y_scale;

  int graphs_count = m_plot_model->graphs_count ();
  for (int i = 0; i < graphs_count; i++)
    {
      draw_graph (i);
    }
}

void graph_painter::set_model (abstract_plot_model *model)
{
  m_plot_model = model;

}


graph_painter::~graph_painter ()
{

}

void graph_painter::calculate_pivot_count ()
{
  m_pivot_count = device()->width () / 100 * m_smooth;
}

QRect graph_painter::calculate_window_rect (double &x_scale, double &y_scale)
{
  int graphs_count = m_plot_model->graphs_count ();
  for (int i = 0; i < graphs_count; i++)
    {
      if (!m_plot_model->paint_config (i, graph_role::shown).toBool ())
          continue;

      double a, b;
      m_plot_model->bounds (i, a, b);
      if (i == 0)
        {
          m_x_min = a;
          m_x_max = b;
        }
      else
        {
          if (a < m_x_min)
            m_x_min = a;
          if (b > m_x_max)
            m_x_max = b;
        }

      calculate_graph_vert_bounds (i, a, b);
      if (i == 0)
        {
          m_y_min = a;
          m_y_max = b;
        }
      else
        {

          if (a < m_y_min)
            m_y_min = a;
          if (b > m_y_max)
            m_y_max = b;
        }
    }

  double len = m_x_max - m_x_min;
  double left = m_x_min - len / 10;
  double width = len + len / 5;

  len = m_y_max - m_y_min;
  double top = m_y_max + len / 10;
  double height = m_y_max - m_y_min + len / 5;
  int d_width = device ()->width ();
  int d_height = device ()->height ();
  int ileft = (int)ceil(left);
  int itop = (int)ceil(top);
  int iwidth = (int)ceil(width);
  int iheight = (int)ceil(height);

  QRect to_ret = QRect (ileft, itop, iwidth, -iheight);

  if (iheight == 1)
    {
      if (m_y_max - m_y_min > 1e-35)
        y_scale = iheight / (m_y_max - m_y_min);
      else
        y_scale = iheight / 1e-30;
    }
  else
    y_scale = 1;

  if (iwidth == 1)
    {
      if (m_x_max - m_x_min > 1e-35)
        x_scale = iwidth / (m_x_max - m_x_min);
      else
        x_scale = iwidth / 1e-30;
    }
  else
    x_scale = 1;

  return to_ret;
//  setTransform (QTransform (10, 0, 0,
//                            0, -10, 0,
//                            0, 0, 1));

//  int side = d_width;
//  int x = (device ()->width() - side / 2);
//  int y = (device ()->height() - side / 2);

 // setViewport(0, 0, d_width, d_height);
//  scale (d_width / width, d_height /  height);
//  scale (1, -1);
//  translate (0, fabs (m_y_min));
//  translate (fabs (m_x_min) * d_width / width, fabs (m_y_max) * d_height / height);
//  setPen (set_pen (0));
//  drawText (m_x_min, m_y_min, "1");
//  drawText (0, 0, "2");
//  drawText (m_x_max, m_y_max, "3");
}

void graph_painter::calculate_graph_vert_bounds (const int graph_num,
                                                 double &loc_min, double &loc_max)
{
  calculate_pivot_count ();

  QPointF first, second;

  double a, b;
  m_plot_model->bounds (graph_num, a, b);

  first = m_plot_model->point_by_x (graph_num, a);
  loc_min = first.y ();
  loc_max = first.x ();

  double x = a;


  double hx = (b - a)/(m_pivot_count - 1);
  for (int j = 1; j < m_pivot_count; j++)
    {
      x = a + hx * j;

      second = m_plot_model->point_by_x (graph_num, x);

      second.setX (second.x ());
      second.setY (second.y ());

      first = second;
      if (first.y () < loc_min)
        loc_min = first.y ();
      if (first.y () > loc_max)
        loc_max = first.y ();
    }
}

QPen graph_painter::set_pen (const int graph_num) const
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

QPointF graph_painter::to_scale (QPointF point)
{
  return QPointF (point.x () * m_x_scale, point.y () * m_y_scale);
}

void graph_painter::draw_line (QPointF point_a, QPointF point_b)
{
  drawLine (to_scale (point_a), to_scale (point_b));
}
