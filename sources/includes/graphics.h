#pragma once

#include <includes/assetUtils.h>
#include <includes/types.h>

#include <SFML/Graphics.hpp>

namespace Shooter {


	// the graphic "system"
	class Graphics {
		public:
			Graphics(int width, int height);
			~Graphics();
			void Update(std::vector<RenderEntry> renderEntries);
			void AddToFrame(sf::Drawable& drawable);
			const sf::Texture& GetMainTexture();

		protected:
		private:
			const std::string fontName = "DejaVuSans.ttf";
			// the render texture that directly shows on screen
			sf::RenderTexture* mainRenderTexture_;
			sf::Font font_;
			void Render(std::vector<RenderEntry> renderEntries);
	};
}