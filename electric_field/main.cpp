#include "electric_field.h"

const int numberOfLines = 16;
const int maxLength = 2000;

int main(){
    ElectricField E;
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "$$$  FORCE LINES  $$$"/*, Style::Fullscreen*/);

    while (window.isOpen()){
        checkEvents(window, E);

        window.clear();
        drawWindow(window, E, numberOfLines, maxLength);
        window.display();
    }

    return 0;
}
