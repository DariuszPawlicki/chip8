#include "chip8.hpp"
#include "display.hpp"



int main(int argc, char* argv[])
{  
    Chip8 platform;
    Display display("CHIP8", 640, 320);
    

    platform.load_rom("..\\ROMS\\IBM_Logo.ch8");

    
    while (true) {
        platform.cpu_cycle();
        display.draw(platform.screen);
    }


    SDL_Delay(5000);

    return 0;
}
