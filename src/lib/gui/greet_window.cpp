#include "greet_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleValidator>
#include "gui_checkbox_w_id.h"
#include <QDialogButtonBox>

greet_window::greet_window (QWidget *parent) : QDialog (parent)
{
  setWindowTitle ("Aprox Welcome");
  QVBoxLayout *vlo_main = new QVBoxLayout;
  {

    vlo_main->addWidget (new QLabel
                    ("Welcome to the Aprox!\nChoose the source for an interpolation:"), 0,
                         Qt::AlignLeft | Qt::AlignTop);

    m_boxes[0] = new gui_checkbox_w_id ("From file", 0, this);
    m_boxes[1] = new gui_checkbox_w_id ("From test_functions.h", 1, this);
    vlo_main->addWidget (m_boxes[0], 0, Qt::AlignLeft| Qt::AlignTop);

    m_path_edit = new QLineEdit ("/home/Vyacheslav/aprox/input/t1", this);
    vlo_main->addWidget (m_path_edit, 1);
    m_path_edit->hide ();

    connect (m_boxes[0], SIGNAL (toggled (int, bool)), this, SLOT (on_checkbox (int, bool)));
    connect (m_boxes[1], SIGNAL (toggled (int, bool)), this, SLOT (on_checkbox (int, bool)));

    vlo_main->addWidget (m_boxes[1]);
    QHBoxLayout *hlo_bounds = new QHBoxLayout;
    {
      m_min_edit = new QLineEdit ("-5", this);
      m_min_edit->setDisabled (true);
      m_max_edit = new QLineEdit ("10", this);
      m_max_edit->setDisabled (true);

      m_min_edit->setValidator (new QDoubleValidator (this));
      m_max_edit->setValidator (new QDoubleValidator (this));
      hlo_bounds->addWidget (new QLabel ("x_min:",this), 0, Qt::AlignLeft);
      hlo_bounds->addWidget (m_min_edit, 1);
      hlo_bounds->addWidget (new QLabel ("x_max:",this), 0, Qt::AlignLeft);
      hlo_bounds->addWidget (m_max_edit, 1);
    }
    vlo_main->addLayout (hlo_bounds);
    QDialogButtonBox *bb = new QDialogButtonBox (QDialogButtonBox::Ok
                                                 | QDialogButtonBox::Cancel,
                                                 Qt::Horizontal, this);

    vlo_main->addWidget (bb, 0, Qt::AlignRight | Qt::AlignBottom);

    connect(bb, SIGNAL(accepted()), this, SLOT(accept()));
    connect(bb, SIGNAL(rejected()), this, SLOT(reject()));
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

double greet_window::get_min () const
{
  return m_min_edit->text ().toDouble ();
}

double greet_window::get_max () const
{
  return m_max_edit->text ().toDouble ();
}

void greet_window::on_checkbox (int id, bool is_checked)
{
  switch (id)
    {
    case 0:
      if (is_checked)
        {
         m_boxes[1]->setChecked (false);
         m_min_edit->setDisabled (true);
         m_max_edit->setDisabled (true);
         m_path_edit->show ();
        }
      else
        {
          m_path_edit->hide ();
        }
      break;
    case 1:
      if (is_checked)
        {
          m_boxes[0]->setChecked (false);
          m_min_edit->setEnabled (true);
          m_max_edit->setEnabled (true);
          m_path_edit->hide ();
        }
      else
        {
          m_min_edit->setDisabled (true);
          m_max_edit->setDisabled (true);
        }
      break;
    }
}


