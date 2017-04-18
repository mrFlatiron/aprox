#ifndef MAIN_WINDOW_V2_H
#define MAIN_WINDOW_V2_H

#include <QDialog>
#include "gui_enums.h"

class interpol_plot_model;
class plot_scene;

class QLineEdit;
class QSpinBox;
class QComboBox;
class QMenuBar;
class QAction;

class plot_widget;

class gui_checkbox_group;
class graph_painter;

class main_window_v2 : public QDialog
{
  Q_OBJECT
private:
  plot_widget *m_plot_drawer;
  graph_painter *m_graph_painter;
  interpol_plot_model *m_plot_model;
  QSpinBox *m_points_count_edit;
  QMenuBar *m_menu_bar;
  QAction *m_actions[(int)graph_mode::COUNT];
  graph_mode m_mode = graph_mode::origin;
public:

  main_window_v2 (const double x_min, const double x_max, const int points_count); //assume they are already validated
  ~main_window_v2 ();

  QSize sizeHint () const override;
private:
//  void open_greetings_window ();
  void set_layouts ();
  void do_connects ();
  QString static get_action_name (const graph_mode mode);
private slots:
  void on_menu_bar (QAction *action);
  void on_points_count_changed (int points_count);

};

#endif // MAIN_WINDOW_V2_H
