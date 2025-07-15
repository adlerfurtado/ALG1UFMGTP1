#ifndef PESSOAs_HPP
#define PESSOAs_HPP

class Pessoas {
private:
    int x;
    int y;

public:
    // Construtor
    Pessoas(int x, int y);

    // Getters
    int getX() const;
    int getY() const;

    // Setters
    void setX(int x);
    void setY(int y);
};

#endif
