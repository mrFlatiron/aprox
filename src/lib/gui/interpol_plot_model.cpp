#include "interpol_plot_model.h"
#include "interpol/interpol_factory.h"
#include "interpol/interpol_c_splines_w_derivs.h"
#include <QColor>

interpol_plot_model::interpol_plot_model (const double x_min,
                                          const double x_max,
                                          const int points_count) :
  m_shown_count (0),
  m_interpols ((int)interpol::polynom_type::COUNT),
  m_additional_vectors ((int)interpol::polynom_type::COUNT),
  m_additional_funcs ((int)interpol::polynom_type::COUNT),
  m_interpol_shown ((int)interpol::polynom_type::COUNT)
{
  set_meta (x_min, x_max, points_count);
}

interpol_plot_model::~interpol_plot_model ()
{

}

void interpol_plot_model::set_meta (const double x_min, const double x_max, const int points_count)
{
  m_x_min = (x_min < x_max) ? x_min : x_max;
  m_x_max = (x_min > x_max) ? x_min : x_max;
  m_points_count = abs (points_count);
}

void interpol_plot_model::add_interpol (const interpol::polynom_type type,
                               const std::vector<double> &additional)
{
  int id = (int)type;
  {
  using namespace interpol;
  switch (type)
    {
    case polynom_type::c_spline_w_derivs:
     m_additional_vectors[id] = additional;
      m_interpols[id].reset (create_polynom
                                          <c_splines_w_derivs>
                                          (m_x_min, m_x_max, (unsigned int)m_points_count,
                                           m_origin, additional));
      m_interpol_shown[id] = true;
      m_shown_count++;
      emit model_changed ();
      break;
    case polynom_type::newton_mult_nodes:
      std::abort ();
    case polynom_type::COUNT:
      std::abort ();
    }
  }
}

void interpol_plot_model::add_interpol (const interpol::polynom_type type,
                                        std::function<double (const double)> d)
{
  int id = (int)type;
  {
    using namespace interpol;
    switch (type)
      {
      case polynom_type::newton_mult_nodes:
        m_additional_funcs[id] = d;
        m_interpols[id].reset (create_polynom <newton_mult_nodes>(m_x_min, m_x_max,
                                                                        (unsigned int)m_points_count,
                                                                        m_origin, d));
        m_interpol_shown[id] = true;
        m_shown_count++;
        emit model_changed ();
        break;
      case polynom_type::c_spline_w_derivs:
        std::abort ();
      case polynom_type::COUNT:
        std::abort ();
      }
  }
}

void interpol_plot_model::set_origin_func (const std::function<double (const double)> &origin)
{
  m_origin = origin;
}

int interpol_plot_model::get_points_count () const
{
  return m_points_count;
}

void interpol_plot_model::set_points_count (const int points_count)
{
  if (points_count <= 1)
      std::abort ();
  m_points_count = points_count;
  reinterpolate ();
}

void interpol_plot_model::reinterpolate ()
{
  int size = (int)m_interpols.size ();
  for (int i = 0; i < size; i++)
    {
      if (m_interpols[i].get ())
        {
          switch (m_interpols[i]->get_add_type ())
            {
            case interpol::additional_array_size::const_size:
              m_interpols[i]->interpolate_function (m_x_min, m_x_max, m_points_count,
                                                m_origin, m_additional_vectors[i]);
              break;
            case interpol::additional_array_size::x_size:
              m_interpols[i]->interpolate_function (m_x_min, m_x_max, m_points_count,
                                                    m_origin, m_additional_funcs[i]);
              break;
            }
        }
    }
  emit model_changed ();
}

int interpol_plot_model::graphs_count () const
{
  return m_interpol_shown.size () + 1; //origin is always shown
}

QPointF interpol_plot_model::point_by_x (const int graph_num, const double x) const
{
  int id = graph_num - 1;
  switch (graph_num)
    {
    case 0:
      return QPointF (x, m_origin (x));
    default:
      interpol::polynom *p = m_interpols[id].get ();
      return QPointF (x, p->eval (x));
    }
}

QVariant interpol_plot_model::paint_config (const int graph_num, const graph_role role) const
{
  int id = graph_num - 1;
  interpol::polynom_type type = (interpol::polynom_type)id;
  switch (graph_num)
    {
    case 0:
      switch (role)
        {
        case graph_role::color:
          return QColor (Qt::green);
        case graph_role::width:
          return 3;
        case graph_role::shown:
          return true;
        }
    default:
      switch (role)
        {
        case graph_role::color:
          return get_color (type);
        case graph_role::width:
          return 3;
        case graph_role::shown:
          return m_interpol_shown[id];
        }
    }
  return QVariant ();
}

void interpol_plot_model::bounds (const int graph_num, double &left, double &right) const
{
  (void)graph_num;
  left = m_x_min;
  right = m_x_max;
}

QColor interpol_plot_model::get_color (const interpol::polynom_type type) const
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

//void interpol_plot_model::on_points_count_changed (int points_count)
//{
//  m_points_count = abs (points_count);
//  for (int i = 0; i < (int)(m_interpols.size ()); i++)
//    {
//      interpol::polynom *p = m_interpols[i].get ();
//      p->interpolate_function (m_x_min, m_x_max, m_points_count,
//                                        m_origin, m_additionals[type]);
//    }
//  emit model_changed ();
//}



