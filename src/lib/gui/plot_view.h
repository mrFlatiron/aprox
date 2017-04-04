#ifndef PLOT_VIEW_H
#define PLOT_VIEW_H

#include <QWidget>
//#include <QGraphicsScene>
#include <QGraphicsView>

class QGraphicsScene;


class plot_view : public QGraphicsView
{
  Q_OBJECT
private:
  QGraphicsScene *m_scene;
public:
  explicit plot_view (QWidget *parent = 0);
  virtual ~plot_view ();

signals:

public slots:
};

#endif // PLOT_VIEW_H
