#include <SFML/Graphics.hpp>
#include <iostream>
#include <numeric>
#include "math.h"
#include "Vector2.h"
#include "electric_field.h"


using namespace sf;

void addCharge(RenderWindow &window, ElectricField& E, int charge){
    Charge q;
    q.value = charge;
    q.position = vec::Vector2(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
    E.charge.push_back(q);
}

void checkEvents(RenderWindow &window, ElectricField& E){
    Event event;
    while (window.pollEvent(event)){
        if(event.type == Event::Closed){
            window.close();
            break;
        } else if(event.type == Event::MouseButtonReleased){
            if (event.mouseButton.button == Mouse::Left){
                addCharge(window, E, 1);
            } else if (event.mouseButton.button == Mouse::Right){
                addCharge(window, E, -1);
            }
        } else if(event.type == Event::KeyReleased && event.key.code == Keyboard::Space) {
            addCharge(window, E, 1);
            addCharge(window, E, -1);

            E.stack.push_back(E.charge.size());
            E.stack.push_back(E.charge.size() - 1);
        }
    }
}

void drawWindowCharge(RenderWindow &window, ElectricField& E, sf::Color color, std::vector<int, std::allocator<int>>::iterator p, unsigned long i){
    if (p != E.stack.end()){
        CircleShape circle(sizeCharge);
        circle.setPosition(E.charge[i].position.x-sizeCharge, E.charge[i].position.y-sizeCharge);
        circle.setFillColor(Color::White);
        window.draw(circle);
    } else{
        CircleShape circle(sizeCharge);
        circle.setPosition(E.charge[i].position.x-sizeCharge, E.charge[i].position.y-sizeCharge);
        circle.setFillColor(color);
        window.draw(circle);
    }
}

void drawWindow(RenderWindow &window, ElectricField& E, int numberOfLines, int maxLength){
    for (unsigned long i = 0; i < E.charge.size(); ++i)
    {
        Charge charges[numberOfLines];
        for (int k = 0; k < numberOfLines; ++k){
            charges[k] = E.charge[i];
            charges[k].position += vec::Vector2(2 * cos(k*2*M_PI / numberOfLines),  sin(k*2*M_PI / numberOfLines));
        }

        for (int k = 0; k < maxLength; ++k){
            vec::Vector2 tmp[numberOfLines];

            for (int j = 0; j < numberOfLines; ++j) {
                tmp[j] = E.find(charges[j]).norm();

                Vertex line[] = {Vertex(Vector2f(charges[j].position.x, charges[j].position.y)),
                                Vertex(Vector2f(charges[j].position.x + tmp[j].x, charges[j].position.y + tmp[j].y))};

                charges[j].position += tmp[j];
                window.draw(line, 2, Lines);
            }
        }

        auto p = std::find(E.stack.begin(), E.stack.end(), i);

        if (E.charge[i].value > 0)
            drawWindowCharge(window, E, sf::Color::Red, p, i);
        else if (E.charge[i].value < 0)
            drawWindowCharge(window, E, sf::Color::Blue, p, i);
    }
}
