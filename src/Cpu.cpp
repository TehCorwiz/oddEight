//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include <ctime>

#include "Cpu.h"

Cpu::Cpu(Memory *memory, IO *io, Display *display) {
    this->_memory = memory;
    this->_io = io;
    this->_display = display;

    std::cout << "CPU initialized." << std::endl;
}

void Cpu::reset() {
    this->_I = 0;
    this->_PC = Cpu::ExecutionStartAddress;
    this->_SP = 0;

    for (uint8_t &v_register: this->_V) { v_register = 0; }

    this->_delayTimer = 0;
    this->_soundTimer = 0;

    this->_cycleCount = 0;
    this->_error = false;

    std::cout << "CPU reset." << std::endl;
}

void Cpu::_executeOpcode(uint16_t opcode) {
    //v_register - So, despite not all opcodes using this nibble in this way it doesn't impose a performance
    // penalty in our use scenario. Enough DO use it that it's not wasted and it prevents simple bit-shifting errors.
    auto v_register = (uint8_t) ((opcode & 0x0F00) >> 8);

    switch (opcode & 0xF000) {
        case (0x0000):
            switch (opcode & 0x00FF) {
                case (0x0000):
                    //0nnn: Jump to a machine code routine at nnn. Not used in emulator.
                    // No machine code in memory space. Essentially NOP.
                    break;
                case (0x00E0):
                    //00E0: Clear the display.
                    this->_display->clear();
                    break;
                case (0x00EE):
                    //00EE: Return from a subroutine.
                    this->_PC = this->_memory->popStack((uint8_t) (this->_SP >> 8));
                    this->_PC = this->_memory->popStack((uint8_t) (this->_SP & 0x00FF));
                    this->_SP--;
                    break;
                default:
                    std::cout << "ERROR on cycle " << this->_cycleCount << " Unknown opcode: 0x"
                              << std::hex << std::uppercase << opcode << std::endl;
                    break;
            }
            break;
        case (0x1000):
            //1nnn: Jump to location nnn.
            this->_PC = (uint16_t) (opcode & 0x0FFF);
            break;
        case (0x2000):
            //2nnn: Call subroutine at nnn
            this->_SP++;
            this->_memory->pushStack(this->_SP, (uint16_t) ((opcode & 0x0F00) >> 8)); // Most sig byte
            this->_memory->pushStack(this->_SP, (uint16_t) (opcode & 0x00FF)); // Least sig byte
            break;
        case (0x3000):
            //3xkk: Skip next instruction if Vx = kk.

//            std::cout << "V" << v_register << ": " << std::hex << std::uppercase << this->_V[v_register] << std::endl;
//            std::cout << "kk: " << std::hex << std::uppercase << (uint8_t) (opcode & 0x00FF) << std::endl;

            if (this->_V[v_register] == (uint8_t) (opcode & 0x00FF)) this->_PC += 2;
            break;
        case (0x6000):
            //6xkk: Set Vx = kk.
            this->_V[v_register] = (uint8_t) ((opcode & 0x00FF));
            break;
        case (0x7000):
            //7xkk: Set Vx = Vx + kk.
            // TODO: carry flag?
            this->_V[v_register] += (uint8_t) (opcode & 0x00FF);
            break;
        case (0x8000): {
            auto x = (uint8_t) ((opcode & 0x0F00) >> 8);
            auto y = (uint8_t) ((opcode & 0x00F0) >> 4);

            switch (opcode & 0x000F) {
                case (0x0000):
                    //8xy0: Set Vx = Vy.
                    this->_V[x] = this->_V[y];

                    break;
                case (0x0001):
                    //8xy1: Set Vx = Vx OR Vy.
                    this->_V[x] |= this->_V[y];

                    break;
                case (0x0002):
                    //8xy2: Set Vx = Set Vx = Vx AND Vy.
                    this->_V[x] &= this->_V[y];

                    break;
                case (0x0003):
                    //8xy3: Set Vx = Vx XOR Vy.
                    this->_V[x] ^= this->_V[y];

                    break;
                case (0x0004): {
                    //8xy4: Set Vx = Vx + Vy, set VF = carry.
                    auto new_vx = ((uint16_t) this->_V[x] ^ (uint16_t) this->_V[y]);
                    if (new_vx > 0xFF) this->_V[0xF] = 1;
                    this->_V[x] = (uint8_t) new_vx;
                }
                    break;
                case (0x0005):
                    //8xy5: Set Vx = Vx - Vy, set VF = NOT borrow.
                    if (this->_V[x] > this->_V[y]) this->_V[0xF] = 1;
                    this->_V[x] -= this->_V[y];

                    break;
                case (0x0006):
                    //8xy6: Set Vx = Vx SHR 1.
                    this->_V[0xF] = (uint8_t) (this->_V[x] & 1);
                    this->_V[x] = (uint8_t) (this->_V[x] >> 1); // Divide by 2

                    break;
                case (0x0007):
                    //8xy7: Set Vx = Vy - Vx, set VF = NOT borrow.
                    this->_V[0xF] = (uint8_t) (this->_V[y] > this->_V[x]);
                    this->_V[x] = this->_V[y] - this->_V[x];

                    break;
                case (0x000E):
                    //8xyE: Set Vx = Vx SHL 1.
                    this->_V[0xF] = (uint8_t) (this->_V[x] & 0x80);
                    this->_V[x] = (uint8_t) (this->_V[x] << 1); // Multiply by 2

                    break;
                default:
                    std::cout << "ERROR on cycle " << this->_cycleCount << " Unknown opcode: 0x"
                              << std::hex << std::uppercase << opcode << std::endl;
                    break;
            }
        }
            break; // End 0x8000 Opcodes
        case (0xA000):
            //Annn: Set I = nnn.
            this->_I = (uint16_t) (opcode & 0x0FFF);
            break;
        case (0xC000): {
            //Cxkk: Set Vx = random byte AND kk.

            std::srand((unsigned int) std::time(nullptr)); // use current time as seed for random generator
            // Get random number and truncate to less than 256
            // Compiler complains about limited randomness in std::rand() (Don't care)
            auto random_value = (uint8_t) (std::rand() & 0xFF);

            this->_V[v_register] = (uint8_t) (random_value & (opcode & 0x00FF));
        }
            break;
        case (0xD000): {
            //Dxyn: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
            const uint8_t x = this->_V[(opcode & 0x0F00) >> 8];
            const uint8_t y = this->_V[(opcode & 0x00F0) >> 4];
            const auto height = (uint8_t) (opcode & 0x000F);

            uint8_t sprite[height];
            for (uint8_t row = 0; row < height; row++) {
                sprite[row] = this->_memory->readByte(this->_I + row);
            }

            this->_V[0xF] = this->_display->drawSprite(sprite, height, x, y);
        }
            break;
        case (0xE000):
            //Ex9E: Skip next instruction if key with the value of Vx is pressed.
            if (this->_io->readKey(this->_V[(opcode & 0x0F00) >> 8])) this->_PC += 2;
            break;
        case (0xF000): {
            switch (opcode & 0x00FF) {
                case (0x0007):
                    //Fx07: Set Vx = delay timer value.
                    this->_V[v_register] = this->_delayTimer;
                    break;
                case (0x000A):
                    //Fx0A: Wait for a key press, store the value of the key in Vx.
                    // TODO: wait for interrupt? Poll _IO?
                    std::cout << "Unimplemented opcode: 0x" << std::hex << std::uppercase << opcode << std::endl;
                    break;
                case (0x15):
                    //Fx15: Set delay timer = Vx.
                    this->_delayTimer = this->_V[v_register];
                    break;
                case (0x18):
                    //Fx18: Set sound timer = Vx.
                    this->_soundTimer = this->_V[v_register];
                    break;
                case (0x1E):
                    //Fx1E: Set I = I + Vx.
                    this->_I = (this->_I + this->_V[v_register]);
                    break;
                case (0x29):
                    //Fx29: Set I = location of sprite for digit Vx.
                    // TODO: lookup text sprite.
                    std::cout << "Unimplemented opcode: 0x" << std::hex << std::uppercase << opcode << std::endl;
                    break;
                case (0x33):
                    //Fx33: Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    // Hundreds
                    this->_memory->writeByte((uint8_t) ((this->_V[v_register] / 100) % 10), this->_I);
                    // Tens
                    this->_memory->writeByte((uint8_t) ((this->_V[v_register] / 10) % 10), (uint16_t) (this->_I + 1));
                    // Ones
                    this->_memory->writeByte((uint8_t) ((this->_V[v_register] / 100) % 10), (uint16_t) (this->_I + 2));
                    break;
                case (0x55):
                    //Fx55: Store registers V0 through Vx in memory starting at location I.
                    for (uint8_t i = 0; i < v_register; i++) {
                        this->_memory->writeByte(this->_V[i], this->_I + i);
                    }
                    break;
                case (0x65):
                    //Fx65: Read registers V0 through Vx from memory starting at location I.
                    for (uint8_t i = 0; i < v_register; i++) {
                        this->_V[i] = this->_memory->readByte(this->_I + i);
                    }
                    break;
                default:
                    std::cout << "ERROR on cycle " << this->_cycleCount << " Unknown opcode: 0x"
                              << std::hex << std::uppercase << opcode << std::endl;
                    break;
            }
            break; // End 0xF000 opcodes
        }
        default:
            std::cout << "ERROR on cycle " << this->_cycleCount << " Unknown opcode: 0x"
                      << std::hex << std::uppercase << opcode << std::endl;
            this->_error = true;
            break;
    }

    this->_PC += 2;
}

const uint32_t Cpu::cycleCount() const {
    return this->_cycleCount;
}

void Cpu::runCycle() {
    // Read next opcode
    uint16_t opcode = this->_memory->readWord(_PC);

    std::cout << "Opcode 0x" << std::hex << std::uppercase << opcode << " on cycle " << this->_cycleCount << std::endl;

    // Execute opcode
    this->_executeOpcode(opcode);

    // Update timers
    if (this->_delayTimer > 0) --this->_delayTimer;
    if (this->_soundTimer > 0) --this->_soundTimer;

    // Update stats
    this->_cycleCount++;
}

const bool Cpu::error() const {
    return this->_error;
}
