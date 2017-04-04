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
#include "stretch.h"

main_window::main_window (QWidget *parent) : QDialog (parent)
{
  setWindowTitle ("Aprox");

  open_greetings_window ();
  set_layouts ();
}

main_window::~main_window()
{

}

QSize main_window::sizeHint() const
{
  return QSize (600, 600);
}

void main_window::set_layouts ()
{

  QVBoxLayout *vlo_main = new QVBoxLayout;
  {
    vlo_main->addWidget (m_plot_drawer->get_view ());
    int spinbox_size;
    int label_size;
    QHBoxLayout *hlo_1 = new QHBoxLayout;
    {
      int pc = m_plot_model->get_points_count ();
      m_points_count_edit = new QSpinBox (this);
      m_points_count_edit->setMinimum (2);
      m_points_count_edit->setMaximum (10000000);
      spinbox_size = m_points_count_edit->width ();
      m_points_count_edit->setValue (pc);
      connect (m_points_count_edit, SIGNAL (valueChanged (int)),
               this, SLOT (on_pc_slider_moved (int)));
      QLabel *l = new QLabel ("interpol points:", this);
      label_size = l->width ();
      hlo_1->addWidget (l);
      hlo_1->addWidget (m_points_count_edit);
      hlo_1->addWidget (new stretch (this), 1);
    }
    vlo_main->addLayout (hlo_1);
    QHBoxLayout *hlo_2 = new QHBoxLayout;
    {
      int scale = m_plot_drawer->scale ();
      m_scale_edit = new QSpinBox (this);
      m_scale_edit->setMinimum (1);
      m_scale_edit->setMaximum (1000);
      m_scale_edit->setFixedWidth (spinbox_size -4);
      m_scale_edit->setValue (scale);
      connect (m_scale_edit, SIGNAL (valueChanged (int)),
               this, SLOT (on_scale_slider_moved (int)));
      QLabel *l = new QLabel ("scale:", this);
      l->setFixedWidth (label_size - 5);
      hlo_2->addWidget (l);
      hlo_2->addWidget (m_scale_edit);
      hlo_2->addWidget (new stretch (this), 1);
    }
    vlo_main->addLayout (hlo_2);
  }
  QPushButton *center = new QPushButton ("to center");
  connect (center, SIGNAL (clicked ()), m_plot_drawer, SLOT (set_centered ()));
  vlo_main->addWidget (center, 0, Qt::AlignLeft);
  setLayout (vlo_main);
  m_plot_drawer->show ();
}

void main_window::open_greetings_window ()
{
  greet_window *g_window = new greet_window (this);
  g_window->exec ();
  data_source type = g_window->get_source_type ();
  QString path;
  switch (type)
    {
    case data_source::file:
      path = g_window->get_path ();
      break;
    case data_source::function:
      m_plot_model = new interpol_plot_model (0, 10, 2);
      m_plot_model->set_origin_func (std::function<double(const double)> (func_to_aprox));
      m_plot_model->add_interpol (interpol::polynom_type::c_spline_w_derivs,
      {cubic_d (0), cubic_d (10)});
      m_plot_model->add_interpol (interpol::polynom_type::newton_mult_nodes, cubic_d);
      m_plot_drawer = new abstract_plot_drawer (this);
      m_plot_drawer->set_model (m_plot_model);
      break;
    }

}


void main_window::on_pc_slider_moved (int val)
{
  m_plot_model->set_points_count (val);
}

void main_window::on_scale_slider_moved (int val)
{
  m_plot_drawer->set_scale (val);
}
