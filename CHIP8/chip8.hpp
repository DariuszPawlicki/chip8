#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>

class Chip8
{

	typedef void (Chip8::*FuncPtr)();


	public:

		static const uint16_t FONT_LOCATION_ADDRESS = 0x50;
		static const uint16_t PROGRAM_START_ADDRESS = 0x200;

		uint16_t program_end_address{ 0 };
		uint8_t memory[4096]{ 0 };
		uint8_t v_registers[16]{ 0 }; // Registers annotated as Vx, where x is HEX digit 0 - F
		uint16_t index{ 0 }; // Register for storing addresses
		uint16_t pc{ 0 }; // Program counter
		uint16_t opcode{ 0 }; // Instruction
		uint32_t screen[64 * 32]{ 0 };
		uint16_t stack[16]{ 0 }; // Stack for storing PC's
		uint8_t sp{ 0 }; // Stack pointer
		uint8_t keypad[16]{ 0 };
		uint8_t delay_timer{ 0 };
		uint8_t sound_timer{ 0 };
		uint8_t font[80] = {

			0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
			0x20, 0x60, 0x20, 0x20, 0x70, // 1
			0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
			0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
			0x90, 0x90, 0xF0, 0x10, 0x10, // 4
			0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
			0xF0, 0x10, 0x20, 0x40, 0x40, // 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
			0xF0, 0x90, 0xF0, 0x90, 0x90, // A
			0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
			0xF0, 0x80, 0x80, 0x80, 0xF0, // C
			0xE0, 0x90, 0x90, 0x90, 0xE0, // D
			0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
			0xF0, 0x80, 0xF0, 0x80, 0x80  // F

		};

		FuncPtr intructions_unique[0xF + 1]{ 0 };
		FuncPtr intructions_0[0xE + 1]{ 0 };
		FuncPtr intructions_8[0xE + 1]{ 0 };
		FuncPtr intructions_E[0xE + 1]{ 0 };
		FuncPtr intructions_F[0x65 + 1]{ 0 };
		
		

	public:

		void load_rom(std::string file_path);

		void op_00e0(); // Clear screen

		void op_00ee(); // Return

		void op_1nnn(); // Jump to nnn

		void op_2nnn(); // Call subroutine in nnn

		void op_3xkk(); // Skip next instruction if Vx == kk

		void op_4xkk(); // Skip next instruction if Vx != kk

		void op_5xy0(); // Skip next instruction if Vx == Vy

		void op_6xkk(); // Set Vx to kk

		void op_7xkk(); // Set Vx += kk

		void op_8xy0(); // Set Vx = Vy

		void op_8xy1(); // Set Vx = Vx OR Vy

		void op_8xy2(); // Set Vx = Vx AND Vy

		void op_8xy3(); // Set Vx = Vx XOR Vy

		void op_8xy4(); // Set Vx = Vx + Vy and VF = carry_bit

		void op_8xy5(); // Set Vx = Vx - Vy and VF = 1 if Vx > Vy

		void op_8xy6(); // Set Vx = Vx SHR 1 - shift right 1 - if least-significant bit of Vx is 1
						// then VF = 1 else 0 then Vx /= 2

		void op_8xy7(); // Set Vx = Vy - Vx if Vy > Vx then VF = 1 else 0

		void op_8xyE(); // Set Vx = Vx SHL 1 - shift left 1 - if most-significant bit is 1 then VF = 1 else 0, then
						// Vx *= 2

		void op_9xy0(); // Skip next instruction if Vx != Vy

		void op_Annn(); // Set index register to nnn

		void op_Bnnn(); // Jump to location V0 + nnn

		void op_Cxkk(); // Set Vx to random byte AND kk

		void op_Dxyn(); // Display n-byte sprite starting from address at index register at (Vx, Vy) location,
						// if some pixels overlapping VF = 1 else 0

		void op_Ex9E(); // Skip next instruction if key[Vx] is pressed

		void op_ExA1(); // Skip next instruction if key[Vx] is not pressed

		void op_Fx07(); // Set Vx = delay timer value

		void op_Fx0A(); // Wait for key press, store value of key in Vx

		void op_Fx15(); // Set delay timer = Vx

		void op_Fx18(); // Set sound timer = Vx

		void op_Fx1E(); // Set index += Vx

		void op_Fx29(); // Set index = location of sprite of value which is stored in Vx

		void op_Fx33(); // Takes decimal value of Vx and stores hundreds digit in index location, tens digit in index + 1 location, 
					    // and ones digit in index + 2 location

		void op_Fx55(); // Copy value of registers V0-Vx to memory starting at location index

		void op_Fx65(); // Read registers V0-Vx starting at location index

		void table_0_func();

		void table_8_func();

		void table_E_func();

		void table_F_func();

		void cpu_cycle();

		Chip8();

};