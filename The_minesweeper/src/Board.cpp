#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board(int r, int c, int m)
    : rows(r), cols(c), mines(m), grid(r, vector<Cell>(c))
{
    reset();
}

void Board::placeMines() {
    int placed = 0;
    while (placed < mines) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (!grid[r][c].isMine) {
            grid[r][c].isMine = true;
            placed++;
        }
    }
}

void Board::floodFill(int r, int c) {
    static int dr[8] = {-1,-1,-1,0,0,1,1,1};
    static int dc[8] = {-1,0,1,-1,1,-1,0,1};

    for (int i = 0; i < 8; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (inBounds(nr,nc)) {
            Cell& next = grid[nr][nc];
            if (!next.isRevealed && !next.isMine) {
                next.isRevealed = true;
                if (next.neighboringMines == 0)
                    floodFill(nr, nc);
            }
        }
    }
}

void Board::reset() {
    for (auto& row : grid)
        for (auto& cell : row)
            cell = Cell();
    placeMines();
    calculateNeighborMines();
}

bool Board::inBounds(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

bool Board::reveal(int r, int c) {
    if (!inBounds(r,c)) return false;
    Cell& cell = grid[r][c];
    if (cell.isRevealed || cell.isFlagged) return false;

    cell.isRevealed = true;
    if (cell.isMine) return true;
    if (cell.neighboringMines == 0) floodFill(r,c);
    return false;
}

void Board::toggleFlag(int r, int c) {
    if (!inBounds(r,c)) return;
    Cell& cell = grid[r][c];
    if (!cell.isRevealed) cell.isFlagged = !cell.isFlagged;
}

bool Board::checkWin() const {
    for (int r=0; r<rows; r++)
        for (int c=0; c<cols; c++)
            if (!grid[r][c].isMine && !grid[r][c].isRevealed)
                return false;
    return true;
}

void Board::calculateNeighborMines() {
    static int dr[8] = {-1,-1,-1,0,0,1,1,1};
    static int dc[8] = {-1,0,1,-1,1,-1,0,1};

    for (int r=0; r<rows; r++) {
        for (int c=0; c<cols; c++) {
            if (grid[r][c].isMine) { grid[r][c].neighboringMines=-1; continue; }
            int count=0;
            for (int i=0; i<8; i++) {
                int nr=r+dr[i], nc=c+dc[i];
                if (inBounds(nr,nc) && grid[nr][nc].isMine) count++;
            }
            grid[r][c].neighboringMines=count;
        }
    }
}

int Board::countFlags() const {
    int cnt = 0;
    for (auto& row : grid)
        for (auto& cell : row)
            if (cell.isFlagged) cnt++;
    return cnt;
}
