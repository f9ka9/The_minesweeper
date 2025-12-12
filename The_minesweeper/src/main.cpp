
#include "MainWindow.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

int main()
try {
    srand((unsigned)time(nullptr));
    
    int rows=25, cols=25, mines=99;
    int width = 20 + cols * 30 + 20;
    int height = 60 + rows * 30 + 20;

    MainWindow win {Point(100,100), width, height, "Сапер", rows, cols, mines};
    return gui_main();
}
catch (exception& e) {
    cerr << e.what() << endl;
    return 1;
}
catch (...) {
    cerr << "Что-то пошло не так..." << endl;
    return 2;
}
