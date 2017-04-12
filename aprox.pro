QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += с++11
CONFIG += debug

SOURCES += \
    src/lib/gui/main_window.cpp \
    src/lib/interpol/interpol_c_splines_w_derivs.cpp \
    src/lib/interpol/interpol_newton_mult_nodes.cpp \
    src/lib/interpol/interpol_polynom.cpp \
    src/lib/interpol/interpol_tools.cpp \
    src/lib/matrix/matrix_simple.cpp \
    src/lib/matrix/matrix_sq.cpp \
    src/lib/matrix/matrix_tri_diag.cpp \
    src/tests/window_test.cpp \
    src/lib/gui/greet_window.cpp \
    src/test_functions/test_functions.cpp \
    src/lib/gui/abstract_plot_model.cpp \
    src/lib/gui/interpol_plot_model.cpp \
    src/lib/gui/stretch.cpp \
    src/lib/gui/gui_checkbox_group.cpp \
    src/lib/gui/gui_checkbox_w_id.cpp \
    src/lib/gui/graph_painter.cpp \
    src/lib/gui/plot_widget.cpp

HEADERS += \
    src/lib/gui/main_window.h \
    src/lib/interpol/interpol_c_splines_w_derivs.h \
    src/lib/interpol/interpol_factory.h \
    src/lib/interpol/interpol_newton_mult_nodes.h \
    src/lib/interpol/interpol_polynom.h \
    src/lib/interpol/interpol_tools.h \
    src/lib/matrix/matrix_simple.h \
    src/lib/matrix/matrix_sq.h \
    src/lib/matrix/matrix_tri_diag.h \
    src/lib/gui/greet_window.h \
    src/lib/gui/gui_enums.h \
    src/test_functions/test_functions.h \
    src/lib/gui/abstract_plot_model.h \
    src/lib/gui/abstract_plot_defs.h \
    src/lib/gui/interpol_plot_model.h \
    src/lib/gui/stretch.h \
    src/lib/gui/gui_checkbox_group.h \
    src/lib/gui/gui_checkbox_w_id.h \
    src/lib/gui/graph_painter.h \
    src/lib/gui/plot_widget.h

INCLUDEPATH += src/lib
INCLUDEPATH += src
