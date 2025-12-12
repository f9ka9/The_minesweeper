#pragma once
#include "Graph_lib/Graph.h"
#include <FL/fl_ask.H>

struct MainWindow;

struct MyButton : public Graph_lib::Button {
    using Graph_lib::Button::Button;
    MainWindow* window_ptr {nullptr};
};

class FltkInterface {
public:
    //Методы для работы с событиями
    static int event_x() { return Fl::event_x(); }
    static int event_y() { return Fl::event_y(); }
    static int event_button() { return Fl::event_button(); }
    
    //Константы мыши
    static const int LEFT_MOUSE = FL_LEFT_MOUSE;
    static const int MIDDLE_MOUSE = FL_MIDDLE_MOUSE;
    static const int RIGHT_MOUSE = FL_RIGHT_MOUSE;
    
    //Константы событий
    static const int PUSH = FL_PUSH;
    static const int RELEASE = FL_RELEASE;
    static const int DRAG = FL_DRAG;
    static const int KEYDOWN = FL_KEYDOWN;
    static const int KEYUP = FL_KEYUP;
    static const int MOVE = FL_MOVE;
    static const int ENTER = FL_ENTER;
    static const int LEAVE = FL_LEAVE;
    static const int FOCUS = FL_FOCUS;
    static const int UNFOCUS = FL_UNFOCUS;
    static const int MOUSEWHEEL = FL_MOUSEWHEEL;
    
    //Удобные методы-проверки
    static bool isRightMouse() { return event_button() == RIGHT_MOUSE; }
    static bool isLeftMouse() { return event_button() == LEFT_MOUSE; }
    static bool isMiddleMouse() { return event_button() == MIDDLE_MOUSE; }

    //Диалоговые окна FLTK 
    static void showMessage(const char* message) { fl_message("%s", message); }

    static void setFixedSize(Fl_Window* window, int width, int height) {
        if (window) {
            window->size_range(width, height, width, height);
            window->resizable(nullptr);
        }
    }
};