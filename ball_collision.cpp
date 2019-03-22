#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "Vector2.h"

const int speed = 400;

using namespace sf;

class MaterialPoint{
public:
    vec::Vector2 GetImpulse() const{
        return vec::Vector2(velocity.x * mass, velocity.y * mass);
    }
    vec::Vector2 velocity;
    vec::Vector2 acceleration;
    vec::Vector2 initialPosition;

    float mass;
    float timeOfCreation;
};

class Ball: public MaterialPoint{
public:
    float radius;
    int type;
    CircleShape circle;
};

void setBall(Ball &ball, RenderWindow &window, Time &time){
    CircleShape circle(ball.mass*10);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    ball.circle = circle;
    ball.circle.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
    ball.initialPosition.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
    ball.circle.setFillColor(Color::Red);
    ball.velocity = vec::Vector2(speed, speed*(-1));
    ball.timeOfCreation = time.asSeconds();
}

bool isInsideOfBorder(Ball ball, RenderWindow &window, int sideOfBorder){    //  1
    switch (sideOfBorder){                                                  //  4 2
        case 0:{                                                            //   3
            if(isInsideOfBorder(ball, window, 1) && isInsideOfBorder(ball, window, 2) && isInsideOfBorder(ball, window, 3) && isInsideOfBorder(ball, window, 4))
                return true;
            else
                return false;
        }
        case 1:{
            if(ball.circle.getPosition().y - ball.circle.getRadius() > 0)
                return true;
            else
                return false;
        }
        case 2:{
            if(ball.circle.getPosition().x + ball.circle.getRadius() < window.getSize().x)
                return true;
            else
                return false;
        }
        case 3:{
            if(ball.circle.getPosition().y + ball.circle.getRadius() < window.getSize().y)
                return true;
            else
                return false;
        }
        case 4:{
            if(ball.circle.getPosition().x - ball.circle.getRadius() > 0)
                return true;
            else
                return false;
        }
        default:
            return false;
    }
}

void reflectWall(Ball &ball, RenderWindow &window, Time time){
    if(!isInsideOfBorder(ball, window, 1) || !isInsideOfBorder(ball, window, 3)) {
        ball.velocity.y *= -1;
        ball.initialPosition = vec::Vector2(ball.circle.getPosition().x, ball.circle.getPosition().y);
        ball.timeOfCreation = time.asSeconds();
    }
    if(!isInsideOfBorder(ball, window, 2) || !isInsideOfBorder(ball, window, 4)){
        ball.velocity.x *= -1;
        ball.initialPosition = vec::Vector2(ball.circle.getPosition().x, ball.circle.getPosition().y);
        ball.timeOfCreation = time.asSeconds();
    }
}

vec::Vector2 getdP(Ball ball_1, Ball ball_2) {
    vec::Vector2 d = vec::Vector2(ball_1.circle.getPosition().x-ball_2.circle.getPosition().x,
                                  ball_1.circle.getPosition().y-ball_2.circle.getPosition().y);

    if((d.length() <= ball_1.circle.getRadius() + ball_2.circle.getRadius()) && ((ball_1.velocity - ball_2.velocity)*d <= 0))
        return (2 * (ball_1.velocity-ball_2.velocity) / (1/ball_1.mass+1/ball_2.mass) * d.norm()) * d.norm();
    else
        return vec::Vector2(0,0);
}

void reflectBall(Ball &ball_1, Ball &ball_2, Time time){
    vec::Vector2 dp = getdP(ball_1, ball_2);

    ball_2.velocity += dp/ball_2.mass;
    ball_1.velocity -= dp/ball_1.mass;

    ball_1.initialPosition = vec::Vector2(ball_1.circle.getPosition().x, ball_1.circle.getPosition().y);
    ball_2.initialPosition = vec::Vector2(ball_2.circle.getPosition().x, ball_2.circle.getPosition().y);

    ball_1.timeOfCreation = time.asSeconds();
    ball_2.timeOfCreation = time.asSeconds();
}

int main()
{
    int ballCount = 0;
    RenderWindow window(VideoMode(1200, 1000), "SFML Window"/*, sf::Style::Fullscreen*/);
    //window.setFramerateLimit(120);
    Event event;
    Clock clock;
    Clock clock1;

    std::vector<Ball> balls;

    float start = 0;

    while(window.isOpen()){
        Time time = clock.getElapsedTime();
        Time time1 = clock1.getElapsedTime();
        Ball ball;

        while(window.pollEvent(event)){
            if(event.type == Event::Closed)
                window.close();
            if(event.type == event.MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                start = time1.asSeconds();
            if(event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left){
                ball.mass = time1.asSeconds() - start;
                setBall(ball, window, time);
                balls.push_back(ball);
                ballCount++;
            }
        }

        if(Mouse::isButtonPressed(Mouse::Left)){
            ball.mass = time1.asSeconds() - start;
            setBall(ball, window, time);
        }

        for(auto &i: balls) {
            i.circle.setPosition(i.initialPosition.x + i.velocity.x * (time.asSeconds() - i.timeOfCreation),
                                 i.initialPosition.y + i.velocity.y * (time.asSeconds() - i.timeOfCreation));
            reflectWall(i, window, time);
        }

        for(int i = 0; i < ballCount; i++) {
            for(int j = i + 1; j < ballCount; j++)
                reflectBall(balls[i], balls[j], time);
        }

        window.clear();
        for(auto &i : balls)
            window.draw(i.circle);
        window.draw(ball.circle);
        window.display();
    }

    return 0;
}


// Launching this prm:
//
//g++ -c main.cpp
//g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
//./sfml-app
