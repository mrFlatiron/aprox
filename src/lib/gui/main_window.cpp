#include "main_window.h"
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
#include <QComboBox>

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
  return QSize (1024, 1280);
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

      if (m_source_type == data_source::file)
        m_points_count_edit->setDisabled (true);

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

    QHBoxLayout *hlo_2 = new QHBoxLayout;
    {

      std::vector<QString> labels;
      labels.push_back ("origin");

      auto temp_labels = interpol::get_interpol_labels ();
      for (auto val : temp_labels)
        labels.push_back (val);

      {
        m_visible_graphs_box = new gui_checkbox_group (labels, this);
        connect (m_visible_graphs_box, SIGNAL (box_toggled (int, bool)), m_plot_model,
                 SLOT (change_visible_graphs (int, bool)));
      }
      hlo_2->addLayout (m_visible_graphs_box->as_layout ());

      QVBoxLayout *vlo_2 = new QVBoxLayout;
      {
        m_discrepancy_pb = new QPushButton ("Discrepancy graph");

        connect (m_discrepancy_pb, SIGNAL (clicked ()), this, SLOT (on_discrepancy_pb_clicked ()));
        m_discrepancy_box = new QComboBox (this);
        for (auto val : temp_labels)
          m_discrepancy_box->addItem (val);
        m_discrepancy_box->setCurrentIndex (0);
        connect (m_discrepancy_box, SIGNAL (currentIndexChanged (int)), this, SLOT (on_discrepancy_pb_clicked ()));
        if (m_source_type == data_source::file)
          {
            m_discrepancy_box->setDisabled (true);
            m_discrepancy_pb->setDisabled (true);
          }

        vlo_2->addWidget (m_discrepancy_pb, 0, Qt::AlignLeft);
        vlo_2->addWidget (m_discrepancy_box);
      }
      hlo_2->addLayout (vlo_2);
    }
    vlo_1->addLayout (hlo_2);
  }
  setLayout (vlo_1);
}

void main_window::open_greetings_window ()
{
  greet_window g_window (this);
  if (QDialog::Accepted != g_window.exec ())
    std::abort ();
  m_source_type = g_window.get_source_type ();
  QString path;
  switch (m_source_type)
    {
    case data_source::file:
      {
        path = g_window.get_path ();
        FILE *fin = fopen (path.toUtf8 ().data (), "r");
        if (!fin)
          {
            fprintf (stderr, "no such file\n");
            std::abort ();
          }
        unsigned int size;
        if (1 != fscanf (fin, "%d", &size))
          {
            fprintf (stderr, "wrong input format\n");
            std::abort ();
          }
        std::vector<double> xes, ys, additional;
        if (interpol::read_to_vector (fin, xes, size))
          std::abort ();
        if (interpol::read_to_vector (fin, ys, size))
          std::abort ();
        if (interpol::read_to_vector (fin, additional, size))
          std::abort ();
        fclose (fin);
        m_plot_model = new interpol_plot_model (xes, ys);
        m_plot_model->add_interpol (interpol::polynom_type::c_spline_w_derivs,
        {additional[0], additional[xes.size () - 1]});
        m_plot_model->add_interpol (interpol::polynom_type::newton_mult_nodes,
                                    additional);
        graph_painter *plot_painter = new graph_painter;
        plot_painter->set_model (m_plot_model);
        m_plot_drawer = new plot_widget (plot_painter, this);
        connect (m_plot_model, SIGNAL (model_changed ()), m_plot_drawer, SLOT (update ()));
      }
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

void main_window::on_discrepancy_pb_clicked ()
{
  m_visible_graphs_box->uncheck_all ();
  m_plot_model->change_discrepancy_graph (m_discrepancy_box->currentIndex ());
}
