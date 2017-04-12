#include "gui_checkbox_group.h"
#include "gui_checkbox_w_id.h"

#include <QVBoxLayout>

gui_checkbox_group::gui_checkbox_group (QWidget *parent) : QWidget (parent)
{

}

gui_checkbox_group::~gui_checkbox_group ()
{

}

gui_checkbox_group::gui_checkbox_group (const std::vector<QString> &labels, QWidget *parent)
  : QWidget (parent)
{
  for (int i = 0; i < (int)labels.size (); i++)
    {
      gui_checkbox_w_id *cb = new gui_checkbox_w_id (labels[i], i, this);
      connect (cb, SIGNAL (toggled (int, bool)), this, SLOT (on_toggled (int, bool)));
      m_boxes.push_back (cb);
      cb->setChecked (true);
    }

  m_layout = new QVBoxLayout ();
  {
    for (auto val : m_boxes)
      {
        m_layout->addWidget (val);
      }
  }
}

QLayout *gui_checkbox_group::as_layout () const
{
  return m_layout;
}

void gui_checkbox_group::uncheck_all ()
{
  for (auto pb : m_boxes)
    {
      if (pb->isChecked ())
        pb->toggle ();
    }
}

void gui_checkbox_group::on_toggled (int id, bool checked)
{
  emit box_toggled (id, checked);
}

