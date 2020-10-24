#include "pch.h"
#include "../CHIP8/chip8.cpp"
#include <string>
#include <fstream>
#include <iostream>



struct Chip8Test 
	:public::testing::Test
{
	const std::string ROM_PATH = "..\\ROMS\\BC_test.ch8";
	const uint16_t TESTING_ADDRESS = 0x456;
	const uint16_t NEXT_INSTRUCTION = 0x202;

	Chip8* chip;

	void SetUp() override {
		chip = new Chip8();
	}

	void TearDown() override {
		delete chip;
	}
};


TEST_F(Chip8Test, LoadedRomCorrectness) {
	
	chip->load_rom(ROM_PATH);

	std::ifstream rom;
	rom.open(ROM_PATH, std::ios::binary | std::ios::ate);

	ASSERT_EQ(rom.is_open(), true);

	if (rom.is_open()) {

		/*
		Comparing count of instructions loaded into the rom,
		and count of instructions loaded into the chip memory.
		*/

		std::streampos file_size = rom.tellg();

		ASSERT_EQ((chip->PROGRAM_START_ADDRESS + file_size) + 1 == chip->program_end_address, true);

		/*
		Comparing each byte from rom file to each byte loaded into
		program segment in chip memory.
		*/

		rom.seekg(0, std::ios::beg);
		rom.unsetf(std::ios::skipws);

		uint8_t byte_from_file{};
		uint8_t byte_from_memory{};
		uint16_t instruction_address = chip->PROGRAM_START_ADDRESS;

		while (rom >> byte_from_file) {

			byte_from_memory = chip->memory[instruction_address];

			ASSERT_EQ(byte_from_file, byte_from_memory);

			instruction_address++;
		}

		rom.close();
	}
}


TEST_F(Chip8Test, Op00E0) {

	for (auto& pixel : chip->screen)
		pixel = 1000;

	chip->op_00e0();

	for (auto& pixel : chip->screen)
		ASSERT_EQ(pixel, 0);
}


TEST_F(Chip8Test, Op00EE) {

	chip->stack[chip->sp] = TESTING_ADDRESS;
	chip->sp++;

	chip->op_00ee();

	ASSERT_EQ(chip->sp, 0);
	ASSERT_EQ(chip->stack[chip->sp], TESTING_ADDRESS);
}


TEST_F(Chip8Test, Op1nnn) {

	chip->memory[TESTING_ADDRESS] = 123;
	chip->opcode = 0x1456;

	chip->op_1nnn();

	ASSERT_EQ(chip->memory[chip->pc], 123);
}


TEST_F(Chip8Test, Op2nnn) {

	chip->opcode = 0x2456;
	chip->memory[TESTING_ADDRESS] = 123;

	chip->op_2nnn();

	ASSERT_EQ(chip->sp, 1);
	ASSERT_EQ(chip->stack[chip->sp - 1], chip->PROGRAM_START_ADDRESS);
	ASSERT_EQ(chip->memory[chip->pc], 123);
}


TEST_F(Chip8Test, Op3xkk) {

	chip->opcode = 0x3456;
	chip->v_registers[4] = 0x56;

	chip->op_3xkk();

	ASSERT_EQ(chip->pc, NEXT_INSTRUCTION);
}


TEST_F(Chip8Test, Op4xkk) {

	chip->opcode = 0x4456;
	chip->v_registers[4] = 0x55;

	chip->op_4xkk();

	ASSERT_EQ(chip->pc, NEXT_INSTRUCTION);
}


TEST_F(Chip8Test, Op5xy0) {

	chip->opcode = 0x5450;
	chip->v_registers[4] = 0x56;
	chip->v_registers[5] = 0x56;

	chip->op_5xy0();

	ASSERT_EQ(chip->pc, NEXT_INSTRUCTION);
}


TEST_F(Chip8Test, Op6xkk) {

	chip->opcode = 0x6456;
	
	chip->op_6xkk();

	ASSERT_EQ(chip->v_registers[4], 0x56);
}


TEST_F(Chip8Test, Op7xkk) {

	chip->opcode = 0x7456;

	chip->op_7xkk();

	ASSERT_EQ(chip->v_registers[4], 0x56);
}


TEST_F(Chip8Test, Op8xy0) {

	chip->opcode = 0x8450;
	chip->v_registers[5] = 0x56;

	chip->op_8xy0();

	ASSERT_EQ(chip->v_registers[4], chip->v_registers[5]);
}


TEST_F(Chip8Test, Op8xy1) {

	chip->opcode = 0x8451;
	chip->v_registers[4] = 0x56;
	chip->v_registers[5] = 0x3;

	chip->op_8xy1();

	ASSERT_EQ(chip->v_registers[4], 0x57);
}


TEST_F(Chip8Test, Op8xy2) {

	chip->opcode = 0x8452;
	chip->v_registers[4] = 0x56;
	chip->v_registers[5] = 0x3;

	chip->op_8xy2();

	ASSERT_EQ(chip->v_registers[4], 0x2);
}


TEST_F(Chip8Test, Op8xy3) {

	chip->opcode = 0x8453;
	chip->v_registers[4] = 0x56;
	chip->v_registers[5] = 0x3;

	chip->op_8xy3();

	ASSERT_EQ(chip->v_registers[4], 0x55);
}


TEST_F(Chip8Test, Op8xy4WithCarryBit) {

	chip->opcode = 0x8454;
	chip->v_registers[4] = 0xFF;
	chip->v_registers[5] = 0x1;

	chip->op_8xy4();

	ASSERT_EQ(chip->v_registers[4], 0);
	ASSERT_EQ(chip->v_registers[0xF], 1);
}


TEST_F(Chip8Test, Op8xy4WithoutCarryBit) {

	chip->opcode = 0x8454;
	chip->v_registers[4] = 0xFE;
	chip->v_registers[5] = 0x1;

	chip->op_8xy4();

	ASSERT_EQ(chip->v_registers[4], 0xFF);
	ASSERT_EQ(chip->v_registers[0xF], 0);
}


TEST_F(Chip8Test, Op8xy5WithCarryBit) {

	chip->opcode = 0x8455;
	chip->v_registers[4] = 0xFF;
	chip->v_registers[5] = 0x1;

	chip->op_8xy5();

	ASSERT_EQ(chip->v_registers[4], 0xFE);
	ASSERT_EQ(chip->v_registers[0xF], 1);
}


TEST_F(Chip8Test, Op8xy5WithoutCarryBit) {

	chip->opcode = 0x8455;
	chip->v_registers[4] = 0x01;
	chip->v_registers[5] = 0xFF;

	chip->op_8xy5();

	ASSERT_EQ(chip->v_registers[4], 2);
	ASSERT_EQ(chip->v_registers[0xF], 0);
}


TEST_F(Chip8Test, Fx33) {

	chip->opcode = 0xF433;
	chip->v_registers[4] = 123;

	chip->op_Fx33();

	ASSERT_EQ(chip->memory[chip->index], 1);
	ASSERT_EQ(chip->memory[chip->index + 1], 2);
	ASSERT_EQ(chip->memory[chip->index + 2], 3);
}


TEST_F(Chip8Test, Fx55) {

	for (int i = 0; i < 16; i++) {
		chip->v_registers[i] = i + 1;
	}

	chip->opcode = 0xFF55;

	chip->op_Fx55();

	for (int i = 0; i < 16; i++) {
		ASSERT_EQ(chip->memory[chip->index + i], i + 1);
	}
}