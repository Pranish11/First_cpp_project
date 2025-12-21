// library
#include <SFML/Graphics.hpp>
#include <iostream>


int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{800, 600}), "Title");


    sf::Texture texture("../assets/Sprite-0001.png");
    sf::Sprite sprite001(texture);

    sprite001.setPosition({40.f,30.f});
    sprite001.setScale({2.f,2.f});



    while (window.isOpen())
    {
            // handle events
            while (auto event = window.pollEvent())
            {
                // on close button release
                if (event->is<sf::Event::Closed>())
                {
                    // close window
                    window.close();
                }

                // on window resize
                else if (auto resized = event->getIf<sf::Event::Resized>())
                {
                    // update view
                    window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resized->size))));
                }
            }
        window.clear(sf::Color::White);
        window.draw(sprite001);
        window.display();
    }

    return 0;
}