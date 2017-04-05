#ifndef GUI_CHECKBOX_W_ID_H
#define GUI_CHECKBOX_W_ID_H
#include <QCheckBox>
class QString;
class gui_checkbox_w_id : public QCheckBox
{
  Q_OBJECT
private:
  int m_id = 0;
public:
  explicit gui_checkbox_w_id (QWidget *parent);
  gui_checkbox_w_id (const QString &label, const int id, QWidget *parent);
//  gui_checkbox_w_id (const QCheckBox &box, const int id);
signals:
  void toggled (int id, bool checked);
private:
  void do_connects ();
private slots:
  void forward_toggled (bool checked);
};

#endif // GUI_CHECKBOX_W_ID_H
