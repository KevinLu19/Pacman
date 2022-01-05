#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

/*
    How to compile and run this file:

    Compile main.cpp: 

    g++ -c main.cpp

    Create and Executable file: 
    g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

    Run the executable file: 
    ./sfml-app
*/

class Animation
{
    public:
    private:
        sf::Texture texture;
};

class PacMan
{
    public:
        PacMan()
        {
            //Load Texture and sprites.
            if (!game_map.loadFromFile("sprites/map.jpg"))
            {
                std::cerr << "Could not load pacman map!" << std::endl;
                exit(1);
            }

            if (!pacman.loadFromFile("sprites/PacMan.png"))
            {
                std::cerr << "Cannot load PacMan file" << std::endl;
                exit(1);
            }

            sprite_game_map.setTexture(game_map);
            sprite_pacman.setTexture(pacman);
            sprite_game_map.scale(sf::Vector2f(0.4f, 0.4f));
            
            // Sub-dividing pacman image to grab only one form of the sprite from animation file.
            // Uses sub rectangle within the file to get what I need.
            // Got 16 because 128 (width pixel) divide by 8 (8 iterations of animation) = 16
            sprite_pacman.setTextureRect({0,0, 16, 16 });
            sprite_pacman.scale(sf::Vector2f(2.5f, 2.5f));
            sprite_pacman.setPosition(sf::Vector2f(1000000000000000.0f, 100000000000000000000.0f));

        }

        void draw(sf::RenderTarget& render) const
        {
            render.draw(sprite_game_map);
            render.draw(sprite_pacman);
        }

        void set_direction(const sf::Vector2f & dir)
        {
            velocity = dir * speed;
        }

        void update(float dt)
        {
            position += velocity * dt;
            sprite_pacman.setPosition(position);
        }

    private:
        static constexpr float speed = 300.0f;
        sf::Vector2f position;
        sf::Vector2f velocity = {0.0f, 0.0f};
        sf::Texture game_map, pacman;
        sf::Sprite sprite_game_map, sprite_pacman;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "PacMan");
    
    PacMan best_pacman_game_ever;

    // Keeping track of time.
    // Use this in order to be able to move within certain fps ranges. E.g. Move x pixels in 30 fps. 
    auto tp = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close(); 
        }

        // Get DT
        float dt;
        {
            const auto newtp = std::chrono::steady_clock::now();
            dt = std::chrono::duration<float>(newtp - tp).count();
            tp = newtp;
        }

        // Event Hanlde Inputs
        sf::Vector2f dir = {0.0f, 0.0f};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            dir.y -= 1.0f;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            dir.y += 1.0f;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            dir.x -= 1.0f;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            dir.x += 1.0f;
        best_pacman_game_ever.set_direction(dir); 

        best_pacman_game_ever.update(dt);

        window.clear();
        best_pacman_game_ever.draw(window);

        window.display();
    }

    return 0;
}
