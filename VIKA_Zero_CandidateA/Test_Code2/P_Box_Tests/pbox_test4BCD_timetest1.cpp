/*
    This is part of the VIKA_Zero_CandidateA encryption system tests, the source code file for testing the encryption speed of encryption setups 4B, 4C, & 4D
    Copyright (C) 2025 stringzzz, Ghostwarez Co.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

using namespace std::chrono;

const unsigned int NTEST_BLOCKS = 10000000; //FLAG

struct block {
    unsigned int plaintext[16];
};

void bytesToBits(unsigned int input[], int input_len, unsigned int bits[]);
void genBox2(unsigned int out_box[], int box_size, unsigned int nks_bits, unsigned int shuffles);
void sBox(unsigned int input[], int input_len, unsigned int sbox[]);
void pBox(unsigned int output[], unsigned int input[], int input_len, unsigned int pbox[]);
void bytesToNybbles(unsigned int input[], int input_len, unsigned int nybbles[]);
void nybblesToBytes(unsigned int output[], int output_len, unsigned int nybbles[]);
void encrypt_test4B(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles);
void encrypt_test4C(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles);
void encrypt_test4D(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles, unsigned int xor_bytes[]);

std::random_device random_source;

int main(int argc, char *argv[]) {

    unsigned int n_sbox_shuffles[] = { 32 };
    unsigned int n_ks_bits_per_box[] = { 8192 };
    unsigned int n_pbox_shuffles[] = { 256 };
    unsigned int cycles_per_pbox[] = { 2 };
    unsigned int cycles[] = { 16 };

    struct block* test_blocks4B = new struct block[NTEST_BLOCKS];
    struct block* test_blocks4C = new struct block[NTEST_BLOCKS];
    struct block* test_blocks4D = new struct block[NTEST_BLOCKS];

    for(unsigned int n_blocks = 0; n_blocks < NTEST_BLOCKS; n_blocks++) {
        for(unsigned int n_bytes = 0; n_bytes < 16; n_bytes++) {
            unsigned int temp = random_source() % 256;
            test_blocks4B[n_blocks].plaintext[n_bytes] = temp;
            test_blocks4C[n_blocks].plaintext[n_bytes] = temp;
            test_blocks4D[n_blocks].plaintext[n_bytes] = temp;
        }
    }

    for(unsigned int n_shuffles = 0; n_shuffles < 1; n_shuffles++) {
        for(unsigned int n_cycles_per_pbox = 0; n_cycles_per_pbox < 1; n_cycles_per_pbox++) {
            for(unsigned int n_cycles = 0; n_cycles < 1; n_cycles++) {

                unsigned int sbox[256];
                genBox2(sbox, 256, n_ks_bits_per_box[n_shuffles], n_sbox_shuffles[n_shuffles]);
                unsigned int pbox[32];
                genBox2(pbox, 32, n_ks_bits_per_box[n_shuffles], n_pbox_shuffles[n_shuffles]);
                //Gen XOR bytes
                unsigned int xor_bytes[256];
                for(unsigned int n = 0; n < 256; n++) {
                    xor_bytes[n] = random_source() % 256;
                }

                //4B
                std::cout << "Testing 4B\n";
                auto start4B = high_resolution_clock::now();
                for(unsigned int n_blocks = 0; n_blocks < NTEST_BLOCKS; n_blocks++) {
                    encrypt_test4B(test_blocks4B[n_blocks].plaintext, sbox, pbox, cycles_per_pbox[n_cycles_per_pbox], cycles[n_cycles]);
                }
                auto stop4B = high_resolution_clock::now();
                auto duration4B = duration_cast<microseconds>(stop4B - start4B);
                delete[] test_blocks4B;

                //FLAG
                std::ofstream pbox_file("pbox_test4BCD_timetest_10000000.txt", std::ios::app);
                if (pbox_file.is_open()) {
                    
                    pbox_file <<
                    "4B:\n" <<
                    "Total operations per block: 5993\n" <<
                    duration4B.count() << " microseconds\n" <<
                    "Time per 1 block: " << (double)duration4B.count() / (double)NTEST_BLOCKS <<
                    " microseconds\n\n";

                    pbox_file.close();
                }

                //4C
                std::cout << "Testing 4C\n";
                auto start4C = high_resolution_clock::now();
                for(unsigned int n_blocks = 0; n_blocks < NTEST_BLOCKS; n_blocks++) {
                    encrypt_test4C(test_blocks4C[n_blocks].plaintext, sbox, pbox, cycles_per_pbox[n_cycles_per_pbox], cycles[n_cycles]);
                }
                auto stop4C = high_resolution_clock::now();
                auto duration4C = duration_cast<microseconds>(stop4C - start4C);
                delete[] test_blocks4C;

                //FLAG
                std::ofstream pbox_file2("pbox_test4BCD_timetest_10000000.txt", std::ios::app);
                if (pbox_file2.is_open()) {
                    
                    pbox_file2 <<
                    "4C:\n" <<
                    "4C: Operations per block: 4309\n" <<
                    duration4C.count() << " microseconds\n" <<
                    "Time per 1 block: " << (double)duration4C.count() / (double)NTEST_BLOCKS <<
                    " microseconds\n\n";

                    pbox_file2.close();
                }

                //4D
                std::cout << "Testing 4D\n";
                auto start4D = high_resolution_clock::now();
                for(unsigned int n_blocks = 0; n_blocks < NTEST_BLOCKS; n_blocks++) {
                    encrypt_test4D(test_blocks4D[n_blocks].plaintext, sbox, pbox, cycles_per_pbox[n_cycles_per_pbox], cycles[n_cycles], xor_bytes);
                }
                auto stop4D = high_resolution_clock::now();
                auto duration4D = duration_cast<microseconds>(stop4D - start4D);
                delete[] test_blocks4D;

                //FLAG
                std::ofstream pbox_file3("pbox_test4BCD_timetest_10000000.txt", std::ios::app);
                if (pbox_file3.is_open()) {
                    
                    pbox_file3 <<
                    "4D:\n" <<
                    "4D: Operations per block: 6117\n" <<
                    duration4D.count() << " microseconds\n" <<
                    "Time per 1 block: " << (double)duration4D.count() / (double)NTEST_BLOCKS <<
                    " microseconds\n\n";

                    pbox_file3.close();
                }

            }
        }
    }

    return 0;
}

void bytesToBits(unsigned int input[], int input_len, unsigned int bits[]) {
    //bits must be size input_len * 8!
    int i = 0;
    for (unsigned int n = 0; n < input_len; n++) {
        for (int n2 = 1; n2 <= 8; n2++) {
            bits[i] = ((unsigned int)input[n] >> (8 - n2)) & 1;
            i++;
        }
    }
}

void genBox2(unsigned int out_box[], int box_size, unsigned int nks_bits, unsigned int shuffles) {
    //Setup out_box to initial values in order from 0 to box_size
    //shuffles should be greater for greater box_size, actual amount is up to the user
    for(int i = 0; i < box_size; i++) {
        out_box[i] = i;
    }

    unsigned int ks_bits[nks_bits];
    for(unsigned int n = 0; n < nks_bits; n++) {
        ks_bits[n] = random_source() % 2;
    }

    //Shuffle the box by int shuffles amount of times
    //Uses rand() to choose from beginning (left) or end (right) to grab from
    unsigned int ks_n = 0;
    for(int shuffle_count = 0; shuffle_count < shuffles; shuffle_count++) {
        int temp_box[box_size];
        //Copy out_box to temp_box
        for(int i = 0; i < box_size; i++) {
            temp_box[i] = out_box[i];
        }
        int left = 0, right = box_size - 1;
        for(int i = 0; i < box_size; i++) {
            if (ks_bits[ks_n] == 0) {
                out_box[i] = temp_box[left];
                left++;
            } else {
                out_box[i] = temp_box[right];
                right--;
            }
            ks_n++;
        } 
    }
}

void sBox(unsigned int input[], int input_len, unsigned int sbox[]) {
        for(int i = 0; i < input_len; i++) {
		input[i] = sbox[input[i]];
	}
}

void pBox(unsigned int output[], unsigned int input[], int input_len, unsigned int pbox[]) {
    for(int i = 0; i < input_len; i++) {
        output[pbox[i]] = input[i];
    }
}

void bytesToNybbles(unsigned int input[], int input_len, unsigned int nybbles[]) {
    //nybbles must be size input_len * 2!
    int n = 0;
    for(int i = 0; i < input_len; i++) {
        nybbles[n] = input[i] >> 4; //Left nybble
        n++;
        nybbles[n] = input[i] & 15; //Right nybble
        n++;
    }
}

void nybblesToBytes(unsigned int output[], int output_len, unsigned int nybbles[]) {
    //nybbles must be size output_len * 2!
    int n = 0;
    for(int i = 0; i < output_len; i++) {
        output[i] = ((unsigned int)nybbles[n] << 4) ^ (nybbles[n+1]);
        n += 2;
    }
}

void encrypt_test4B(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles) {
    for(unsigned int cycles = 1; cycles <= ncycles; cycles++) { //1 + 2 * 16 = 33
        sBox(block, 16, sbox); //(1 + 2 * 16 + 4 * 16) * 16 cycles = 1552
        if (cycles % pbox_cycles == 0) { //2 * 16 cycles = 32
            unsigned int temp_nybbles_input[32];
            bytesToNybbles(block, 16, temp_nybbles_input); //(10 * 16 + 1 + 2 * 16) * 8 cycles = 1544
            unsigned int temp_nybbles_output[32];
            pBox(temp_nybbles_output, temp_nybbles_input, 32, pbox); //(1 + 2 * 32 + 4 * 32) * 8 cycles = 1544
            nybblesToBytes(block, 16, temp_nybbles_output); //(1 + 2 * 16 + 8 * 16) * 8 cycles = 1288
            //Total operations per block: 5993
        }
    }
}

void encrypt_test4C(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles) {
    for(unsigned int cycles = 1; cycles <= ncycles; cycles++) { //1 + 2 * 16 = 33
        sBox(block, 16, sbox); //(1 + 2 * 16 + 4 * 16) * 16 cycles = 1552
        if (cycles % pbox_cycles == 0 && cycles % 4 != 0) { //5 * 16 = 80
            unsigned int temp_nybbles_input[32];
            bytesToNybbles(block, 16, temp_nybbles_input); //(10 * 16 + 1 + 2 * 16) * 4 cycles = 772
            unsigned int temp_nybbles_output[32];
            pBox(temp_nybbles_output, temp_nybbles_input, 32, pbox); //(1 + 2 * 32 + 4 * 32) * 4 cycles = 772
            nybblesToBytes(block, 16, temp_nybbles_output);//(1 + 2 * 16 + 8 * 16) * 4 cycles = 644
        } else if (cycles % 4 == 0) { // 2 * 4 = 8
            for(unsigned int i = 1; i < 15; i++) { //(1 + 2 * 14) * 4 cycles = 116
                if (i == 14) { //(1 * 14) * 4 cycles = 56
                    block[0] ^= block[15]; //(4 * 1) * 4 = 16
                } else {
                    block[i + 1] ^= block[i]; //(5 * 13) * 4 = 260
                }
            }
        }
        //Operations per block: 4309
    }
}

void encrypt_test4D(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles, unsigned int xor_bytes[]) {
    unsigned int nxor_bytes = 0; // = 1
    for(unsigned int cycles = 1; cycles <= ncycles; cycles++) { //1 + 2 * 16 = 33
        sBox(block, 16, sbox); //(1 + 2 * 16 + 4 * 16) * 16 cycles = 1552
        if (cycles % pbox_cycles == 0 && cycles % 4 != 0) {  //5 * 16 = 80
            unsigned int temp_nybbles_input[32];
            bytesToNybbles(block, 16, temp_nybbles_input); //(10 * 16 + 1 + 2 * 16) * 4 cycles = 772
            unsigned int temp_nybbles_output[32];
            pBox(temp_nybbles_output, temp_nybbles_input, 32, pbox); //(1 + 2 * 32 + 4 * 32) * 4 cycles = 772
            nybblesToBytes(block, 16, temp_nybbles_output); //(1 + 2 * 16 + 7 * 16) * 4 cycles = 644
        } else if (cycles % 4 == 0) { // 2 * 4 = 8
            for(unsigned int i = 1; i < 15; i++) { //(1 + 2 * 14) * 4 cycles = 116
                if (i == 14) { //(1 * 14) * 4 cycles = 56
                    block[0] ^= block[15]; //(4 * 1) * 4 = 16
                } else {
                    block[i + 1] ^= block[i]; //(5 * 13) * 4 = 260
                }
            }
        }
        //16 bytes per cycle, 16 cycles = 256 xor bytes
        for(unsigned int n = 0; n < 16; n++, nxor_bytes++) { //(1 + 3 * 16) * 16 cycles = 784
            block[n] ^= xor_bytes[nxor_bytes]; //4 * 16 * 16 = 1024
        }

        //Operations per block: 6117
    }
}