#include "graphics_view.h"

#include <QTransform>

graphics_view::graphics_view (QWidget *parent) : QGraphicsView (parent)
{
  this->setFrameRect (QRect(-100, 100, 100, 100));
  this->setTransform (QTransform (1, 0, 0,
                                  0, -1, 0,
                                  0, 0, 1));
}

graphics_view::~graphics_view ()
{

}

QSize graphics_view::sizeHint() const
{
  return QSize (1024, 780);
}

void graphics_view::set_scale_x (const int scale)
{
    QTransform old = transform();
    setTransform (abs (scale), old.m12(), old.m13(),
                  old.m21(), old.m22 (), old.m23 (),
                  old.m31 (), old.m32 (), old.m33 ());
}

void graphics_view::set_scale_y (const int scale)
{
    QTransform old = transform();
    setTransform (old.m11(), old.m12(), old.m13(),
                  old.m21(), -abs (scale), old.m23 (),
                  old.m31 (), old.m32 (), old.m33 ());
}

