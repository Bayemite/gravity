#ifndef SF_UTILS_H
#define SF_UTILS_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <cmath>
#include <initializer_list>
#include <vector>
#include <ostream>

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const sf::Vector2<T> &vec)
{
	os << '(' << vec.x << ", " << vec.y << ')';
	return os;
}

// Helper- get centre coodinates of window
inline sf::Vector2f center(const sf::WindowBase &window)
{
	return sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
}

template <typename T>
inline sf::Vector2<T> center(const sf::Rect<T> &rect)
{
	return rect.getSize() - rect.getPosition();
}

template <typename To, typename From>
inline sf::Vector2<To> round(const sf::Vector2<From> &vec)
{
	return sf::Vector2<To>(std::round(vec.x), std::round(vec.y));
}

template <typename To, typename From>
inline sf::Vector2<To> round_vec2(const sf::Vector2<From> &vec)
{
	static_assert(std::is_floating_point<From>::value, "round called with non-float type for 'typename From'.");
	static_assert(std::is_arithmetic<To>::value, "round called with non-arithmetic type for 'typename To'.");
	return sf::Vector2<To>(std::round(vec.x), std::round(vec.y));
}

inline void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow &window, float zoom, sf::View &view)
{
	const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel)};
	// sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{window.mapPixelToCoords(pixel)};
	const sf::Vector2f offsetCoords{beforeCoord - afterCoord};
	view.move(offsetCoords);
	window.setView(view);
}

// Helper for multiple keys query using sf::Keyboard::isKeyPressed
// Duplicate key queries are not removed
struct KeysPressed
{
	KeysPressed(std::initializer_list<sf::Keyboard::Key> key_list)
	{
		m_pressed.reserve(key_list.size());
		for (const sf::Keyboard::Key &key : key_list)
		{
			if (sf::Keyboard::isKeyPressed(key))
			{
				m_pressed.push_back(key);
			}
		}
	}

	bool is_pressed(sf::Keyboard::Key key_pressed) const
	{
		for (sf::Keyboard::Key key : m_pressed)
		{
			if (key == key_pressed)
				return true;
		}
		return false;
	}

	bool any_pressed() const
	{
		return (!m_pressed.empty());
	}

	bool any_pressed(const std::vector<sf::Keyboard::Key> &keys) const
	{
		for (sf::Keyboard::Key key : m_pressed)
		{
			for (sf::Keyboard::Key query_key : keys)
			{
				if (key == query_key)
					return true;
			}
		}
		return false;
	}

private:
	std::vector<sf::Keyboard::Key> m_pressed;
};

#endif