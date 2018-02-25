//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include <ctime>
#include <sstream>

#include "Cpu.h"

/* Public */
Cpu::Cpu(Memory *memory, IO *io, Display *display) {
    this->_memory = memory;
    this->_io = io;
    this->_display = display;

    std::cout << "CPU initialized." << std::endl;
}

void Cpu::reset() {
    this->_I = 0;
    this->_PC = Cpu::executionStartAddress;
    this->_SP = 0;

    // Reset registers
    for (uint8_t &v_register: this->_V) { v_register = 0; }

    // Reset stack
    for (uint16_t &cell: this->_stack) { cell = 0; }

    this->_delayTimer = 0;
    this->_soundTimer = 0;

    this->_tickCount = 0;
    this->_error = false;

    std::cout << "CPU reset." << std::endl;
}

void Cpu::runTick() {
    // Read next opcode
    uint16_t opcode = this->_memory->readWord(_PC);

    // Execute opcode
    this->_executeOpcode(opcode);

    // Update timers
    if (this->_delayTimer > 0) --this->_delayTimer;
    if (this->_soundTimer > 0) --this->_soundTimer;

    // Update stats
    this->_tickCount++;
}

const bool Cpu::error() const { return this->_error; }

const uint32_t Cpu::tickCount() const { return this->_tickCount; }

const uint16_t Cpu::I() const { return this->_I; }

const uint16_t Cpu::PC() const { return this->_PC; }

const uint16_t Cpu::SP() const { return this->_SP; }

const uint8_t *Cpu::V() const { return this->_V; }

const uint16_t *Cpu::stack() const { return this->_stack; }

const uint8_t Cpu::delayTimer() const { return this->_delayTimer; }

const uint8_t Cpu::soundTimer() const { return this->_soundTimer; }

