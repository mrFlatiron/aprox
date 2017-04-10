#include "main_window.h"
#include "abstract_plot_drawer.h"
#include "interpol_plot_model.h"
#include "interpol/interpol_factory.h"
#include "greet_window.h"
#include "test_functions/test_functions.h"
#include <QGraphicsView>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "gui_checkbox_group.h"
#include "stretch.h"
#include "plot_widget.h"
#include "graph_painter.h"

main_window::main_window (QWidget *parent) : QDialog (parent)
{
  setWindowTitle ("Aprox");

  open_greetings_window ();
  set_layouts ();
}

main_window::~main_window ()
{
  if (m_plot_model)
    delete m_plot_model;
}

QSize main_window::sizeHint() const
{
  return QSize (1380, 900);
}

void main_window::set_layouts ()
{
  QVBoxLayout *vlo_1 = new QVBoxLayout;
  {
    vlo_1->addWidget (m_plot_drawer);
    int spinbox_size;
    int label_size;

    QHBoxLayout *hlo_1 = new QHBoxLayout;
    {
      int pc = m_plot_model->get_points_count ();
      m_points_count_edit = new QSpinBox (this);
      m_points_count_edit->setMinimum (2);
      m_points_count_edit->setMaximum (10000000);
      spinbox_size = m_points_count_edit->width ();
      m_points_count_edit->setFixedWidth (spinbox_size);
      m_points_count_edit->setValue (pc);
      connect (m_points_count_edit, SIGNAL (valueChanged (int)),
               this, SLOT (on_pc_slider_moved (int)));
      QLabel *l = new QLabel ("interpol points", this);
      label_size = l->width ();
      l->setFixedWidth (label_size);
      hlo_1->addWidget (l);
      hlo_1->addWidget (m_points_count_edit);
      hlo_1->addWidget (new stretch (this), 1);
    }
    vlo_1->addLayout (hlo_1);

    std::vector<QString> labels;
    labels.push_back ("origin");

    auto temp_labels = interpol::get_interpol_labels ();
    for (auto val : temp_labels)
      labels.push_back (val);

    gui_checkbox_group *visible_graphs_box = new gui_checkbox_group (labels, this);
    connect (visible_graphs_box, SIGNAL (box_toggled (int, bool)), m_plot_model, SLOT (change_visible_graphs (int, bool)));
    vlo_1->addLayout (visible_graphs_box->as_layout ());
  }
  setLayout (vlo_1);
}

void main_window::open_greetings_window ()
{
  greet_window g_window (this);
  if (QDialog::Accepted != g_window.exec ())
    std::abort ();
  data_source type = g_window.get_source_type ();
  QString path;
  switch (type)
    {
    case data_source::file:
      path = g_window.get_path ();
      fprintf (stderr, "Not Implemented\n");
      std::abort ();
      break;
    case data_source::function:
      double min = g_window.get_min ();
      double max = g_window.get_max ();
      if (min > max)
        {
          double buf = min;
          min = max;
          max = buf;
        }
      graph_painter *plot_painter = new graph_painter;

      m_plot_model = new interpol_plot_model (min, max, 2);
      m_plot_model->set_origin_func (std::function<double(const double)> (func_to_aprox));
      m_plot_model->add_interpol (interpol::polynom_type::c_spline_w_derivs,
      {deriv (min), deriv (max)});
      m_plot_model->add_interpol (interpol::polynom_type::newton_mult_nodes, deriv);
      plot_painter->set_model (m_plot_model);
      m_plot_drawer = new plot_widget (plot_painter, this);
      connect (m_plot_model, SIGNAL (model_changed ()), m_plot_drawer, SLOT (update ()));
      break;
    }

}


void main_window::on_pc_slider_moved (int val)
{
  m_plot_model->set_points_count (val);
}
