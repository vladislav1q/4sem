#include <SFML/Graphics.hpp>
#include <iostream>
#include <numeric>
#include "math.h"
#include "Vector2.h"

int sizeCharge = 10;

vector<unsigned long> range(unsigned long first, unsigned long last)
{
    vector<unsigned long> range(last - first + 1);
    iota(range.begin(), range.end(), first);
    return range;
}

struct Charge {
    float value;
    vec::Vector2 position;
};

struct ElectricField {
    std::vector<Charge> charge;
    std::vector<int> stack;
    vec::Vector2 find(const Charge& c) const;
};

vec::Vector2 ElectricField::find(const Charge& q) const {
    vec::Vector2 result(0, 0);
    vec::Vector2 tmp(0, 0);
    float length_tmp;

    for (unsigned long i : range(0, charge.size())) {
        tmp = q.position - charge[i].position;
        length_tmp = tmp.length();
        result += tmp * (q.value * charge[i].value /(length_tmp * length_tmp * length_tmp));
    }

    return result;
}

void addCharge(sf::RenderWindow &window, ElectricField& E, int charge);
void checkEvents(sf::RenderWindow &window, ElectricField& E);
void drawWindowCharge(sf::RenderWindow &window, ElectricField& E, sf::Color color, std::vector<int, std::allocator<int>>::iterator p, unsigned long i);
void drawWindow(sf::RenderWindow &window, ElectricField& E, int numberOfLines, int maxLength);
