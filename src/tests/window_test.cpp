#include "gui/main_window_v2.h"
#include <QApplication>

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  double a, b;
  int pc;
  if (argc < 3)
    {
      printf ("Usage: %s x_min x_max [points count]\n", argv[0]);
      return 0;
    }
  a = atof (argv[1]);
  b = atof (argv[2]);
  if (argc >= 4)
    pc = atoi (argv[3]);
  else
    pc = 2;
  if (pc < 2)
    {
      printf ("points count must be integer >= 2\n");
      return 0;
    }
  if (fabs (a - b) < 1e-16)
    {
      printf ("|x_max - x_min| must be > 0\n");
      return 0;
    }
  main_window_v2 *window = new main_window_v2 (a, b, 2);
  window->exec ();
  delete window;
  return 0;
}
