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
    //this->setSquare(pt,getSquare(pt)+1);
    //I know it is wrong but I don't know what's happening here even with the hint.
}

unsigned int Grid::collapseTiles(Direction dir) {
    unsigned int points=0;
    if (dir==UP || dir==DOWN){
        for (int i = 0; i < height; i++) {
            if (insideGrid(adjacentPoint({i, 0}, dir))) {
                for (int j = 0; j < width; j++) {
                    if (getSquare({i,j})==getSquare(adjacentPoint({i, j}, dir))) {
                        clearSquare({i,j});
                        upgradeTile(adjacentPoint({i, j}, dir));
                        points+=getSquare(adjacentPoint({i, j}, dir))->points;
                    }
                }
            }
        }
    }else {
        for (int j = 0; j < width; j++) {
            if (insideGrid(adjacentPoint({0, j}, dir))) {
                for (int i = 0; i < height; i++) {
                    if (getSquare({i,j})==getSquare(adjacentPoint({i, j}, dir))) {
                        clearSquare({i,j});
                        upgradeTile(adjacentPoint({i, j}, dir));
                        points+=getSquare(adjacentPoint({i, j}, dir))->points;
                    }
                }
            }
        }
    }
    return points;
}
bool Grid::shiftTile(const Point &dst, const Point &src) {
    if (!isEmpty(src) && isEmpty(dst)) {
        setSquare(dst,getSquare(src));
        setSquare(src, nullptr);
        return true;
    }
    return false;
}

bool Grid::shiftTiles(Direction dir) {
    bool change= false,all= false;
    while (!change) {
        change=true;
        if (dir == UP || dir == DOWN) {
            for (int i = 0; i < height; i++) {
                if (insideGrid(adjacentPoint({i, 0}, dir))) {
                    for (int j = 0; j < width; j++) {
                        while (shiftTile(adjacentPoint({i, j}, dir), {i, j})) {
                            all = true;
                            change= false;
                        }
                    }
                }
            }
        } else {
            for (int j = 0; j < width; j++) {
                if (insideGrid(adjacentPoint({0, j}, dir))) {
                    for (int i = 0; i < height; i++) {
                        while (shiftTile(adjacentPoint({i, j}, dir), {i, j})) {
                            change= false;
                            all = true;
                        }
                    }
                }
            }
        }
    }
    return all;
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
