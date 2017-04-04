#include "plot_scene.h"
#include "interpol/interpol_c_splines_w_derivs.h"
#include "interpol/interpol_factory.h"
#include "interpol/interpol_newton_mult_nodes.h"
#include <functional>
#include <QPointF>
#include <QPainter>

plot_scene::plot_scene (QObject *parent) : QGraphicsScene (parent),
  m_scale (5),
  m_smooth (5),
  m_x_min (0),
  m_x_max (0),
  m_y_min (-5),
  m_y_max (5),
  m_points_count (0),
  m_origin (),
  m_interpols_count (0)
{
  calculate_pivot_poins ();
}

plot_scene::plot_scene (const double a, const double b,
                       std::function<double (const double)> origin,
                       QObject *parent) :
  QGraphicsScene (parent),
  m_scale (5),
  m_smooth (5),
  m_x_min (a),
  m_x_max (b),
  m_y_min (-5),
  m_y_max (5),
  m_points_count (0),
  m_origin (origin),
  m_interpols_count (0)
{
  calculate_pivot_poins ();
}


int plot_scene::get_points_count () const
{
  return m_points_count;
}

void plot_scene::draw_interpol (const interpol::polynom_type type)
{
  interpol::polynom *p = m_interpols[type].get ();
  if (!p) return;
  draw_function<interpol::polynom> (p, get_color (type));
}

void plot_scene::draw_origin ()
{
  if (m_origin)
    draw_function<std::function<double (const double)>> (&m_origin, Qt::green);
}

inline qreal round(qreal val, int step) {
   int tmp = int(val) + step /2;
   tmp -= tmp % step;
   return qreal(tmp);
}

void plot_scene::draw_grid ()
{
  QPen pen;

  pen.setStyle (Qt::SolidLine);
  pen.setWidth (0);
  pen.setBrush (QBrush ());
  pen.setColor (QColor(200, 200, 255, 125));
  pen.setCapStyle (Qt::RoundCap);
  pen.setJoinStyle (Qt::RoundJoin);

  QRectF rect = sceneRect ();

  // draw horizontal grid
  qreal start = round (rect.top (), m_scale);

  if (start > rect.top ()) {
     start -= m_scale;
  }

  for (qreal y = start - m_scale; y < rect.bottom (); ) {
     y += m_scale;
     addLine (rect.left (), y, rect.right (), y, pen);
  }

  // now draw vertical grid
  start = round (rect.left (), m_scale);
  if (start > rect.left ()) {
     start -= m_scale;
  }
  for (qreal x = start - m_scale; x < rect.right (); ) {
     x += m_scale;
     addLine (x, rect.top (), x, rect.bottom (), pen);
    }


}


void plot_scene::set_origin_func (const std::function<double (const double)> &origin)
{
  m_origin = origin;
}

void plot_scene::set_scale (const int scale)
{
  m_scale = scale;
  calculate_pivot_poins ();
}

int plot_scene::get_scale() const
{
  return m_scale;
}

void plot_scene::set_smooth(const int smooth)
{
  m_smooth = abs (smooth);
  calculate_pivot_poins ();
}



void plot_scene::set_points_count (const int points_count)
{
  m_points_count = points_count;
}

void plot_scene::set_bounds (const double a, const double b)
{
  if (a == b)
    return;
  if (a < b)
    {
      m_x_min = a;
      m_x_max = b;
    }
  else
    {
      m_x_min = b;
      m_x_max = a;
    }
  calculate_pivot_poins ();
}

void plot_scene::add_interpol (const interpol::polynom_type type,
                               const std::vector<double> &additional)
{
  {
  using namespace interpol;
  switch (type)
    {
    case polynom_type::c_spline_w_derivs:
      m_additionals[type] = additional;
      m_interpols.insert (std::make_pair (type, std::unique_ptr<polynom> (create_polynom
                                          <c_splines_w_derivs>
                                          (m_x_min, m_x_max, (unsigned int)m_points_count,
                                           m_origin, additional))));
      m_interpols_count++;
      draw_interpol (type);
      draw_function<std::function<double(const double)>> (&m_origin, Qt::green);
      break;
    case polynom_type::newton_mult_nodes:
//      m_interpols.insert (std::make_pair (type, create_polynom
//                                          <newton_mult_nodes>
//                                          (m_x_min, m_x_max, (unsigned int)m_points_count,
//                                           m_origin, additional)));
//      if (m_interpols[type] != nullptr)
//        m_interpols_count++;
      std::abort ();
      break;
    case polynom_type::COUNT:
      std::abort ();
    }
  }
}

QPointF plot_scene::get_center () const
{
  double c = (m_x_max + m_x_min)/2;
  return QPointF (c, m_origin (c));
}

void plot_scene::on_points_count_changed (int points_count)
{
  m_points_count = points_count;
  for (auto &val : m_interpols)
    {
      auto type = val.first;
      val.second->interpolate_function (m_x_min, m_x_max, m_points_count,
                                        m_origin, m_additionals[type]);
    }
  redraw_all ();
}

void plot_scene::on_scale_changed (int val)
{
  set_scale (val);
}

void plot_scene::calculate_pivot_poins ()
{
  int grid_columns = (int)ceil(m_x_max - m_x_min) ;
  m_pivot_points = grid_columns * m_smooth;
  redraw_all ();
}

Qt::GlobalColor plot_scene::get_color (const interpol::polynom_type type)
{
  switch (type)
    {
    case interpol::polynom_type::c_spline_w_derivs:
      return Qt::red;
    case interpol::polynom_type::newton_mult_nodes:
      return Qt::blue;
    case interpol::polynom_type::COUNT:
      std::abort ();
      return Qt::black;
    }
  return Qt::black;
}

void plot_scene::try_set_y_bounds (const double y_min, const double y_max)
{
  if (y_min < m_y_min)
    m_y_min = y_min;
  if (y_max > m_y_max)
    m_y_max = y_max;

}

void plot_scene::redraw_all ()
{
  clear ();
  for (auto &val : m_interpols)
    {
      draw_interpol (val.first);
    }
  draw_origin ();
  draw_grid ();
}

plot_scene::~plot_scene ()
{

}
