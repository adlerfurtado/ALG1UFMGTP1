#include "abrigos.hpp"

Abrigos::Abrigos(int raio, int x, int y) {
    R = raio;
    X = x;
    Y = y;
}

Abrigos::Abrigos() {
    R = 0;
    X = 0;
    Y = 0;
}

int Abrigos::getRaio() const {
    return R;
}

int Abrigos::getX() const {
    return X;
}

int Abrigos::getY() const {
    return Y;
}

void Abrigos::setRaio(int raio) {
    R = raio;
}

void Abrigos::setX(int x) {
    X = x;
}

void Abrigos::setY(int y) {
    Y = y;
}

bool Abrigos::contemPonto(int px, int py) const {
    int dx = px - X;
    int dy = py - Y;
    int distancia2 = dx * dx + dy * dy;
    return distancia2 <= R * R;
}
