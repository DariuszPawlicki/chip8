#include "chip8.hpp"
#include "display.hpp"
#include <chrono>



int main(int argc, char* argv[])
{  
    Chip8 platform;
    Display display("CHIP8", 10);

    platform.load_rom("..\\ROMS\\pong.ch8");

    float delay = 1.5;    
    bool exit = false;
    auto last_cycle_time = std::chrono::high_resolution_clock::now();

    while (!exit) {

        exit = display.keypad_handler(platform.keypad);

        auto current_time = std::chrono::high_resolution_clock::now();
        float time_diff = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last_cycle_time).count();

        if (time_diff >= delay) {            
            platform.cpu_cycle();
            display.draw(platform.screen);

            last_cycle_time = current_time;
        }    
    }

    return 0;
}