#include "sf_utils.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

class App
{
    sf::RenderWindow window;
    sf::View view;
    float viewZoom; // view doesn't track zoom... why??

    sf::Vector2f mouseCoords;
    sf::Vector2i mousePixelCoords;
    bool leftClicked;

    void handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::KeyPressed:
            {
                static sf::Vector2i cart_coords;
                auto &data{event.key};
                switch (data.code)
                {
                default:
                    break;
                }
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                auto &data{event.mouseButton};
                mousePixelCoords = sf::Vector2i(data.x, data.y);
                mouseCoords = window.mapPixelToCoords(sf::Vector2i(data.x, data.y), view);
                if (data.button == sf::Mouse::Left)
                    leftClicked = true;
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                auto &data{event.mouseButton};
                if (data.button == sf::Mouse::Left)
                    leftClicked = false;
                break;
            }
            case sf::Event::MouseMoved:
            {
                auto &data{event.mouseMove};
                sf::Vector2f coords{window.mapPixelToCoords(sf::Vector2i(data.x, data.y), view)};
                if (leftClicked)
                {
                    sf::Vector2f offset{mousePixelCoords - sf::Vector2i(data.x, data.y)};
                    view.move(offset * viewZoom);
                    window.setView(view);
                }
                mousePixelCoords = sf::Vector2i(data.x, data.y);
                mouseCoords = coords;
                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                auto &data{event.mouseWheelScroll};
                float factor = 1 + (-data.delta / 3);
                viewZoom *= factor;
                zoomViewAt({event.mouseWheelScroll.x, event.mouseWheelScroll.y}, window, factor, view);
                break;
            }
            case sf::Event::Resized:
            {
                auto &data{event.size};
                view.setSize(sf::Vector2f(data.width, data.height));
                view.reset({{0.f,0.f}, sf::Vector2f(data.width, data.height)});
                window.setView(view);
                break;
            }
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }

public:
    App() : window{sf::VideoMode({800, 600}), "Gravity"},
            view{sf::FloatRect(sf::Vector2f(0, 0), window.getView().getSize())},
            leftClicked{false},
            viewZoom{1.f}
    {
        window.setFramerateLimit(60);
    }

    void run()
    {
        while (window.isOpen())
        {
            handleEvents();
            window.clear(sf::Color::Black);

            sf::RectangleShape rect({100.f, 100.f});
            window.draw(rect);
            window.display();
        }
    }
};

int main()
{
    App app;
    app.run();

    return 0;
}