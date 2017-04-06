#ifndef GREET_WINDOW_H
#define GREET_WINDOW_H

#include <QDialog>
#include <QString>
#include <QSize>
#include "gui_enums.h"
class gui_checkbox_w_id;
class QLineEdit;
class QHBoxLayout;

class greet_window : public QDialog
{
  Q_OBJECT
private:
  gui_checkbox_w_id *m_boxes[2];
  QLineEdit *m_path_edit;
  QLineEdit *m_min_edit;
  QLineEdit *m_max_edit;
  data_source m_source_type = data_source::function;
  double m_x_min;
  double m_x_max;
public:
  greet_window (QWidget *parent = 0);
  ~greet_window ();
  data_source get_source_type () const;
  QString get_path () const;
  virtual QSize sizeHint () const override;
  double get_min () const;
  double get_max () const;
public slots:
  void on_checkbox (int id, bool is_checked);
};

#endif // GREET_WINDOW_H
