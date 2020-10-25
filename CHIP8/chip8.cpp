#include "chip8.hpp"


Chip8::Chip8(){
	
	this->pc = PROGRAM_START_ADDRESS;

	for (int i = 0; i < 80; i++) {
		 this->memory[FONT_LOCATION_ADDRESS + i] = font[i];
	}
	
	srand(time(NULL)); // Init random seed for RNG

	this->intructions_unique[0] = &Chip8::table_0_func;
	this->intructions_unique[1] = &Chip8::op_1nnn;
	this->intructions_unique[2] = &Chip8::op_2nnn;
	this->intructions_unique[3] = &Chip8::op_3xkk;
	this->intructions_unique[4] = &Chip8::op_4xkk;
	this->intructions_unique[5] = &Chip8::op_5xy0;
	this->intructions_unique[6] = &Chip8::op_6xkk;
	this->intructions_unique[7] = &Chip8::op_7xkk;
	this->intructions_unique[8] = &Chip8::table_8_func;
	this->intructions_unique[9] = &Chip8::op_9xy0;
	this->intructions_unique[0xA] = &Chip8::op_Annn;
	this->intructions_unique[0xB] = &Chip8::op_Bnnn;
	this->intructions_unique[0xC] = &Chip8::op_Cxkk;
	this->intructions_unique[0xD] = &Chip8::op_Dxyn;
	this->intructions_unique[0xE] = &Chip8::table_E_func;
	this->intructions_unique[0xF] = &Chip8::table_F_func;

	this->intructions_0[0] = &Chip8::op_00e0;
	this->intructions_0[0xE] = &Chip8::op_00ee;

	this->intructions_8[0] = &Chip8::op_8xy0;
	this->intructions_8[1] = &Chip8::op_8xy1;
	this->intructions_8[2] = &Chip8::op_8xy2;
	this->intructions_8[3] = &Chip8::op_8xy3;
	this->intructions_8[4] = &Chip8::op_8xy4;
	this->intructions_8[5] = &Chip8::op_8xy5;
	this->intructions_8[6] = &Chip8::op_8xy6;
	this->intructions_8[7] = &Chip8::op_8xy7;
	this->intructions_8[0xE] = &Chip8::op_8xyE;

	this->intructions_E[0xE] = &Chip8::op_Ex9E;
	this->intructions_E[1] = &Chip8::op_ExA1;

	this->intructions_F[0x07] = &Chip8::op_Fx07;
	this->intructions_F[0x0A] = &Chip8::op_Fx0A;
	this->intructions_F[0x15] = &Chip8::op_Fx15;
	this->intructions_F[0x18] = &Chip8::op_Fx18;
	this->intructions_F[0x1E] = &Chip8::op_Fx1E;
	this->intructions_F[0x29] = &Chip8::op_Fx29;
	this->intructions_F[0x33] = &Chip8::op_Fx33;
	this->intructions_F[0x55] = &Chip8::op_Fx55;
	this->intructions_F[0x65] = &Chip8::op_Fx65;
}


void Chip8::load_rom(std::string file_path) {

	std::ifstream rom;

	rom.open(file_path, std::ios::binary | std::ios::ate);

	if (rom.is_open()) {

		std::streampos file_size = rom.tellg();

		this->program_end_address = PROGRAM_START_ADDRESS + file_size + 1;

		char* buffer = new char[(int)file_size];

		rom.seekg(0, std::ios::beg);
		rom.read(buffer, file_size);
		rom.close();

		for (int i = 0; i < file_size; i++) {
			this->memory[PROGRAM_START_ADDRESS + i] = buffer[i];
		}

		delete[] buffer;
	}
}


void Chip8::op_00e0() {

	memset(this->screen, 0, sizeof(this->screen));
}


void Chip8::op_00ee() {

	this->sp--;
	this->pc = this->stack[this->sp];
}


void Chip8::op_1nnn() {

	this->pc = this->opcode & 0x0FFF;
}


void Chip8::op_2nnn() {

	uint16_t address = this->opcode & 0x0FFF;

	this->stack[this->sp] = this->pc;
	this->sp++;
	this->pc = address;
}


void Chip8::op_3xkk() {

	uint8_t reg = (this->opcode & 0x0F00) >> 8;
	uint8_t kk_value = this->opcode & 0x00FF;

	if (this->v_registers[reg] == kk_value)
		this->pc += 2;
}


void Chip8::op_4xkk() {

	uint8_t reg = (this->opcode & 0x0F00) >> 8;
	uint8_t kk_value = this->opcode & 0x00FF;

	if (this->v_registers[reg] != kk_value)
		this->pc += 2;
}


void Chip8::op_5xy0() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;

	if (this->v_registers[x_reg] == this->v_registers[y_reg])
		this->pc += 2;
}


void Chip8::op_6xkk() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t kk_value = (this->opcode & 0x00FF);

	this->v_registers[x_reg] = kk_value;
}


void Chip8::op_7xkk() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t kk_value = (this->opcode & 0x00FF);

	this->v_registers[x_reg] += kk_value;
}


void Chip8::op_8xy0() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;

	this->v_registers[x_reg] = this->v_registers[y_reg];
}


void Chip8::op_8xy1() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;

	this->v_registers[x_reg] |= this->v_registers[y_reg];
}


void Chip8::op_8xy2() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;

	this->v_registers[x_reg] &= this->v_registers[y_reg];
}


void Chip8::op_8xy3() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;

	this->v_registers[x_reg] ^= this->v_registers[y_reg];
}


