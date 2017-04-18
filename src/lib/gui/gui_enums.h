#ifndef GUI_ENUMS_H
#define GUI_ENUMS_H

enum class data_source
{
  file,
  function
};

enum class graph_mode
{
  origin,
  newton_mult_nodes,
  discrepancy_newton,
  c_splines_w_derivs,
  discrepancy_splines,
  COUNT
};

#endif // GUI_ENUMS_H
