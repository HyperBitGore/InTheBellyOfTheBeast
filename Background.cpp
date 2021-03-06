#include "Header.h"


SDL_Texture* Game::loadBackground(int level, SDL_Renderer* rend) {
	Gore gore;
	SDL_Surface* surf = nullptr;
	SDL_Texture* tex = nullptr;
	switch (level) {
	case 1:
		surf = gore.loadPNG("Sprites/back1.png", SDL_PIXELFORMAT_RGBA8888, 800, 800);
		tex = SDL_CreateTextureFromSurface(rend, surf);
		break;
	case 2:
		
		break;
	case 3:
		
		break;
	case 4:
		
		break;
	case 5:

		break;
	}
	SDL_FreeSurface(surf);
	return tex;
}


void Game::updateBackground(SDL_Renderer* rend, SDL_Texture* tex, Entity* back, double delta) {
	
	SDL_Rect rect = { back->x, back->y, back->w, back->h };
	SDL_RenderCopy(rend, tex, NULL, &rect);
}
