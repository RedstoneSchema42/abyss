// Compile: g++ -O2 -std=c++17 -o abyss abyss.cpp
#include <cstdio>
#include <cstdint>
#include <cstdlib>

uint8_t program[256];
uint16_t memory[256];
int pc = 0, sp = 255;
uint8_t rotor = 0;

inline uint8_t fetch() {
    uint8_t e = program[pc++];
    uint8_t op = (e - rotor) & 0x0F;
    rotor = (rotor + op + pc) & 0x0F;
    return op;
}

int main(int argc, char** argv) {
    if (argc < 2) return 1;
    FILE* f = fopen(argv[1], "rb");
    if (!f) return 1;

    static const char alphabet[] = "QWERTYUIOPASDFGH";
    int c;
    int bit = 0, val = 0, idx = 0;
    while ((c = fgetc(f)) != EOF) {
        if (c == '0' || c == '1') {
            val = (val << 1) | (c - '0');
            if (++bit == 8) {
                int k = -1;
                for (int i = 0; i < 16; ++i) {
                    if ((uint8_t)alphabet[i] == (uint8_t)val) { k = i; break; }
                }
                if (k < 0) {
                    fprintf(stderr, "invalid encoded char: 0x%02X\n", val);
                    fclose(f);
                    return 1;
                }
                if (idx >= 256) { fclose(f); return 1; }
                program[idx++] = (uint8_t)k;
                val = 0; bit = 0;
            }
        }
    }
    fclose(f);

    while (1) {
        uint8_t op = fetch();
        switch (op) {
        case 0x0: return 0; // HALT
        case 0x1: { uint8_t h = fetch(), l = fetch(); memory[sp--] = (h << 4) | l; break; }
        case 0x2: { uint16_t t = memory[sp + 1]; memory[sp--] = t; break; } // DUP (fix: was sp+2)
        case 0x3: { uint16_t t = memory[sp + 1]; memory[sp + 1] = memory[sp + 2]; memory[sp + 2] = t; break; } // SWAP
        case 0x4: sp++; break; // DROP
        case 0x5: { uint16_t b = memory[++sp], a = memory[++sp]; memory[sp--] = a + b; break; } // ADD
        case 0x6: { uint16_t b = memory[++sp], a = memory[++sp]; memory[sp--] = a - b; break; } // SUB
        case 0x7: { uint16_t b = memory[++sp], a = memory[++sp]; memory[sp--] = a * b; break; } // MUL
        case 0x8: { uint16_t b = memory[++sp], a = memory[++sp]; memory[sp--] = a / b; break; } // DIV
        case 0x9: pc = memory[++sp]; break; // JMP
        case 0xA: { uint16_t a = memory[++sp]; if (memory[++sp] == 0) pc = a; break; } // JZ
        case 0xB: { uint16_t a = memory[++sp], v = memory[++sp]; memory[a] = v; break; } // STORE
        case 0xC: { uint16_t a = memory[++sp]; memory[sp--] = memory[a]; break; } // LOAD
        case 0xD: {
            uint16_t sel = memory[++sp];
            if (sel == 1) putchar(memory[++sp]);
            else if (sel == 0) memory[sp--] = getchar();
            break;
        }
        }
    }
    return 0;
}