void Chip8::op_8xy4() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;

	uint16_t reg_sum = this->v_registers[x_reg] + this->v_registers[y_reg];

	if (reg_sum > 255)
		this->v_registers[0xF] = 1;
	else
		this->v_registers[0xF] = 0;

	this->v_registers[x_reg] = reg_sum & 0xFF;
}


void Chip8::op_8xy5() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;

	uint8_t reg_diff = this->v_registers[x_reg] - this->v_registers[y_reg];

	if (this->v_registers[x_reg] > this->v_registers[y_reg])
		this->v_registers[0xF] = 1;
	else
		this->v_registers[0xF] = 0;

	this->v_registers[x_reg] -= this->v_registers[y_reg];
}


void Chip8::op_8xy6() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	this->v_registers[0xF] = this->v_registers[x_reg] & 0x1;

	this->v_registers[x_reg] >>= 1;
}


void Chip8::op_8xy7() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;

	if (this->v_registers[y_reg] > this->v_registers[x_reg])
		this->v_registers[0xF] = 1;
	else
		this->v_registers[0xF] = 0;

	this->v_registers[x_reg] = this->v_registers[y_reg] - this->v_registers[x_reg];
}


void Chip8::op_8xyE() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	this->v_registers[0xF] = (this->v_registers[x_reg] & 0x80) >> 7;

	this->v_registers[x_reg] <<= 1;
}


void Chip8::op_9xy0() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;

	if (this->v_registers[x_reg] != this->v_registers[y_reg])
		this->pc += 2;
}


void Chip8::op_Annn() {

	uint16_t address = this->opcode & 0x0FFF;

	this->index = address;
}


void Chip8::op_Bnnn() {

	uint16_t address = this->opcode & 0x0FFF;

	this->pc = v_registers[0x0] + address;
}


void Chip8::op_Cxkk() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t kk_value = (this->opcode & 0x00FF);
	uint8_t random_byte = rand() % 255;

	this->v_registers[x_reg] = kk_value & random_byte;
}


void Chip8::op_Dxyn() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t y_reg = (this->opcode & 0x00F0) >> 4;
	uint8_t bytes_count = this->opcode & 0x000F;

	uint8_t x_coord = this->v_registers[x_reg] % 64;
	uint8_t y_coord = this->v_registers[y_reg] % 32;

	for (int row = 0; row < bytes_count; row++) {

		uint8_t byte = memory[this->index + row];
		uint8_t mask = 0x80;

		for (int pixel_in_row = 0; pixel_in_row < 8; pixel_in_row++) {

			uint8_t sprite_pixel_value = (byte & mask) >> (7 - pixel_in_row);

			uint32_t* screen_pixel_value = &this->screen[((y_coord + row) * 64) + x_coord + pixel_in_row];

			if (sprite_pixel_value) {

				if (*screen_pixel_value)
					v_registers[0xF] = 1;
				else
					v_registers[0xF] = 0;

				*screen_pixel_value ^= 0xFFFFFFFF;
			}			

			mask /= 2;
		}
	}
}


void Chip8::op_Ex9E() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	if (this->keypad[v_registers[x_reg]] != 0)
		this->pc += 2;
}


void Chip8::op_ExA1() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	if (this->keypad[v_registers[x_reg]] == 0)
		this->pc += 2;
}


void Chip8::op_Fx07() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	this->v_registers[x_reg] = this->delay_timer;
}


void Chip8::op_Fx0A() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	for (uint8_t key : this->keypad) {

		if (key != 0) {
			this->v_registers[x_reg] = key;
			break;
		}
		else
			this->pc -= 2;
	}
}


void Chip8::op_Fx15() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	this->delay_timer = this->v_registers[x_reg];
}


void Chip8::op_Fx18() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	this->sound_timer = this->v_registers[x_reg];
}


void Chip8::op_Fx1E() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	this->index += this->v_registers[x_reg];
}


void Chip8::op_Fx29() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	this->index = 0x50 + (5 * this->v_registers[x_reg]);
}


void Chip8::op_Fx33() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;
	uint8_t value = this->v_registers[x_reg];

	this->memory[this->index + 2] = value % 10;

	value /= 10;

	this->memory[this->index + 1] = value % 10;

	value /= 10;

	this->memory[this->index] = value;
}


void Chip8::op_Fx55() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	for (int i = 0; i <= x_reg; i++) {
		this->memory[this->index + i] = this->v_registers[i];
	}
}


void Chip8::op_Fx65() {

	uint8_t x_reg = (this->opcode & 0x0F00) >> 8;

	for (int i = 0; i <= x_reg; i++) {
		this->v_registers[i] = this->memory[this->index + i];
	}
}


void Chip8::table_0_func() {
	(this->*(intructions_0[this->opcode & 0x000F]))();
}


void Chip8::table_8_func() {
	(this->*(intructions_8[this->opcode & 0x000F]))();
}


void Chip8::table_E_func() {
	(this->*(intructions_E[this->opcode & 0x000F]))();
}


void Chip8::table_F_func() {
	(this->*(intructions_F[this->opcode & 0x00FF]))();
}


void Chip8::cpu_cycle() {

	this->opcode = this->memory[this->pc];
	this->opcode = (this->opcode << 8) | this->memory[this->pc + 1];

	this->pc += 2;

	uint8_t op_type = (this->opcode & 0xF000) >> 12;

	(this->*(intructions_unique[op_type]))();

	if (this->delay_timer > 0)
		this->delay_timer--;

	if (this->sound_timer > 0)
		this->sound_timer--;
}