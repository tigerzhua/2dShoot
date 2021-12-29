#include "includes/graphics.h"

#include <iostream>

#include <SFML/Graphics.hpp>

#include "includes/systemConsts.h"

namespace Shooter {

	Graphics::Graphics(int width, int height) {
		mainRenderTexture_ = new sf::RenderTexture();
		if (!mainRenderTexture_->create(width, height)) {
			std::cout<<"[Error]: Graphics System Init fail"<< std::endl;
		}

		AssetUtils::LoadFont(fontName, font_);
		std::cout << "Graphics System Init complete" << std::endl;
	}

	Graphics::~Graphics() {
		delete(mainRenderTexture_);
		mainRenderTexture_ = nullptr;
	}

	void Graphics::Update(std::vector<RenderEntry> renderEntries) {
		Render(renderEntries);
	}

	void Graphics::Render(std::vector<RenderEntry> renderEntries) {
		mainRenderTexture_->clear();
		for (unsigned int i = 0; i < renderEntries.size(); ++i) {
			if (renderEntries[i].isText) {
				sf::Text text;
				text.setFont(font_);
				text.setPosition(renderEntries[i].position);
				text.setString(renderEntries[i].text);
				text.setCharacterSize(renderEntries[i].fontSize); // in pixels
				text.setFillColor(sf::Color::White);
				//text.setStyle(sf::Text::Bold | sf::Text::Underlined);

				mainRenderTexture_->draw(text);
			}
			else {
				sf::Sprite sprite = sf::Sprite();
				if (renderEntries[i].sprite != nullptr) {
					sprite = *renderEntries[i].sprite;
				}
				else {
					if (renderEntries[i].texture != nullptr) {
						sprite.setTexture(*renderEntries[i].texture);
					}
					else { // default texture
						sprite.setTexture(sf::Texture());
					}
				}

				sprite.setPosition(renderEntries[i].position);
				if (renderEntries[i].useTextureRect == true) {
					sprite.setTextureRect(renderEntries[i].textureRect);
				}
				mainRenderTexture_->draw(sprite);
			}
		}

		mainRenderTexture_->display();
	}

	void Graphics::AddToFrame(sf::Drawable& drawable) {
		mainRenderTexture_->draw(drawable);
	}

	const sf::Texture& Graphics::GetMainTexture() {
		return mainRenderTexture_->getTexture();
	}
}
