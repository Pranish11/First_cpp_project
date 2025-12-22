// library
#include <SFML/Graphics.hpp>
#include <iostream>

#include "SFML/Audio/Listener.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{800, 600}), "Title",sf::Style::Titlebar|sf::Style::Close);
    sf::Clock clock;

    sf::Texture texture("../assets/Sprite-0001.png");
    sf::Texture ground ("../assets/Ground.png");
    sf::Sprite sprite001(texture);
    sf::Sprite Ground_01(ground);


    sprite001.setPosition({40.f,30.f});
    sprite001.setScale({2.f,2.f});
    sprite001.setOrigin({
        sprite001.getLocalBounds().size.x/2.f,
        sprite001.getLocalBounds().size.y/2.f});
    //* Going to use this "facingLeft" for checking if the sprite is facing right or left
    bool facingLeft = true;


    //ground
    Ground_01.setPosition({0.f,0.f});
    Ground_01.setScale({10.f,5.f});

    //hitboxes for player
    sf::RectangleShape PlayerHitbox;
    PlayerHitbox.setSize(sf::Vector2f(sprite001.getGlobalBounds().size.x,sprite001.getGlobalBounds().size.y));
    PlayerHitbox.setPosition(sprite001.getPosition());
    PlayerHitbox.setFillColor(sf::Color{255,0,0,100}); //for testing


    while (window.isOpen())
    {

        //movement

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            sprite001.move({-0.2f,0.f});
            if (!facingLeft) {
                sprite001.setScale({2.f,2.f});
                facingLeft = true;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            sprite001.move({0.2f,0.f});
            if (facingLeft) {
                sprite001.setScale({-2.f,2.f});
                facingLeft = false;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            sprite001.move({0.0f,-0.2f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            sprite001.move({0.0f,0.2f});
        }

        //boundery
        sf::Vector2f position = sprite001.getPosition();
        sf::FloatRect bounds = sprite001.getGlobalBounds();
        if (position.x < 0) {
            sprite001.setPosition({0, position.y});
        }
        if (position.x + bounds.size.x > 800) {
            sprite001.setPosition({800 - bounds.size.x, position.y});
        }
        if (position.y < 0) {
            sprite001.setPosition({position.x, 0});
        }
        if (position.y + bounds.size.y > 650) {
            sprite001.setPosition({position.x, 650 - bounds.size.y});
        }
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
        window.draw(Ground_01);
        //Render ground and background first so the character don't disappear
        window.draw(sprite001);
        window.display();
    }

    return 0;
}