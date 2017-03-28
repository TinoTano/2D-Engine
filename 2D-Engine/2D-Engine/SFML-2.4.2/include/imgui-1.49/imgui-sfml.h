#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

namespace sf
{
    class Event;
    class RenderTarget;
    class RenderWindow;
    class Sprite;
    class Texture;
    class Window;
}

namespace ImGui
{
namespace SFML
{
    void Init(sf::RenderTarget& target, sf::Texture* fontTexture);

    void ProcessEvent(const sf::Event& event);

    void Update(sf::RenderWindow& window, sf::Time dt);
    void Update(sf::Window& window, sf::RenderTarget& target, sf::Time dt);
    void Update(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize, sf::Time dt);

    void Shutdown();

    void createFontTexture(sf::Texture& texture);
    void setFontTexture(sf::Texture& texture);
}

// custom ImGui widgets for SFML stuff

// Image overloads
    void Image(const sf::Texture& texture,
        const sf::Color& tintColor,
        const sf::Color& borderColor);
    void Image(const sf::Texture& texture, const sf::Vector2f& size,
        const sf::Color& tintColor,
        const sf::Color& borderColor);
    void Image(const sf::Texture& texture, const sf::FloatRect& textureRect,
        const sf::Color& tintColor,
        const sf::Color& borderColor);
    void Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::FloatRect& textureRect,
        const sf::Color& tintColor,
        const sf::Color& borderColor);

    void Image(const sf::Sprite& sprite,
        const sf::Color& tintColor,
        const sf::Color& borderColor);
    void Image(const sf::Sprite& sprite, const sf::Vector2f& size,
        const sf::Color& tintColor,
        const sf::Color& borderColor);

// ImageButton overloads
    bool ImageButton(const sf::Texture& texture, const int framePadding,
                     const sf::Color& bgColor,
                     const sf::Color& tintColor);
    bool ImageButton(const sf::Texture& texture, const sf::Vector2f& size, const int framePadding,
                     const sf::Color& bgColor, const sf::Color& tintColor);

    bool ImageButton(const sf::Sprite& sprite, const int framePadding,
                     const sf::Color& bgColor,
                     const sf::Color& tintColor);
    bool ImageButton(const sf::Sprite& sprite, const sf::Vector2f& size, const int framePadding,
                     const sf::Color& bgColor,
                     const sf::Color& tintColor);

// Draw_list overloads. All positions are in relative coordinates (relative to top-left of the current window)
    void DrawLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& col, float thickness);
    void DrawRect(const sf::FloatRect& rect, const sf::Color& color, float rounding, int rounding_corners, float thickness);
    void DrawRectFilled(const sf::FloatRect& rect, const sf::Color& color, float rounding, int rounding_corners);
}
