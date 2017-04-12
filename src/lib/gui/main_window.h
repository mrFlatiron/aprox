#pragma once
#include <QDialog>

#include "gui_enums.h"

class interpol_plot_model;
class plot_scene;
class QLineEdit;
class QSpinBox;
class plot_widget;
class QComboBox;
class gui_checkbox_group;

class main_window : public QDialog
{
  Q_OBJECT
private:
  QSpinBox *m_points_count_edit;
  QPushButton *m_discrepancy_pb;
  QComboBox *m_discrepancy_box;
  plot_widget *m_plot_drawer;
  interpol_plot_model *m_plot_model;
  data_source m_source_type;
  gui_checkbox_group *m_visible_graphs_box;
public:
  main_window (QWidget *parent = 0);
  ~main_window ();

  virtual QSize sizeHint () const override;
signals:
  void points_count_changed (int val);
  void scale_changed (int val);
private:
  void set_layouts ();
  void open_greetings_window ();
private slots:
  void on_pc_slider_moved (int val);
  void on_discrepancy_pb_clicked ();
};
