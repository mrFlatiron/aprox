#include "plot_view.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTransform>

plot_view::plot_view (QWidget *parent) : QGraphicsView (parent)
{
  this->setFrameRect (QRect(-100, 100, 100, 100));
  this->setTransform (QTransform (1, 0, 0,
                                  0, -1, 0,
                                  0, 0, 1));
}

plot_view::~plot_view()
{

}
