#ifndef STRETCH_H
#define STRETCH_H
#include <QLabel>

class stretch : public QLabel
{
  Q_OBJECT
public:
  stretch (QWidget *parent);
  ~stretch ();
};

#endif // STRETCH_H
