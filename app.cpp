#include "app.h"

#include <execution>
#include <numeric>

App::App() : window{sf::VideoMode({800, 600}), "Gravity", sf::Style::Default, sf::ContextSettings(0, 0, 4)},
             view{sf::FloatRect(sf::Vector2f(0, 0), window.getView().getSize())},
             plainView{view},
             viewZoom{1.f},
             leftClicked{false},
             newGravPoint{false},
             m_newGravPointVec{{0.f, 0.f}, {0.f, 0.f}, 8.f}
{
    window.setFramerateLimit(60);

    m_newGravPointVec.setFillColor(sf::Color::White);

    addGravPoint({0.f, 0.f}, {0.f, 0.f});
    gravPoints[0].setFillColor(color::gray);
    gravPoints[0].setInitialVelocity({10.f, 0.f});
    gravPoints[0].setMass(1.f);
    gravPoints[0].setRadius(10.f);

    addGravPoint(center(window), {0.f, 0.f});
    gravPoints[1].setFillColor(color::steel_blue);
    gravPoints[1].setMass(1000.f);
}

void App::addGravPoint(const sf::Vector2f &coords, const sf::Vector2f &velocity)
{
    GravityPoint point{coords};
    point.setInitialVelocity(velocity);
    gravPoints.push_back(point);
    traces.push_back(Trace(256, gravPoints.back().getFillColor()));
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

            switch (data.button)
            {
            case sf::Mouse::Left:
            {
                leftClicked = true;
                lClickCoords = mouseCoords;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    newGravPoint = true;
                    m_newGravPointVec.setPoint(0, lClickCoords);
                    m_newGravPointVec.setPoint(1, lClickCoords);
                }
                break;
            }
            case sf::Mouse::Middle:
            {
                break;
            }
            default:
                break;
            }
            break;
        }
        case sf::Event::MouseMoved:
        {
            auto &data{event.mouseMove};
            sf::Vector2f coords{window.mapPixelToCoords(sf::Vector2i(data.x, data.y), view)};
            if (leftClicked)
            {
                if (newGravPoint)
                {
                    m_newGravPointVec.setPoint(1, coords);
                }
                else
                {
                    sf::Vector2f offset{mousePixelCoords - sf::Vector2i(data.x, data.y)};
                    view.move(offset * viewZoom);
                    window.setView(view);
                }
            }
            mousePixelCoords = sf::Vector2i(data.x, data.y);
            mouseCoords = coords;
            break;
        }
        case sf::Event::MouseButtonReleased:
        {
            auto &data{event.mouseButton};
            if (data.button == sf::Mouse::Left)
            {
                leftClicked = false;
                if (newGravPoint)
                {
                    float softenFactor = 32.f;
                    addGravPoint(lClickCoords, -m_newGravPointVec.getDirection() / softenFactor);
                    gravPoints.back().setRadius(10.f);
                    gravPoints.back().setMass(1.f);
                    newGravPoint = false;
                    m_newGravPointVec.setPoint(0, {0.f, 0.f});
                    m_newGravPointVec.setPoint(1, {0.f, 0.f});
                }
            }
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
            sf::FloatRect rect{{0.f, 0.f}, sf::Vector2f(data.width, data.height)};
            view.reset(rect);
            plainView.reset(rect);
            viewZoom = 1.f;
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
    sf::Clock deltaClock;

    sf::Font robotoMono;
    if (!robotoMono.loadFromFile("fonts/Roboto_Mono/RobotoMono-VariableFont_wght.ttf"))
    {
        std::cerr << "Could not load Roboto Mono from 'RobotoMono-VariableFont_wght.ttf'.\n";
        return;
    }
    unsigned fontSize = 12;
    sf::Text stats{"FPS: N/A\nn=" + std::to_string(gravPoints.size()), robotoMono, fontSize};
    stats.setPosition({4.f, 4.f}); // Margin from top-left
    int fps = 60;                  // Average over four frames

    while (window.isOpen())
    {
        handleEvents();

        stepSim();
        window.clear(sf::Color::Black);

        for (int i = 0; i < gravPoints.size(); i++)
        {
            auto &point = gravPoints[i];
            window.draw(point);
            traces[i].push(point.getPosition());
        }

        for (const auto &trace : traces)
        {
            window.draw(trace);
        }

        window.draw(m_newGravPointVec);

        window.setView(plainView);

        int avgFps = (fps + 1000000.f / deltaClock.restart().asMicroseconds()) / 2;
        fps = avgFps;

        stats.setString(
            "FPS: " + std::to_string(avgFps) + "\n" +
            "n=" + std::to_string(gravPoints.size()));
        window.draw(stats);

        window.setView(view);

        window.display();
    }
}