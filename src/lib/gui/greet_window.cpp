#include "greet_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

greet_window::greet_window (QWidget *parent) : QDialog (parent)
{
  setWindowTitle ("Aprox Welcome");
  QVBoxLayout *vlo_main = new QVBoxLayout;
  {
    const int pb_min_width = 105;

    vlo_main->addWidget (new QLabel
                    ("Welcome to the Aprox!\nChoose the source for an interpolation:"));

    QHBoxLayout *hlo_file = new QHBoxLayout;
    {
      QPushButton *file_pb = new QPushButton ("From this file ->", this);
      file_pb->setMinimumWidth (pb_min_width);
      m_path_edit = new QLineEdit ("/home/Vyacheslav/aprox/input/t1", this);
      connect (file_pb, SIGNAL (clicked(bool)), this, SLOT (on_file_pb_clicked ()));
      hlo_file->addWidget (file_pb);
      hlo_file->addWidget (m_path_edit, 1);
    }
    vlo_main->addLayout (hlo_file);

    QHBoxLayout *hlo_func = new QHBoxLayout;
    {
      QPushButton *func_pb = new QPushButton ("Function ->", this);
      func_pb->setMinimumWidth (pb_min_width);
      QLineEdit *func_file_line =
          new QLineEdit ("{project}/src/test_functions/test_functions.h::func_to_aprox",
                         this);
      func_file_line->setReadOnly (true);
      connect (func_pb, SIGNAL (clicked(bool)), this, SLOT (on_func_pb_clicked ()));
      hlo_func->addWidget (func_pb);
      hlo_func->addWidget (func_file_line);
    }
    vlo_main->addLayout (hlo_func);
  }
  setLayout (vlo_main);
}

greet_window::~greet_window ()
{

}

data_source greet_window::get_source_type () const
{
  return m_source_type;
}

QString greet_window::get_path () const
{
  return m_path_edit->text ();
}

QSize greet_window::sizeHint() const
{
  return QSize (540, 100);
}

void greet_window::on_file_pb_clicked ()
{
  m_source_type = data_source::file;
  accept ();
}

void greet_window::on_func_pb_clicked()
{
  m_source_type = data_source::function;
  accept();
}

