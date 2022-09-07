#include "app.h"

#include "circle_buffer.h"

App::App() : window{sf::VideoMode({800, 600}), "Gravity", sf::Style::Default, sf::ContextSettings(0, 0, 4)},
             view{sf::FloatRect(sf::Vector2f(0, 0), window.getView().getSize())},
             leftClicked{false},
             viewZoom{1.f}
{
    window.setFramerateLimit(60);

    gravPoints.push_back(GravityPoint({0.f, 0.f}));
    gravPoints[0].setFillColor(color::gray);
    gravPoints[0].setInitialVelocity({10.f, 0.f});
    gravPoints[0].setMass(1.f);
    gravPoints[0].setRadius(10.f);

    // The heavier one
    gravPoints.push_back(GravityPoint(center(window)));
    gravPoints[1].setFillColor(color::steel_blue);
    gravPoints[1].setMass(1000.f);
}

void App::handleEvents()
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
            view.reset({{0.f, 0.f}, sf::Vector2f(data.width, data.height)});
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

void App::stepSim()
{
    std::size_t size = gravPoints.size();
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (i != j)
                gravPoints[i].updateVelocity(gravPoints[j]);

    for (GravityPoint &p : gravPoints)
        p.updatePosition();
}

void App::run()
{
    CircleBuffer<sf::CircleShape> paths;
    paths.setSize(256);

    while (window.isOpen())
    {
        handleEvents();
        stepSim();
        window.clear(sf::Color::Black);

        // Template obj.
        sf::CircleShape pathPoint{2.f};
        pathPoint.setOrigin({2.f, 2.f});

        for (int i = 0; i < paths.size(); i++)
        {
            auto &dot = paths[i];
            sf::Color color = dot.getFillColor();
            color.a = i;
            dot.setFillColor(color);
            window.draw(paths[i]);
        }

        for (int i = 0; i < gravPoints.size(); i++)
        {
            auto &point = gravPoints[i];
            pathPoint.setPosition(point.getPosition());
            paths.push(pathPoint);
            window.draw(point);
        }

        window.display();
    }
}