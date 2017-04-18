#include "main_window_v2.h"

#include <QSpinBox>
#include <QLabel>
#include <QString>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "interpol_plot_model.h"
#include "graph_painter.h"
#include "plot_widget.h"
#include "greet_window.h"
#include "test_functions/test_functions.h"
#include "interpol/interpol_tools.h"

main_window_v2::main_window_v2 (const double x_min, const double x_max, const int points_count) : QDialog ()
{
  double a = x_min,
      b = x_max;
  int pc = abs (points_count);
  if (x_min > x_max)
    {
      a = x_max;
      b = x_min;
    }
  if (pc < 2)
    pc = 2;

  m_plot_model = new interpol_plot_model (a, b, pc);
  m_plot_model->set_origin_func (func_to_aprox);
  m_plot_model->add_interpol (interpol::polynom_type::newton_mult_nodes, deriv);
  m_plot_model->add_interpol (interpol::polynom_type::c_spline_w_derivs,
  {deriv (x_min), deriv (x_max)});
  m_plot_model->set_only_origin_visible ();
  m_graph_painter = new graph_painter;
  m_graph_painter->set_model (m_plot_model);
  m_plot_drawer = new plot_widget (m_graph_painter, this);


  m_points_count_edit = new QSpinBox (this);
  m_points_count_edit->setValue (pc);
  m_points_count_edit->setMinimum (2);

  m_mode = graph_mode::origin;
  m_points_count_edit->setDisabled (true);

  m_menu_bar = new QMenuBar (this);

  set_layouts ();
  do_connects ();
}

main_window_v2::~main_window_v2 ()
{
  if (m_graph_painter)
    delete m_graph_painter;
  if (m_plot_model)
    delete m_plot_model;
}

QSize main_window_v2::sizeHint () const
{
  return QSize (1024, 780);
}

void main_window_v2::set_layouts ()
{
  QVBoxLayout *vlo_1 = new QVBoxLayout;
  {
    for (int mode = 0; mode != (int)graph_mode::COUNT; mode++)
      {
        QString label = get_action_name ((graph_mode)mode);
        m_actions[mode] = m_menu_bar->addAction (label);
      }

    vlo_1->addWidget (m_menu_bar, 1, Qt::AlignLeft | Qt::AlignTop);

    vlo_1->addWidget (m_plot_drawer);

    vlo_1->addWidget (m_points_count_edit, 0, Qt::AlignLeft);
  }
  this->setLayout (vlo_1);
}

void main_window_v2::do_connects ()
{
  connect (m_menu_bar, SIGNAL (triggered (QAction *)), this, SLOT (on_menu_bar (QAction *)));
  connect (m_points_count_edit, SIGNAL (valueChanged (int)), this, SLOT (on_points_count_changed (int)));
  connect (m_plot_model, SIGNAL (model_changed ()), m_plot_drawer, SLOT (update ()));
}

QString main_window_v2::get_action_name (const graph_mode mode)
{
  switch (mode)
    {
    case graph_mode::origin:
      return "&Origin (green)";
    case graph_mode::newton_mult_nodes:
      return "&Newton (blue)";
    case graph_mode::discrepancy_newton:
      return "Newton discrepancy (yellow)";
    case graph_mode::c_splines_w_derivs:
      return "&Cubic splines (red)";
    case graph_mode::discrepancy_splines:
      return "Splines discrepancy (yellow)";
    case graph_mode::COUNT:
      return "Not implemented";
    }
}

void main_window_v2::on_menu_bar (QAction *action)
{
  if (action == m_actions[(int)graph_mode::origin])
    {
      m_plot_model->set_only_origin_visible ();
      m_points_count_edit->setDisabled (true);
      return ;
    }
  if (action == m_actions[(int)graph_mode::newton_mult_nodes])
    {
      m_plot_model->set_interpol_visible ((int)interpol::polynom_type::newton_mult_nodes);
      m_points_count_edit->setEnabled (true);
      m_points_count_edit->setMaximum (50);
      return;
    }
  if (action == m_actions[(int)graph_mode::discrepancy_newton])
    {
      m_plot_model->set_only_discr_visible ((int)interpol::polynom_type::newton_mult_nodes);
      m_points_count_edit->setEnabled (true);
      m_points_count_edit->setMaximum (50);
      return;
    }
  if (action == m_actions[(int)graph_mode::c_splines_w_derivs])
    {
      m_plot_model->set_interpol_visible ((int)interpol::polynom_type::c_spline_w_derivs);
      m_points_count_edit->setEnabled (true);
      m_points_count_edit->setMaximum (10000);
      return;
    }
  if (action == m_actions[(int)graph_mode::discrepancy_splines])
    {
      m_plot_model->set_only_discr_visible ((int)interpol::polynom_type::c_spline_w_derivs);
      m_points_count_edit->setEnabled (true);
      m_points_count_edit->setMaximum (10000);
      return;
    }
  return;
}

void main_window_v2::on_points_count_changed (int points_count)
{
   m_plot_model->set_points_count (points_count);
}

