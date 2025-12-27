// library
#include <SFML/Graphics.hpp>
#include <iostream>

#include "SFML/Audio/Listener.hpp"

//TODO : Fix the hitbox of the attack


int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{800, 600}), "Title",sf::Style::Titlebar|sf::Style::Close);
    sf::Clock clock;
    int enemyHealth = 150;
    int score = 0;

    sf::Texture texture("../assets/Sprite-0001.png");
    sf::Texture ground ("../assets/Ground.png");
    sf::Texture Attack_001("../assets/Attack_1.png");
    sf::Sprite sprite001(texture);
    sf::Sprite Ground_01(ground);
    sf::Sprite Attack_01(Attack_001);

    sprite001.setPosition({40.f,30.f});
    sprite001.setScale({2.f,2.f});
    sprite001.setOrigin({
        sprite001.getLocalBounds().size.x/2.f,
        sprite001.getLocalBounds().size.y/2.f});
    //* Going to use this "facingLeft" for checking if the sprite is facing right or left
    bool facingLeft = true;

    //* Sprite Global Bounds
    sf::FloatRect SpriteBounds = sprite001.getGlobalBounds();
    sf::FloatRect AttackBounds = Attack_01.getGlobalBounds();

    //ground
    Ground_01.setPosition({0.f,0.f});
    Ground_01.setScale({10.f,5.f});

    //attack
    bool isAttackActive = false;
    float attackSpeed = 1.0f;
    bool attackFacingLeft = true;


    //hitboxes for player
    sf::RectangleShape PlayerHitbox;
    PlayerHitbox.setSize(SpriteBounds.size);
    PlayerHitbox.setPosition(SpriteBounds.position);
    PlayerHitbox.setFillColor(sf::Color{255,0,0,100}); //for testing


    //hitbox for the projectile
    sf::RectangleShape Attack_Hitbox;
    Attack_Hitbox.setSize(AttackBounds.size);
    Attack_Hitbox.setPosition(AttackBounds.position);
    Attack_Hitbox.setFillColor(sf::Color{255,0,0,100}); //for testing



    while (window.isOpen())
    {
        float dt = clock.getElapsedTime().asSeconds();

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

        //boundary
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

        //* Update Hitbox of the player
        PlayerHitbox.setSize(bounds.size);
        PlayerHitbox.setPosition(bounds.position);

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
        //*player hitbox
        // window.draw(PlayerHitbox)

        //for attacking
        sf::Vector2f  AttackPosition= Attack_01.getPosition();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
            if (!isAttackActive) {
                isAttackActive = true;  // Mark that we now have an active projectile
                attackFacingLeft = facingLeft;  // Store the direction player is facing

                float attackOffsetX = 30.f; // distance from player

                if (attackFacingLeft)
                {
                    Attack_01.setScale({ 2.f, 2.f });
                    Attack_01.setPosition({
                        PlayerHitbox.getPosition().x - attackOffsetX,
                        PlayerHitbox.getPosition().y
                    });
                }
                else
                {
                    Attack_01.setScale({ -2.f, 2.f });
                    Attack_01.setPosition({
                        PlayerHitbox.getPosition().x + attackOffsetX,
                        PlayerHitbox.getPosition().y
                    });
                }
            }
        }

        if (isAttackActive) {
            // Move the projectile in the direction it was fired
            if (attackFacingLeft) {
                Attack_01.move({-attackSpeed, 0.f});  // Move left
            } else {
                Attack_01.move({attackSpeed, 0.f});   // Move right
            }

            // Check if projectile has gone off-screen (window bounds)
            sf::Vector2f attackPos = Attack_01.getPosition();
            sf::FloatRect attackBounds = Attack_01.getGlobalBounds();

            // Delete projectile if it goes beyond window boundaries
            if (attackPos.x + attackBounds.size.x < 0 ||
                attackPos.x > 800) {
                isAttackActive = false;
                }
            Attack_Hitbox.setSize(AttackBounds.size);
            Attack_Hitbox.setPosition(AttackPosition);
            window.draw(Attack_Hitbox);
            window.draw(Attack_01);
        }
        window.display();
    }

    return 0;
}