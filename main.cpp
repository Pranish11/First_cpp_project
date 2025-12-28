// library
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <chrono>      


int randomness() {
    std::mt19937 rng(
            static_cast<unsigned int>(
                std::chrono::high_resolution_clock::now().time_since_epoch().count()
            )
        );
    std::uniform_int_distribution<int> distribution(1, 2);
    int rand = distribution(rng);
    return rand;
}

sf::Vector2i randomPos() {
    static std::mt19937 rng(
            static_cast<unsigned int>(
                std::chrono::high_resolution_clock::now().time_since_epoch().count()
            )
        );
    std::uniform_int_distribution<int> RandPosX(10, 700);
    std::uniform_int_distribution<int> RandPosY(10, 500);

    return {RandPosX(rng), RandPosY(rng)};
}

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{800, 600}), "Title",sf::Style::Titlebar|sf::Style::Close);
    int score = 0;
    int enemyHealth = 150;

    // displaying score
    sf::Font font("../assets/Jetbrains_Font.ttf");
    sf::Text TextScore(font);
    TextScore.setCharacterSize(15);
    TextScore.setFillColor(sf::Color::White);
    TextScore.setPosition({10.f,10.f});

    sf::Texture texture("../assets/Sprite-0001.png");
    sf::Texture ground ("../assets/Ground.png");
    sf::Texture Attack_001("../assets/Attack_1.png");
    sf::Texture Enemy("../assets/Enemy.png");
    sf::Sprite sprite001(texture);
    sf::Sprite Ground_01(ground);
    sf::Sprite Attack_01(Attack_001);
    sf::Sprite Enemy_01(Enemy);

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
    sf::FloatRect EnemyBounds =Enemy_01.getGlobalBounds();

    //ground
    Ground_01.setPosition({0.f,0.f});
    Ground_01.setScale({10.f,5.f});

    //attack
    bool isAttackActive = false;
    bool attackFacingLeft = true;

    //for enemy
    Enemy_01.setScale({2.f,2.f});
    int directionEnemy = randomness() % 2;
    bool EnemyAlive = true;
    float EnemySpeed = 0.2f;


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

    //hitbox for enemy
    sf::RectangleShape Enemy_Hitbox;
    Enemy_Hitbox.setSize(EnemyBounds.size);
    Enemy_Hitbox.setPosition(EnemyBounds.position);
    Enemy_Hitbox.setFillColor(sf::Color{255,0,0,100});



    while (window.isOpen())
    {
        window.clear(sf::Color::White);
        window.draw(Ground_01);
        // float dt = clock.getElapsedTime().asSeconds();

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


        sf::Vector2f position = sprite001.getPosition();
        sf::FloatRect bounds = sprite001.getGlobalBounds();
        //* boundary


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

        //Enemy

        sf::FloatRect EnemyBounds = Enemy_01.getGlobalBounds();
        if (EnemyAlive) {
            // Move enemy side to side
            if (directionEnemy == 0) {
                Enemy_01.move({-EnemySpeed, 0.f});
            } else {
                Enemy_01.move({EnemySpeed, 0.f});
            }


            if (Enemy_01.getPosition().x < -50 || Enemy_01.getPosition().x > 850) {
                sf::Vector2i newPos = randomPos();
                Enemy_01.setPosition(static_cast<sf::Vector2f>(newPos));
                directionEnemy = randomness() % 2;
                enemyHealth = 150;
            }

            // Update enemy hitbox
            Enemy_Hitbox.setSize(EnemyBounds.size);
            Enemy_Hitbox.setPosition(EnemyBounds.position);

            window.draw(Enemy_01);
            // window.draw(Enemy_Hitbox);  // for testing
        }

        window.draw(Enemy_01);


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

        //Render ground and background first so the character don't disappear
        window.draw(sprite001);
        //*player hitbox
        // window.draw(PlayerHitbox)

        //for attacking
        sf::FloatRect AttackBounds = Attack_01.getGlobalBounds();

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
            float attackSpeed = 1.0f;
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
            if (attackPos.x + attackBounds.size.x < 0 || attackPos.x > 800) {
                isAttackActive = false;
                }

            Attack_Hitbox.setSize(AttackBounds.size);
            Attack_Hitbox.setPosition(AttackBounds.position);
            // window.draw(Attack_Hitbox);
            window.draw(Attack_01);

            //* Collision
            if (isAttackActive && EnemyAlive) {
                if (Attack_Hitbox.getGlobalBounds().findIntersection(Enemy_Hitbox.getGlobalBounds())) {
                    enemyHealth -= 150;
                    if (enemyHealth < 0) {
                        EnemyAlive = false;
                        score += 150;

                        //spawing a new enemy
                        sf::Vector2i newPos = randomPos();
                        Enemy_01.setPosition(static_cast<sf::Vector2f>(newPos));
                        directionEnemy = randomness() % 2;
                        enemyHealth = 150;
                        EnemyAlive = true;
                    }
                }
            }
        }
        TextScore.setString("Score: " + std::to_string(score));
        window.draw(TextScore);
        window.display();
    }
    return 0;
}