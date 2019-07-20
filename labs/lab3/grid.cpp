#include "grid.h"
#include "const.h"
#include "point.h"
#include "tile.h"
#include <iostream>
#include <iomanip>

using namespace std;

Grid::Grid(unsigned int height, unsigned int width) : height(height), width(width), squares() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            squares[i][j] = nullptr;
        }
    }
}

unsigned int Grid::getHeight() const {
    return height;
}

unsigned int Grid::getWidth() const {
    return width;
}

bool Grid::insideGrid(const Point &pt) const {
    return pt.r >= 0 && pt.r < height && pt.c >= 0 && pt.c < width;
}

Tile *Grid::getSquare(const Point &pt) const {
    return squares[pt.r][pt.c];
}

void Grid::setSquare(const Point &pt, Tile *tile) {
    squares[pt.r][pt.c] = tile;
}

void Grid::clearSquare(const Point &pt) {
    setSquare(pt, nullptr);
}

void Grid::clearSquares() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            clearSquare({i, j});
        }
    }
}

bool Grid::isEmpty(const Point &pt) const {
    return getSquare(pt) == nullptr;
}

void Grid::upgradeTile(const Point &pt) {
    squares[pt.r][pt.c]++;
}

unsigned int Grid::collapseTiles(Direction dir) {
    Point dst;
    unsigned int score = 0;
    switch (dir) {
        case UP:
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    dst = {i, j};
                    if (!isEmpty(dst)) {
                        findPos(dst, dir);
                        //cout << i << ' ' <<  j << ' ' << dst.r << ' ' << dst.c << endl;
                        dst = adjacentPoint(dst, dir);
                        if (insideGrid(dst) && getSquare(dst) != nullptr && getSquare(dst)->points == getSquare({i, j})->points) {
                            //cout << i << ' ' << j << endl;
                            clearSquare(dst);
                            upgradeTile({i, j});
                            score += getSquare({i, j})->points;
                            //printGrid();
                        }
                    }
                }
            }
            break;
        case DOWN:
            for (int i = height - 1; i >= 0; i--) {
                for (int j = 0; j < width; j++) {
                    dst = {i, j};
                    if (!isEmpty(dst)) {
                        findPos(dst, dir);
                        dst = adjacentPoint(dst, dir);
                        if (insideGrid(dst) && getSquare(dst) != nullptr && getSquare(dst)->points == getSquare({i, j})->points) {
                            clearSquare(dst);
                            upgradeTile({i, j});
                            score += getSquare({i, j})->points;
                        }
                    }
                }
            }
            break;
        case LEFT:
            for (int i = height - 1; i >= 0; i--) {
                for (int j = 0; j < width; j++) {
                    dst = {i, j};
                    if (!isEmpty(dst)) {
                        findPos(dst, dir);
                        dst = adjacentPoint(dst, dir);
                        if (insideGrid(dst) && getSquare(dst) != nullptr && getSquare(dst)->points == getSquare({i, j})->points) {
                            clearSquare(dst);
                            upgradeTile({i, j});
                            score += getSquare({i, j})->points;
                        }
                    }
                }
            }
            break;
        case RIGHT:
            for (int i = height - 1; i >= 0; i--) {
                for (int j = width - 1; j >= 0; j--) {
                    dst = {i, j};
                    if (!isEmpty(dst)) {
                        findPos(dst, dir);
                        dst = adjacentPoint(dst, dir);
                        if (insideGrid(dst) && getSquare(dst) != nullptr && getSquare(dst)->points == getSquare({i, j})->points) {
                            clearSquare(dst);
                            upgradeTile({i, j});
                            score += getSquare({i, j})->points;
                        }
                    }
                }
            }
            break;
    }
    return score;
    
}

bool Grid::shiftTile(const Point &dst, const Point &src) {
    if (!isEmpty(dst) || isEmpty(src)) return false;
    setSquare(dst, getSquare(src));
    clearSquare(src);
    return true;
}

bool Grid::findPos(Point &dst, Direction dir) const {
    Point adjPt = adjacentPoint(dst, dir);
    if (!insideGrid(adjPt)) return false;
    if (getSquare(adjPt) != nullptr) return false;
    dst = adjPt;
    findPos(dst,dir);
    return true;
}

bool Grid::shiftTiles(Direction dir) {
    Point dst;
    bool flag = false;
    switch (dir) {
        case UP:
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    dst = {i, j};
                    if (!isEmpty(dst) && findPos(dst, dir)) {
                        shiftTile(dst, {i, j});
                        flag = true;
                    }
                }
            }
            break;
        case DOWN:
            for (int i = height - 1; i >= 0; i--) {
                for (int j = 0; j < width; j++) {
                    dst = {i, j};
                    if (!isEmpty(dst) && findPos(dst, dir)) {
                        shiftTile(dst, {i, j});
                        flag = true;
                    }
                }
            }
            break;
        case LEFT:
            for (int i = height - 1; i >= 0; i--) {
                for (int j = 0; j < width; j++) {
                    dst = {i, j};
                    if (!isEmpty(dst) && findPos(dst, dir)) {
                        shiftTile(dst, {i, j});
                        flag = true;
                    }
                }
            }
            break;
        case RIGHT:
            for (int i = height - 1; i >= 0; i--) {
                for (int j = width - 1; j >= 0; j--) {
                    dst = {i, j};
                    if (!isEmpty(dst) && findPos(dst, dir)) {
                        shiftTile(dst, {i, j});
                        flag = true;
                    }
                }
            }
            break;
    }
    return flag;
}

void Grid::printGrid() const {
    for (int i = 0; i < height; i++) {
        wcout << "|";
        for (int j = 0; j < width; j++) {
            if (isEmpty({i, j})) {
                wcout << setw(TILEWIDTH) << " " << "\t";
            } else {
                squares[i][j]->printTile();
            }
            wcout << "|";
        }
        wcout << endl << endl;
    }
}
