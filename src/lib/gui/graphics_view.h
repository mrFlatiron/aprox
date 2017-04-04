#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H
#include <QGraphicsView>

class graphics_view : public QGraphicsView
{
  Q_OBJECT
private:
  QGraphicsScene *m_scene;
public:
  explicit graphics_view (QWidget *parent = 0);
  virtual ~graphics_view ();
  virtual QSize sizeHint () const override;
  void set_scale (const int scale);
signals:

public slots:
};
#endif // GRAPHICS_VIEW_H
