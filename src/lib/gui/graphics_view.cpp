#include "graphics_view.h"

#include <QTransform>

graphics_view::graphics_view (QWidget *parent) : QGraphicsView (parent)
{
//  this->setFrameRect (QRect(-100, 100, 100, 100));
  this->setTransform (QTransform (1, 0, 0,
                                  0, -1, 0,
                                  0, 0, 1));
}

graphics_view::~graphics_view ()
{

}

QSize graphics_view::sizeHint() const
{
  return QSize (1000, 1000);
}
