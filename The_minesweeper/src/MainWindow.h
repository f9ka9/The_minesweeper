#pragma once
#include "Graph_lib/GUI.h"
#include "Graph_lib/Graph.h"
#include "Graph_lib/Window.h"
#include "Board.h"
#include "Update_Graph_lib.h"
#include <vector>
#include <string>
using namespace Graph_lib;
using namespace std;

struct MainWindow : public Window {
public:
    MainWindow(Point xy, int w, int h, const string& title,
               int rows=10, int cols=10, int mines=10);
    ~MainWindow();
private:
    static constexpr int CELL_SIZE=30;

    int rows, cols, mines;
    Board board;
    int origin_x, origin_y;
    bool firstClick, gameOver;

    MyButton newGameButton;
    MyButton exitButton;

    Vector_ref<Rectangle> cellRects;
    Vector_ref<Text> cellTexts;
    Text* statusText;

    static void cb_new_game(Address, Address pw);
    static void cb_exit(Address, Address pw);
    void on_new_game();
    void on_exit();
    void drawBoard();
    void updateCell(int r, int c);
    void clickCellAtIndex(int r, int c, bool rightClick);
    bool coordsToIndex(int x, int y, int& out_r, int& out_c);
    int handle(int event) override;
};
