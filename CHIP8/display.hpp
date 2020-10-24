#pragma once

#include <SDL.h>


class Display
{
	public:
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;
		
	public:
		Display(const char* title, int scale_factor);
		~Display();

		void draw(uint32_t *screen);
		bool keypad_handler(uint8_t *keypad);

};