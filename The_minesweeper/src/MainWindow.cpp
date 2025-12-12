#include "MainWindow.h"
#include <sstream>
#include <iostream>
using namespace std;

// ------------------- Конструктор -------------------
MainWindow::MainWindow(Point xy, int w, int h, const string& title,
                       int rows, int cols, int mines)
    : Window(xy, w, h, title),
      rows(rows), cols(cols), mines(mines),
      board(rows, cols, mines),
      newGameButton(Point(20, 10), 100, 30, "Новая игра", cb_new_game),
      exitButton(Point(140, 10), 80, 30, "Выход", cb_exit),
      firstClick(true), gameOver(false)
{
    newGameButton.window_ptr = this;
    exitButton.window_ptr = this;

    attach(newGameButton);
    attach(exitButton);

    origin_x = 20;
    origin_y = 60;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int x = origin_x + c * CELL_SIZE;
            int y = origin_y + r * CELL_SIZE;
            Rectangle* rect = new Rectangle(Point(x, y), CELL_SIZE, CELL_SIZE);
            rect->set_color(Color::black);
            rect->set_fill_color(Color::green);
            cellRects.push_back(rect);
            attach(*rect);
            

            Text* t = new Text(Point(x + CELL_SIZE/3, y + CELL_SIZE*2/3), "");
            t->set_font_size(14);
            t->set_color(Color::black);
            cellTexts.push_back(t);
            attach(*t);
        }
    }

    statusText = new Text(Point(origin_x + 220, origin_y - 25),
                          "Мины: " + to_string(mines));
    statusText->set_font_size(14);
    attach(*statusText);

    board.reset();
    firstClick = true;
    gameOver = false;
    drawBoard();
    FltkInterface::setFixedSize(this, w, h);    
}
MainWindow::~MainWindow(){
    statusText = nullptr;
}

// ------------------- Колбэки кнопок -------------------
void MainWindow::cb_new_game(Address, Address pw) {
    reference_to<MyButton>(pw).window_ptr->on_new_game();
}

void MainWindow::cb_exit(Address, Address pw) {
    reference_to<MyButton>(pw).window_ptr->on_exit();
}

// ------------------- Действия кнопок -------------------
void MainWindow::on_new_game() {
    board.reset();
    firstClick = true;
    gameOver = false;

    // Обновляем все клетки
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int index = r * cols + c;
            
            // Проверяем границы (на всякий случай)
            if (index < cellRects.size()) {
                cellRects[index].set_fill_color(Color::darker_gray);
            }
            
            if (index < cellTexts.size()) {
                cellTexts[index].set_label("");
            }
        }
    }

    // Обновляем статус
    if (statusText) {
        statusText->set_label("Мины: " + to_string(mines));
    }
    
    redraw();
}
void MainWindow::on_exit() {
    hide();
}

// ------------------- Визуализация -------------------
void MainWindow::drawBoard() {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            updateCell(r, c);
    redraw();
}

void MainWindow::updateCell(int r, int c) {
    int index = r * cols + c;
    if (index < 0 || index >= cellRects.size()) return;
    
    
    Cell& cell = board.grid[r][c];
    Rectangle& rect = cellRects[index];
    Text& text = cellTexts[index];

    if (cell.isRevealed) {
        rect.set_fill_color(Color::white);
        if (cell.isMine) {
            text.set_label("*");
            text.set_color(Color::red);
        } else if (cell.neighboringMines > 0) {
            ostringstream oss;
            oss << cell.neighboringMines;
            text.set_label(oss.str());
            switch(cell.neighboringMines) {
                case 1: text.set_color(Color::blue); break;
                case 2: text.set_color(Color::green); break;
                case 3: text.set_color(Color::red); break;
                case 4: text.set_color(Color::magenta); break;
                case 5: text.set_color(Color::dark_red); break;
                case 6: text.set_color(Color::dark_cyan); break;
                case 7: text.set_color(Color::black); break;
                case 8: text.set_color(Color::dark_yellow); break;
            }
        } else text.set_label("");
    } else {
        rect.set_fill_color(cell.isFlagged ? Color::red : Color::darker_gray);
        text.set_label("");
    }
}

// ------------------- Логика клика -------------------
void MainWindow::clickCellAtIndex(int r, int c, bool rightClick) {
    if (gameOver) return;
    if (!board.inBounds(r,c)) return;

    Cell& cell = board.grid[r][c];

    if (rightClick) {
        if (!cell.isRevealed) {
            board.toggleFlag(r,c);
            int flags = board.countFlags();
            int remaining = mines - flags;
            statusText->set_label("Мины: " + to_string(remaining));
        }
        drawBoard();
        return;
    }

    if (firstClick) {
        do { board.reset(); } while(board.grid[r][c].isMine);
        firstClick = false;
    }

    if (cell.isFlagged || cell.isRevealed) return;

    bool exploded = board.reveal(r,c);

    if (exploded) {
        for (int rr=0; rr<rows; rr++)
            for (int cc=0; cc<cols; cc++)
                if (board.grid[rr][cc].isMine)
                    board.grid[rr][cc].isRevealed = true;

        gameOver = true;
        statusText->set_label("Статус: Проигрыш");
        drawBoard();
        FltkInterface::showMessage("Вы проиграли!");
        return;
    }

    if (board.checkWin()) {
        gameOver = true;
        statusText->set_label("Статус: Победа");
        drawBoard();
        FltkInterface::showMessage("Вы выиграли!");
        return;
    }

    drawBoard();
}

// ------------------- Конвертация координат -------------------
bool MainWindow::coordsToIndex(int x, int y, int& out_r, int& out_c) {
    if (x < origin_x || y < origin_y) return false;
    out_c = (x - origin_x) / CELL_SIZE;
    out_r = (y - origin_y) / CELL_SIZE;
    return board.inBounds(out_r, out_c);
}

// ------------------- Обработка событий -------------------
int MainWindow::handle(int event) {
    switch(event) {
        case FltkInterface::PUSH: {  
            int x = FltkInterface::event_x();    
            int y = FltkInterface::event_y();      
            int button = FltkInterface::event_button(); 
            bool right = FltkInterface::isRightMouse(); 
            int r,c;
            if (coordsToIndex(x,y,r,c)) {
                clickCellAtIndex(r,c,right);
                return 1;
            }
            return Window::handle(event);
        }
        default:
            return Window::handle(event);
    }
}