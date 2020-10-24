#include "display.hpp"



Display::Display(const char* title, int scale_factor) {

	SDL_Init(SDL_INIT_EVERYTHING);

	this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
									64 * scale_factor, 32 * scale_factor, SDL_WINDOW_SHOWN);

	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->renderer);
	SDL_RenderPresent(this->renderer);
}


Display::~Display() {

	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyTexture(this->texture);
}


void Display::draw(uint32_t *screen) {

	SDL_UpdateTexture(this->texture, nullptr, screen, sizeof(*screen) * 64);
	SDL_RenderClear(this->renderer);
	SDL_RenderCopy(this->renderer, this->texture, nullptr, nullptr);
	SDL_RenderPresent(this->renderer);
}


bool Display::keypad_handler(uint8_t *keypad) {

    bool exit = false;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_KEYDOWN) {

            auto key_pressed = event.key.keysym.sym;

            switch (key_pressed) {

            case SDLK_1:
                keypad[0] = 1;
                break;
            case SDLK_2:
                keypad[1] = 1;
                break;
            case SDLK_3:
                keypad[2] = 1;
                break;
            case SDLK_4:
                keypad[3] = 1;
                break;
            case SDLK_q:
                keypad[4] = 1;
                break;
            case SDLK_w:
                keypad[5] = 1;
                break;
            case SDLK_e:
                keypad[6] = 1;
                break;
            case SDLK_r:
                keypad[7] = 1;
                break;
            case SDLK_a:
                keypad[8] = 1;
                break;
            case SDLK_s:
                keypad[9] = 1;
                break;
            case SDLK_d:
                keypad[10] = 1;
                break;
            case SDLK_f:
                keypad[11] = 1;
                break;
            case SDLK_z:
                keypad[12] = 1;
                break;
            case SDLK_x:
                keypad[13] = 1;
                break;
            case SDLK_c:
                keypad[14] = 1;
                break;
            case SDLK_v:
                keypad[15] = 1;
                break;
            case SDLK_ESCAPE:
                exit = true;
                break;
            }

        }
        else if (event.type == SDL_KEYUP) {

            auto key_released = event.key.keysym.sym;

            switch (key_released) {

            case SDLK_1:
                keypad[0] = 0;
                break;
            case SDLK_2:
                keypad[1] = 0;
                break;
            case SDLK_3:
                keypad[2] = 0;
                break;
            case SDLK_4:
                keypad[3] = 0;
                break;
            case SDLK_q:
                keypad[4] = 0;
                break;
            case SDLK_w:
                keypad[5] = 0;
                break;
            case SDLK_e:
                keypad[6] = 0;
                break;
            case SDLK_r:
                keypad[7] = 0;
                break;
            case SDLK_a:
                keypad[8] = 0;
                break;
            case SDLK_s:
                keypad[9] = 0;
                break;
            case SDLK_d:
                keypad[10] = 0;
                break;
            case SDLK_f:
                keypad[11] = 0;
                break;
            case SDLK_z:
                keypad[12] = 0;
                break;
            case SDLK_x:
                keypad[13] = 0;
                break;
            case SDLK_c:
                keypad[14] = 0;
                break;
            case SDLK_v:
                keypad[15] = 0;
                break;
            }

        }
        else if (event.type == SDL_QUIT) {

        exit = true;
        }

    }

    return exit;
}