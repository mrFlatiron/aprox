#include "gui_checkbox_w_id.h"

gui_checkbox_w_id::gui_checkbox_w_id (QWidget *parent)
  :QCheckBox (parent)
{
  m_id = 0;
  do_connects ();
}

gui_checkbox_w_id::gui_checkbox_w_id (const QString &label, const int id, QWidget *parent)
  :QCheckBox (label, parent)
{
  m_id = id;
  do_connects ();
}

//gui_checkbox_w_id::gui_checkbox_w_id (const QCheckBox &box, const int id)
//  :QCheckBox (box)
//{
//  m_id = id;
//  do_connects ();
//}

void gui_checkbox_w_id::do_connects ()
{
  connect (this, SIGNAL (toggled (bool)), this, SLOT (forward_toggled (bool)));
}

void gui_checkbox_w_id::forward_toggled (bool checked)
{
  emit toggled (m_id, checked);
}

