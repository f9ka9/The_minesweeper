#pragma once
#include "Graph_lib/Graph.h"

struct MainWindow;

struct MyButton : public Graph_lib::Button {
    using Graph_lib::Button::Button;
    MainWindow* window_ptr {nullptr};
};
