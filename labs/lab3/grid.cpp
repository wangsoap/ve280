#include "grid.h"
#include "const.h"
#include "point.h"
#include "tile.h"
#include <iostream>
#include <iomanip>

using namespace std;

static Point checkDst(Direction dir, Point src, const Grid &grid) {

    while (grid.insideGrid(adjacentPoint(src, dir)) && grid.isEmpty(adjacentPoint(src, dir))) {
        src = adjacentPoint(src, dir);
    }
    return src;
}

static Point sameTileFinder(Point src, Direction dir, const Grid &grid) {

    Point dst = adjacentPoint(checkDst(dir, src, grid),dir);

    if (grid.insideGrid(dst) && (grid.getSquare(src) != nullptr) && ((grid.getSquare(dst) == grid.getSquare(src)))) {
        return dst;
    } else {
        return src;
    }
}

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

    unsigned int score = 0;

    switch (dir) {
        case UP:
        case LEFT: {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    Point sameTile = sameTileFinder(Point{i, j}, dir, *this);
                    if (sameTile != Point{i, j}) {
                        upgradeTile(sameTile);
                        score += (2 * getSquare(Point{i, j})->points);
                        clearSquare(Point{i, j});
                    }

                }
            }
        }
            break;
        case DOWN:
        case RIGHT: {
            for (int i = height - 1; i >= 0; i--) {
                for (int j = width - 1; j >= 0; j--) {
                    Point sameTile = sameTileFinder(Point{i, j}, dir, *this);
                    if (sameTile != Point{i, j}) {
                        upgradeTile(sameTile);
                        score += (2 * getSquare(Point{i, j})->points);
                        clearSquare(Point{i, j});
                    }

                }
            }
        }
    }
    return score;
}

bool Grid::shiftTile(const Point &dst, const Point &src) {

    if (dst == src) {
        return false;
    }

    if (!isEmpty(src)) {
        setSquare(dst, getSquare(src));
        clearSquare(src);
        return true;
    }

    return false;
}

bool Grid::shiftTiles(Direction dir) {

    bool flag = false;

    switch (dir) {
        case UP:
        case LEFT: {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (shiftTile(checkDst(dir, Point{i, j}, *this), Point{i, j})) {
                        flag = true;
                    }
                }
            }
        }
            break;

        case DOWN:
        case RIGHT: {
            for (int i = height - 1; i >= 0; i--) {
                for (int j = width - 1; j >= 0; j--) {
                    if (shiftTile(checkDst(dir, Point{i, j}, *this), Point{i, j})) {
                        flag = true;
                    }
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
