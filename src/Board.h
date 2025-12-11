#pragma once
#include <vector>
using namespace std;

struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int neighboringMines = 0;
};

class Board {
private:
    void placeMines();
    void floodFill(int r, int c);

public:
    int rows, cols, mines;
    vector<vector<Cell>> grid;

    Board(int r, int c, int m);

    void reset();
    bool inBounds(int r, int c) const;
    bool reveal(int r, int c);
    void toggleFlag(int r, int c);
    bool checkWin() const;
    void calculateNeighborMines();
    int countFlags() const;
};