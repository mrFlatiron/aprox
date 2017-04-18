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

interpol_plot_model::interpol_plot_model (const std::vector<double> &xes, const std::vector<double> &ys) :
  m_shown_count (0),
  m_interpols ((int)interpol::polynom_type::COUNT),
  m_additional_vectors ((int)interpol::polynom_type::COUNT),
  m_additional_funcs ((int)interpol::polynom_type::COUNT),
  m_interpol_shown ((int)interpol::polynom_type::COUNT)
{
  set_origin_points (xes, ys);
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
    m_additional_vectors[id] = additional;
    switch (type)
      {
      case polynom_type::c_spline_w_derivs:

        if (!m_origin_xes.size ())
          m_interpols[id].reset (create_polynom
                                 <c_splines_w_derivs>
                                 (m_x_min, m_x_max, (unsigned int)m_points_count,
                                  m_origin, additional));
        else
          m_interpols[id].reset (create_polynom
                                 <c_splines_w_derivs>
                                 (m_origin_xes, m_origin_ys, additional));
        break;
      case polynom_type::newton_mult_nodes:
        m_interpols[id].reset (create_polynom
                               <newton_mult_nodes>
                               (m_origin_xes, m_origin_ys, additional));
        break;
      case polynom_type::COUNT:
        std::abort ();
      }
  }
  m_interpol_shown[id] = true;
  m_shown_count++;
  emit model_changed ();
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

        m_interpols[id].reset (create_polynom
                               <newton_mult_nodes>
                               (m_x_min, m_x_max,
                                (unsigned int)m_points_count,
                                m_origin, d));
        break;
      case polynom_type::c_spline_w_derivs:
        std::abort ();
      case polynom_type::COUNT:
        std::abort ();
      }
  }
  m_interpol_shown[id] = true;
  m_shown_count++;
  emit model_changed ();
}

void interpol_plot_model::set_origin_func (const std::function<double (const double)> &origin)
{
  m_origin = origin;
}

void interpol_plot_model::set_origin_points (const std::vector<double> &xes, const std::vector<double> &ys)
{
  m_origin_xes = xes;
  m_origin_ys = ys;
  m_points_count = xes.size ();
  m_x_min = xes[0];
  m_x_max = xes[m_points_count - 1];

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
  return m_interpol_shown.size () + 2;
}

QPointF interpol_plot_model::point_by_x (const int graph_num, const double x) const
{
  int id = graph_num - 2;
  switch (graph_num)
    {
    case 0: //origin
      return QPointF (x, m_origin (x));
    case 1: //discrepancy
      return QPointF (x, m_origin (x) - m_interpols[m_discrepancy_id]->eval (x));
    default:
      interpol::polynom *p = m_interpols[id].get ();
      return QPointF (x, p->eval (x));
    }
}

QPointF interpol_plot_model::point_by_num (const int graph_num, const int point_num) const
{
  if (graph_num != 0)
    return QPointF ();
  return QPointF (m_origin_xes[point_num], m_origin_ys[point_num]);
}

QVariant interpol_plot_model::paint_config (const int graph_num, const graph_role role) const
{
  int id = graph_num - 2;
  interpol::polynom_type type = (interpol::polynom_type)id;
  switch (graph_num)
    {
    case 0:
      switch (role)
        {
        case graph_role::discrete:
          {
            if (m_origin_xes.size ())
              return true;
            else
              return false;
          }
        case graph_role::points_count:
          return (int)m_origin_xes.size ();
        case graph_role::color:
          return QColor (Qt::green);
        case graph_role::width:
          return 3;
        case graph_role::shown:
          return m_origin_shown;
        }
    case 1:
      switch (role)
        {
        case graph_role::discrete:
          return false;
        case graph_role::points_count:
          return 0;
        case graph_role::color:
          return QColor (Qt::darkYellow);
        case graph_role::width:
          return 3;
        case graph_role::shown:
          if (m_discrepancy_id >= 0)
            return true;
          else
            return false;
        }
    default:
      switch (role)
        {
        case graph_role::discrete:
          return false;
        case graph_role::points_count:
          return 0;
        case graph_role::color:
          return get_color (type);
        case graph_role::width:
          return 3;
        case graph_role::shown:
          return m_interpol_shown[id];
//          return false;
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

void interpol_plot_model::change_visible_graphs (int id, bool shown)
{
  if (shown)
    m_discrepancy_id = -1;
  switch (id)
    {
    case 0:
      m_origin_shown = shown;
      break;
    default:
      m_interpol_shown[id - 1] = shown;
      break;
    }
  emit model_changed ();
}

void interpol_plot_model::change_discrepancy_graph (int id)
{
  m_discrepancy_id = id;
  emit model_changed ();
}

void interpol_plot_model::set_only_origin_visible ()
{

  m_origin_shown = true;
  m_discrepancy_id = -1;
  for (int i = 0; i != (int)interpol::polynom_type::COUNT; i++)
    m_interpol_shown[i] = false;

  emit model_changed ();
}

void interpol_plot_model::set_only_discr_visible (int id)
{
  m_origin_shown = false;
  m_discrepancy_id = id;
  for (int i = 0; i != (int)interpol::polynom_type::COUNT; i++)
    m_interpol_shown[i] = false;

  emit model_changed ();
}

void interpol_plot_model::set_interpol_visible (int id)
{
  m_origin_shown = false;
  m_discrepancy_id = -1;
  m_interpol_shown[id] = true;

  for (int i = 0; i != (int)interpol::polynom_type::COUNT; i++)
    {
      if (i == id)
        continue;
      m_interpol_shown[i] = false;
    }

  emit model_changed ();
}



