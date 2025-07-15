#ifndef ABRIGOS_HPP
#define ABRIGOS_HPP

class Abrigos {
private:
    int R; // Raio de alcance
    int X; // Coordenada X
    int Y; // Coordenada Y

public:
    // Construtor
    Abrigos(int raio, int x, int y);
    Abrigos();

    // Getters
    int getRaio() const;
    int getX() const;
    int getY() const;

    // Setters
    void setRaio(int raio);
    void setX(int x);
    void setY(int y);

    // Verifica se um ponto (px, py) está dentro do raio
    bool contemPonto(int px, int py) const;
};

#endif
