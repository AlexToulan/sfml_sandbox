#include "HoverInfo.hpp"
#include "GameMode/ConsoleEvents.hpp"

HoverInfo::HoverInfo(const sf::Font& font)
  : EventListener()
{
  _shouldShow = false;
  _message.setFont(font);
  _background.setFillColor(sf::Color(127, 127, 127, 127));
  _message.setFillColor(sf::Color(255, 255, 255));
  _message.setCharacterSize(12);
}

HoverInfo::~HoverInfo()
{

}

void HoverInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  if (_shouldShow)
  {
    target.draw(_background, states);
    target.draw(_message, states);
  }
}

void HoverInfo::setup()
{
  Events::Console->bind("show_info_text", this, &HoverInfo::show);
  Events::Console->bind("hide_info_text", this, &HoverInfo::hide);
}

void HoverInfo::show(const std::string& message, const sf::Vector2f& location)
{
  _shouldShow = true;

  _message.setString(message);
  _message.setOrigin(_message.getLocalBounds().getSize() * 0.5f + sf::Vector2f(0.0f, 1.0f));
  _background.setSize(_message.getLocalBounds().getSize() + sf::Vector2f(16.0f, 16.0f));
  _background.setOrigin(_background.getLocalBounds().getSize() * 0.5f);

  auto loc = location + sf::Vector2f(_background.getLocalBounds().getSize().x * 0.5f + 24.0f, 24.0f);
  _message.setPosition(loc);
  _background.setPosition(loc);
}

void HoverInfo::hide()
{
  _shouldShow = false;
}
