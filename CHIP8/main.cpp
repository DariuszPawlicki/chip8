#include "chip8.hpp"
#include "display.hpp"
#include <chrono>
#include <iostream>



int main(int argc, char* argv[])
{  

    std::string rom_name;
    float delay;
    int scale_factor;

    if (argc == 4) {

        rom_name = argv[1];
        delay = std::stoi(argv[2]);
        scale_factor = std::stoi(argv[3]);
    }
    else if (argc == 2){

        std::string arg = argv[1];

        if (arg == "--help" || arg == "-h") {

            std::cout << "\n   CHIP8 emu accepts three arguments to run: \n" << std::endl;
            std::cout << "1. Name of the ROM - placed in ROMS directory - you want to run." << std::endl;
            std::cout << "   ROMS should have an .ch8 extension.\n" << std::endl;
            std::cout << "2. Delay in milliseconds you want to apply between each CPU cycle, different games on CHIP8 worked\n" 
                         "   with different timing 500Hz-1000Hz,\n"
                         "   so you must try different delays to get ROM working correctly.\n" << std::endl;
            std::cout << "3. Scale factor of a screen. By default CHIP8 has a 64x32 resolution\n"
                         "   if you apply scale factor of 10, you will get 640x320 resolution screen.\n" << std::endl;           
        }
        else {

            std::cout << "\nTo get help for running an emulator type --help or -h cmd.\n" << std::endl;
        }

        std::exit(EXIT_FAILURE);
    }
    else {

        std::cout << "\nTo get help for running an emulator type --help or -h cmd.\n" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    Chip8 platform;
    Display display("Chip8", scale_factor);

    platform.load_rom("..\\ROMS\\" + rom_name + ".ch8");
      
    bool exit = false;
    auto last_cycle_time = std::chrono::high_resolution_clock::now();

    while (!exit) {

        exit = display.keypad_handler(platform.keypad);

        auto current_time = std::chrono::high_resolution_clock::now();
        auto time_span = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last_cycle_time);
        float time_diff = time_span.count();

        if (time_diff >= delay) {    

            platform.cpu_cycle();
            display.draw(platform.screen);

            last_cycle_time = current_time;
        }    
    }

    return 0;
}