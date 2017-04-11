#pragma once
#include <QDialog>

#include "gui_enums.h"

class interpol_plot_model;
class plot_scene;
class QLineEdit;
class QSpinBox;
class plot_widget;

class main_window : public QDialog
{
  Q_OBJECT
private:
  QSpinBox *m_points_count_edit;
  QSpinBox *m_scale_edit;
  plot_widget *m_plot_drawer;
  interpol_plot_model *m_plot_model;
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
};
