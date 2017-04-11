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


  QPoint o (m_oy_shift, d_height - m_ox_shift);
  QPoint x (d_width, d_height - m_ox_shift);
  QPoint y (m_oy_shift, 0);

  drawLine (o, x);
  drawLine (o, y);

  drawText (to_scale (QPointF (m_x_min, 0)).x (), d_height - m_ox_shift + 15, QString::number (m_x_min));
  double med = (m_x_max + m_x_min) / 2;
  drawText (to_scale (QPointF (med, 0)).x (), d_height - m_ox_shift + 15, QString::number (med));
  drawText (to_scale (QPointF (m_x_max, 0)).x (), d_height - m_ox_shift + 15, QString::number (m_x_max));

  drawText (0, to_scale (QPointF (0, m_y_min)).y (), QString::number (m_y_min, 'e', 2));
  med = (m_y_max + m_y_min) / 2;
  drawText (0, to_scale (QPointF (0, med)).y (), QString::number (med, 'e', 2));
  drawText (0, to_scale (QPointF (0, m_y_max)).y (), QString::number (m_y_max, 'e', 2));
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
    calculate_window_rect ();
    draw_axis ();


//  setViewport (m_axis_shift, m_axis_shift, device ()->width () - m_axis_shift, device ()->height () - m_axis_shift);
//  setWindow (rect);


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

void graph_painter::calculate_window_rect ()
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

  double width = m_x_max - m_x_min;
  double height = m_y_max - m_y_min;


  int d_width = device ()->width ();
  int d_height = device ()->height ();

  int work_height = d_height  - 2 * m_graphs_shift - m_ox_shift;
  int work_width = d_width - 2 * m_graphs_shift - m_oy_shift;

  if (height > 1e-35)
    m_y_scale = (work_height) / (m_y_max - m_y_min);
  else
    m_y_scale = (work_height) / 1e-30;



  if (width > 1e-35)
    m_x_scale = (work_width) / (m_x_max - m_x_min);
  else
    m_x_scale = (work_width) / 1e-30;

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
  return QPointF ((point.x () - m_x_min) * m_x_scale + m_oy_shift + m_graphs_shift,
                  (point.y () - m_y_max) * -m_y_scale + m_graphs_shift);
}

void graph_painter::draw_line (QPointF point_a, QPointF point_b)
{
  drawLine (to_scale (point_a), to_scale (point_b));
}
