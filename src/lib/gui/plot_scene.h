#ifndef PLOT_SCENE_H
#define PLOT_SCENE_H

#include <QGraphicsScene>
#include <map>
#include <memory>

#include "interpol/interpol_factory.h"

class plot_scene : public QGraphicsScene
{
  Q_OBJECT
private:
  const double EPS = 1e-15;
  int m_scale;
  int m_pivot_points;
  int m_smooth = 5;
  double m_x_min;
  double m_x_max;
  double m_y_min;
  double m_y_max;
  int m_points_count;
  std::function<double(const double)> m_origin;
  int m_interpols_count;
  std::map<interpol::polynom_type, std::unique_ptr<interpol::polynom>> m_interpols;
  std::map<interpol::polynom_type, std::vector<double>> m_additionals;
public:
  plot_scene (QObject *parent = 0);
  plot_scene (const double a, const double b,
              std::function<double (const double)> origin,
              QObject *parent = 0);
  virtual ~plot_scene ();

  int get_points_count () const;

  void set_origin_func (const std::function <double(const double)> &origin);

  void set_scale (const int scale);

  int get_scale () const;

  void set_smooth (const int smooth);

  void set_points_count (const int points_count);

  void set_bounds (const double a, const double b);

  void add_interpol (const interpol::polynom_type type,
                     const std::vector<double> &additional);

  QPointF get_center () const;

public slots:
  void on_points_count_changed (int);
  void on_scale_changed (int);
  void on_visible_changed (std::vector<interpol::polynom_type>) {}

private:
  void calculate_pivot_poins ();
  Qt::GlobalColor get_color (const interpol::polynom_type type);
  void try_set_y_bounds (const double y_min, const double y_max);
  void redraw_all ();
  void draw_interpol (const interpol::polynom_type type);
  void draw_origin ();
  void draw_grid ();
  template <class T>
    void draw_function (T *func, const Qt::GlobalColor color);

};

template <class T>
void plot_scene::draw_function (T *func, const Qt::GlobalColor color)
{

  QPointF first;
  QPointF second;
  QPen pen;

  pen.setStyle (Qt::SolidLine);
  pen.setWidth (3);
  pen.setBrush (QBrush ((QColor (color))));
  pen.setCapStyle (Qt::RoundCap);
  pen.setJoinStyle (Qt::RoundJoin);
  double x = m_x_min;
  double y = func->operator ()(x);

  double y_min = y;
  double y_max = y;

  first.setX (x * m_scale);
  first.setY (y * m_scale);
  double hx = (m_x_max - m_x_min)/(m_pivot_points - 1);
  for (int i = 1; i < m_pivot_points; i++)
    {
      x = m_x_min + hx * i;
      y = func->operator()(x);
      if (y_min > y)
        y_min = y;
      if (y_max < y)
        y_max = y;
      second.setX (x * m_scale);
      second.setY (y * m_scale);
      addLine (first.x (), first.y (), second.x (), second.y (), pen);
      first = second;
    }
  try_set_y_bounds (y_min, y_max);
}

#endif // PLOT_SCENE_H
