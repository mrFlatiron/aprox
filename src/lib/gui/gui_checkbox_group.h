#ifndef GUI_CHECKBOX_GROUP_H
#define GUI_CHECKBOX_GROUP_H

#include <QWidget>
#include <vector>

class QCheckBox;
class QVBoxLayout;
class QString;
class gui_checkbox_w_id;

class gui_checkbox_group : public QWidget
{
  Q_OBJECT
private:
  std::vector<gui_checkbox_w_id *> m_boxes;
  QVBoxLayout *m_layout;
public:
  explicit gui_checkbox_group (QWidget *parent);
  ~gui_checkbox_group ();
  gui_checkbox_group (const std::vector<QString> &labels, QWidget *parent);
  QLayout *as_layout () const;
private slots:
  void on_toggled (int id, bool checked);
signals:
  void box_toggled (int id, bool state);
};

#endif // GUI_CHECKBOX_GROUP_H
