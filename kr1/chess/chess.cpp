#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

class Cell {
public:
    char v; // Вертикаль, от 'A' до 'H'
    unsigned short int h; // Горизонталь, от '1' до '8'
    Cell(char v, unsigned short int h) : v(v), h(h) { } // Конструктор заданной клетки поля с параметрами
    Cell() : Cell('A', 1) { } // Конструктор клетки по умолчанию
};
class Piece {
public:
    Cell _c;
    explicit Piece(Cell c) {
        _c = c;
    }
    virtual bool available(Cell c) const = 0;
};
class Rook : public Piece {
public:
    explicit Rook(Cell c) : Piece(c) { }
    bool available(Cell c) const override {
        return c.h == _c.h || c.v == _c.v;
    }
};
class King : public Piece {
public:
    explicit King(Cell c) : Piece(c) { }
    bool available(Cell c) const override {
        int dx = c.v - _c.v;
        int dy = c.h - _c.h;
        return dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1;
    }
};
class Queen : public Piece {
public:
    explicit Queen(Cell c) : Piece(c) { }
    bool available(Cell c) const override {
        int dx = c.v - _c.v;
        int dy = c.h - _c.h;
        return c.h == _c.h || c.v == _c.v || dx == dy || dx == -dy;
    }
};
class Bishop : public Piece {
public:
    explicit Bishop(Cell c) : Piece(c) { }
    bool available(Cell c) const override {
        int dx = c.v - _c.v;
        int dy = c.h - _c.h;
        return dx == dy || dx == -dy;
    }
};



int main() {
    std::vector<Piece *> pieces;
    pieces.push_back(new King(Cell('A', 1)));
    pieces.push_back(new Queen(Cell('B', 2)));
    pieces.push_back(new Rook(Cell('C', 3)));
    pieces.push_back(new Bishop(Cell('D', 4)));


    for (auto p: pieces)
        delete p;

    return 0;
}
