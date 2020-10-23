#pragma once

#include <SDL.h>


class Display
{
	public:
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;
		
	public:
		Display(const char* title, int width, int height);
		~Display();

		void draw(uint32_t *screen);

};

