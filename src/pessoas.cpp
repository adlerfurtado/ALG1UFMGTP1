#include "pessoas.hpp"

Pessoas::Pessoas(int x, int y) {
    this->x = x;
    this->y = y;
}

int Pessoas::getX() const {
    return x;
}

int Pessoas::getY() const {
    return y;
}

void Pessoas::setX(int x) {
    this->x = x;
}

void Pessoas::setY(int y) {
    this->y = y;
}
