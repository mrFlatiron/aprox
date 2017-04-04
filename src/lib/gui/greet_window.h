#ifndef GREET_WINDOW_H
#define GREET_WINDOW_H

#include <QDialog>
#include <QString>
#include <QSize>
#include "gui_enums.h"
class QLineEdit;

class greet_window : public QDialog
{
  Q_OBJECT
private:
  QLineEdit *m_path_edit;
  data_source m_source_type;
public:
  greet_window (QWidget *parent = 0);
  ~greet_window ();
  data_source get_source_type () const;
  QString get_path () const;
  virtual QSize sizeHint () const override;
public slots:
  void on_file_pb_clicked ();
  void on_func_pb_clicked ();
};

#endif // GREET_WINDOW_H