/* Private */
void Cpu::_executeOpcode(uint16_t opcode) {
    //So, despite not all opcodes using things the same way, this gives us a common naming convention to various
    // opcode components.
    const auto x = (uint8_t) ((opcode & 0x0F00) >> 8);
    const auto y = (uint8_t) ((opcode & 0x00F0) >> 4);
    const auto n = (uint8_t) (opcode & 0x000F);
    const auto kk = (uint8_t) (opcode & 0x00FF);
    const auto nnn = (uint16_t) (opcode & 0x0FFF);

    switch (opcode & 0xF000) {
        case (0x0000):
            switch (kk) {
                case (0x0000):
                    //0nnn: Jump to a machine code routine at nnn. Not used in emulator.
                    // No machine code in memory space. Essentially NOP.
                    this->_disassembly[this->_PC] = "SYS addr: 0x" + _toHexString(nnn);
                    this->_PC += 2;

                    break;
                case (0x00E0):
                    //00E0: Clear the display.
                    this->_disassembly[this->_PC] = "CLS";
                    this->_display->clear();
                    this->_PC += 2;

                    break;
                case (0x00EE):
                    //00EE: Return from a subroutine.
                    this->_disassembly[this->_PC] = "RET";
                    this->_PC = this->_popStack();

                    break;
                default:
                    std::cout << "ERROR on tick " << this->_tickCount << " Unknown opcode: 0x"
                              << std::hex << std::uppercase << opcode << std::endl;
                    this->_error = true;

                    break;
            }

            break;
        case (0x1000):
            //1nnn: Jump to location nnn.
            this->_disassembly[this->_PC] = "JP addr: 0x" + _toHexString(nnn);
            this->_PC = nnn;

            break;
        case (0x2000):
            //2nnn: Call subroutine at nnn
            this->_disassembly[this->_PC] = "CALL addr: 0x" + _toHexString(nnn);
            this->_pushStack(this->_PC);
            this->_PC = nnn;

            break;
        case (0x3000):
            //3xkk: Skip next instruction if Vx = kk.
            this->_disassembly[this->_PC] = "SE Vx: " + std::to_string(x) + ", byte: 0x" + _toHexString(kk);
            if (this->_V[x] == kk) this->_PC += 2; // Overflow check below.
            this->_PC += 2;

            break;
        case (0x4000):
            //4xkk: Skip next instruction if Vx != kk.
            this->_disassembly[this->_PC] = "SNE Vx: " + std::to_string(x) + ", byte: 0x" + _toHexString(kk);
            if (this->_V[x] != kk) this->_PC += 2; // Overflow check below.
            this->_PC += 2;

            break;
        case (0x5000):
            //5xy0: Skip next instruction if Vx = Vy.
            this->_disassembly[this->_PC] = "SE Vx: " + std::to_string(x) + ", Vy: 0x" + std::to_string(y);
            if (this->_V[x] == this->_V[y]) this->_PC += 2; // Overflow check below.
            this->_PC += 2;

            break;
        case (0x6000):
            //6xkk: Set Vx = kk.
            this->_disassembly[this->_PC] = "LD Vx: " + std::to_string(x) + ", byte: 0x" + _toHexString(kk);
            this->_V[x] = kk;
            this->_PC += 2;

            break;
        case (0x7000):
            //7xkk: Set Vx = Vx + kk.
            this->_disassembly[this->_PC] = "ADD Vx: " + std::to_string(x) + ", byte: 0x" + _toHexString(kk);
            this->_V[x] += kk;
            this->_PC += 2;

            break;
        case (0x8000):
            switch (n) {
                case (0x0000):
                    //8xy0: Set Vx = Vy.
                    this->_disassembly[this->_PC] = "LD Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
                    this->_V[x] = this->_V[y];

                    break;
                case (0x0001):
                    //8xy1: Set Vx = Vx OR Vy.
                    this->_disassembly[this->_PC] = "OR Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
                    this->_V[x] |= this->_V[y];

                    break;
                case (0x0002):
                    //8xy2: Set Vx = Set Vx = Vx AND Vy.
                    this->_disassembly[this->_PC] = "AND Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
                    this->_V[x] &= this->_V[y];

                    break;
                case (0x0003):
                    //8xy3: Set Vx = Vx XOR Vy.
                    this->_disassembly[this->_PC] = "XOR Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
                    this->_V[x] ^= this->_V[y];

                    break;
                case (0x0004): {
                    //8xy4: Set Vx = Vx + Vy, set VF = carry.
                    this->_disassembly[this->_PC] = "OR Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
                    auto new_vx = ((uint16_t) this->_V[x] ^ (uint16_t) this->_V[y]);
                    if (new_vx > 0xFF) this->_V[0xF] = 1;
                    this->_V[x] = (uint8_t) new_vx;
                }

                    break;
                case (0x0005):
                    //8xy5: Set Vx = Vx - Vy, set VF = NOT borrow.
                    this->_disassembly[this->_PC] = "SUB Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
                    if (this->_V[x] > this->_V[y]) this->_V[0xF] = 1;
                    this->_V[x] -= this->_V[y];

                    break;
                case (0x0006):
                    //8xy6: Set Vx = Vx SHR 1.
                    this->_disassembly[this->_PC] = "SHR Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
                    this->_V[0xF] = (uint8_t) (this->_V[x] & 1);
                    this->_V[x] = (uint8_t) (this->_V[x] >> 1); // Divide by 2

                    break;
                case (0x0007):
                    //8xy7: Set Vx = Vy - Vx, set VF = NOT borrow.
                    this->_disassembly[this->_PC] = "SUBN Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
                    this->_V[0xF] = (uint8_t) (this->_V[y] > this->_V[x]);
                    this->_V[x] = this->_V[y] - this->_V[x];

                    break;
                case (0x000E):
                    //8xyE: Set Vx = Vx SHL 1.
                    this->_disassembly[this->_PC] = "SHL Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
                    this->_V[0xF] = (uint8_t) (this->_V[x] & 0x80);
                    this->_V[x] = (uint8_t) (this->_V[x] << 1); // Multiply by 2

                    break;
                default:
                    std::cout << "ERROR on tick " << this->_tickCount << " Unknown opcode: 0x"
                              << std::hex << std::uppercase << opcode << std::endl;
                    this->_error = true;

                    break;
            }

            this->_PC += 2;

            break; // End 0x8000 Opcodes
        case (0x9000):
            //9xy0: Skip next instruction if Vx != Vy.
            this->_disassembly[this->_PC] = "SNE Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y);
            if (this->_V[x] != this->_V[y]) this->_PC += 2; // Overflow check below.
            this->_PC += 2;

            break;
        case (0xA000):
            //Annn: Set I = nnn.
            this->_disassembly[this->_PC] = "LD I addr: 0x" + _toHexString(nnn);
            this->_I = nnn;
            this->_PC += 2;

            break;
        case (0xB000):
            //Bnnn: Jump to location nnn + V0.
            this->_disassembly[this->_PC] = "JP V0 addr: 0x" + _toHexString(nnn);
            this->_PC = nnn + this->_V[0];

            break;
        case (0xC000): {
            //Cxkk: Set Vx = random byte AND kk.
            this->_disassembly[this->_PC] = "RND Vx: " + std::to_string(x) + ", byte: 0x" + _toHexString(kk);
            std::srand((unsigned int) std::time(nullptr)); // use current time as seed for random generator
            // Get random number and truncate to less than 256
            // Compiler complains about limited randomness in std::rand() (Don't care)
            auto random_value = (uint8_t) (std::rand() & 0xFF);

            this->_V[x] = (random_value & kk);
        }
            this->_PC += 2;

            break;
        case (0xD000): {
            //Dxyn: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
            this->_disassembly[this->_PC] =
                    "DRW Vx: " + std::to_string(x) + ", Vy: " + std::to_string(y) + ", nibble: 0x" + _toHexString(n);
            uint8_t sprite[n];
            for (uint8_t row = 0; row < n; row++) {
                sprite[row] = this->_memory->readByte(this->_I + row);
            }

            this->_V[0xF] = this->_display->drawSprite(sprite, n, this->_V[x], this->_V[y]);
        }
            this->_PC += 2;

            break;
        case (0xE000):
            switch (kk) {
                case (0x009E):
                    //Ex9E: Skip next instruction if key with the value of Vx is pressed.
                    this->_disassembly[this->_PC] = "SKP Vx: " + std::to_string(x);
                    if (this->_io->readKey(this->_V[x])) this->_PC += 2;

                    break;
                case (0x00A1):
                    //ExA1: Skip next instruction if key with the value of Vx is not pressed.
                    this->_disassembly[this->_PC] = "SKNP Vx: " + std::to_string(x);
                    if (!this->_io->readKey(this->_V[x])) this->_PC += 2;

                    break;
                default:
                    std::cout << "ERROR on tick " << this->_tickCount << " Unknown opcode: 0x"
                              << std::hex << std::uppercase << opcode << std::endl;
                    this->_error = true;

                    break;
            }
            this->_PC += 2;

            break;
        case (0xF000): {
            switch (kk) {
                case (0x07):
                    //Fx07: Set Vx = delay timer value.
                    this->_disassembly[this->_PC] =
                            "LD Vx: " + std::to_string(x) + ", DT";
                    this->_V[x] = this->_delayTimer;

                    break;
                case (0x0A):
                    //Fx0A: Wait for a key press, store the value of the key in Vx.
                    this->_disassembly[this->_PC] = "LD Vx: " + std::to_string(x) + ", K";
                    // TODO: wait for interrupt? Poll _IO?
                    std::cout << "Unimplemented opcode: 0x" << std::hex << std::uppercase << opcode << std::endl;

                    break;
                case (0x15):
                    //Fx15: Set delay timer = Vx.
                    this->_disassembly[this->_PC] = "LD DT, Vx: " + std::to_string(x);
                    this->_delayTimer = this->_V[x];

                    break;
                case (0x18):
                    //Fx18: Set sound timer = Vx.
                    this->_disassembly[this->_PC] = "LD ST, Vx: " + std::to_string(x);
                    this->_soundTimer = this->_V[x];

                    break;
                case (0x1E):
                    //Fx1E: Set I = I + Vx.
                    this->_disassembly[this->_PC] = "ADD I, Vx: " + std::to_string(x);
                    this->_I = (this->_I + this->_V[x]);

                    break;
                case (0x29):
                    //Fx29: Set I = location of sprite for digit Vx.
                    this->_disassembly[this->_PC] = "LD F, Vx: " + std::to_string(x);
                    this->_I = (Memory::fontsetStartaddress + (this->_V[x] * Memory::fontHeight));

                    break;
                case (0x33):
                    //Fx33: Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    this->_disassembly[this->_PC] = "LD B, Vx: " + std::to_string(x);
                    // Hundreds
                    this->_memory->writeByte((uint8_t) ((this->_V[x] / 100) % 10), this->_I);
                    // Tens
                    this->_memory->writeByte((uint8_t) ((this->_V[x] / 10) % 10), (uint16_t) (this->_I + 1));
                    // Ones
                    this->_memory->writeByte((uint8_t) ((this->_V[x] / 100) % 10), (uint16_t) (this->_I + 2));

                    break;
                case (0x55):
                    //Fx55: Store registers V0 through Vx in memory starting at location I.
                    this->_disassembly[this->_PC] = "LD [I], Vx: " + std::to_string(x);
                    for (uint8_t i = 0; i < x; i++) {
                        this->_memory->writeByte(this->_V[i], this->_I + i);
                    }

                    break;
                case (0x65):
                    //Fx65: Read registers V0 through Vx from memory starting at location I.
                    this->_disassembly[this->_PC] = "LD Vx: " + std::to_string(x) + ", [I]";
                    for (uint8_t i = 0; i < x; i++) {
                        this->_V[i] = this->_memory->readByte(this->_I + i);
                    }

                    break;
                default:
                    std::cout << "ERROR on tick " << this->_tickCount << " Unknown opcode: 0x"
                              << std::hex << std::uppercase << opcode << std::endl;
                    this->_error = true;

                    break;
            }
            this->_PC += 2;

            break; // End 0xF000 opcodes
        }
        default:
            std::cout << "ERROR on tick " << this->_tickCount << " Unknown opcode: 0x"
                      << std::hex << std::uppercase << opcode << std::endl;
            this->_error = true;

            break;
    }

    this->_PC &= 0x0FFF; // Address range is 4095 = 0x0FFF. This limits to valid memory range.
}

void Cpu::_pushStack(const uint16_t value) {
    this->_stack[this->_SP] = value;
    this->_SP++;
}

uint16_t Cpu::_popStack() {
    this->_SP--;
    return this->_stack[this->_SP];
}

std::string Cpu::_toHexString(uint16_t value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void Cpu::_dumpDisassembly() {
    for (uint16_t addr = 0; addr < Memory::memorySize; addr++) {
        if (!this->_disassembly[addr].empty()) {
            std::cout << "0x" << std::to_string(addr) << " ";
            std::cout << this->_disassembly[addr];
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}
