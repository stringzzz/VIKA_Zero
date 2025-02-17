/*
    This is part of the VIKA_Zero_CandidateA encryption system tests, the source code file for testing the keygenerator 1B
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

const unsigned int NTEST_KEYS = 1000000; //FLAG
const unsigned int ks_blocks = 144;
const unsigned int box_bytes = 1024;
const unsigned int inner_ks_blocks = 289;

void bytesToBits(unsigned int input[], int input_len, unsigned int bits[]);
void genBox3(unsigned int out_box[], int box_size, unsigned int ks_bits[], unsigned int shuffles);
void sBox(unsigned int input[], int input_len, unsigned int sbox[]);
void pBox(unsigned int output[], unsigned int input[], int input_len, unsigned int pbox[]);
void bytesToNybbles(unsigned int input[], int input_len, unsigned int nybbles[]);
void nybblesToBytes(unsigned int output[], int output_len, unsigned int nybbles[]);
void apply_pbox(unsigned int block[], unsigned int pbox[]);
void diffusion_slide_encrypt(unsigned int block[]);
void encrypt_test4D(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles, unsigned int xor_bytes[]);
void genInvertedBox(unsigned int inv_box[], unsigned int box[], int box_size);
void diffusion_slide_decrypt(unsigned int block[]);
void decrypt_test4D(unsigned int block[], unsigned int inv_sbox[], unsigned int inv_pbox[], int pbox_cycles, int ncycles, unsigned int xor_bytes[]);
void key_schedule1A(unsigned int input_key[], unsigned int output_key_schedule[][16]);
void key_generator1B(unsigned int original_key[]);

std::random_device random_source;

int main(int argc, char *argv[]) {

    const unsigned int n_sbox_shuffles = 32;
    const unsigned int n_ks_bits_per_box = 8192;
    const unsigned int n_pbox_shuffles = 256;
    const unsigned int cycles_per_pbox = 2;
    const unsigned int cycles = 16;
    const unsigned int bit_flippers[] = { 128, 64, 32, 16, 8, 4, 2, 1 };

    double difference_distribution[129];
    for(unsigned int n = 0; n <= 128; n++) {
        difference_distribution[n] = 0;
    }

    auto start_keygenerator1A = high_resolution_clock::now();

    unsigned int* key_bytes = new unsigned int[NTEST_KEYS * 16];
    unsigned int nkey_bytes = 0;
    unsigned int* key_bits = new unsigned int[NTEST_KEYS * 16 * 8];

    unsigned int nkeys_count = 0;
    unsigned int original_key[16];
    for(unsigned int nkeys = 0; nkeys < NTEST_KEYS; nkeys++) {
        nkeys_count++;
        system("clear");
        std::cout << "Generated key: " << nkeys_count << "/" << NTEST_KEYS << "\n";
        
        key_generator1B(original_key);

        // //Debug
        // std::cout << "Generated key: ";
        // for(unsigned int n = 0; n < 16; n++) {
        //     std::cout << original_key[n] << " ";
        // }
        // std::cout << "\n";

        for(unsigned int n = 0; n < 16; n++) {
            key_bytes[nkey_bytes] = original_key[n];
            nkey_bytes++;
        }

    }

    bytesToBits(key_bytes, NTEST_KEYS * 16, key_bits);
    double pop_count = 0;
    for(unsigned int nbits = 0; nbits < NTEST_KEYS * 16 * 8; nbits++) {
        if (key_bits[nbits] == 1) {
            pop_count++;
        }
    }
    std::cout << "Pop Count: " << pop_count << "/" << NTEST_KEYS * 16 * 8 << " (" << pop_count / (NTEST_KEYS * 16 * 8) * 100 << "%)\n";

    auto stop_keygenerator1A = high_resolution_clock::now();
    auto duration_keygenerator1A = duration_cast<seconds>(stop_keygenerator1A - start_keygenerator1A);
    std::cout << "Tests took " << duration_keygenerator1A.count() << " seconds.\n";

    delete[] key_bytes;
    delete[] key_bits;

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

void genBox3(unsigned int out_box[], int box_size, unsigned int ks_bits[], unsigned int shuffles) {
    //Setup out_box to initial values in order from 0 to box_size
    //shuffles should be greater for greater box_size, actual amount is up to the user
    for(int i = 0; i < box_size; i++) {
        out_box[i] = i;
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

void apply_pbox(unsigned int block[], unsigned int pbox[]) {
    unsigned int temp_nybbles_input[32];
    bytesToNybbles(block, 16, temp_nybbles_input); //(10 * 16 + 1 + 2 * 16) * 4 cycles = 772
    unsigned int temp_nybbles_output[32];
    pBox(temp_nybbles_output, temp_nybbles_input, 32, pbox); //(1 + 2 * 32 + 4 * 32) * 4 cycles = 772
    nybblesToBytes(block, 16, temp_nybbles_output); //(1 + 2 * 16 + 7 * 16) * 4 cycles = 644
}

void diffusion_slide_encrypt(unsigned int block[]) {
    for(unsigned int i = 1; i < 15; i++) { //(1 + 2 * 14) * 4 cycles = 116
        if (i == 14) { //(1 * 14) * 4 cycles = 56
            block[0] ^= block[15]; //(4 * 1) * 4 = 16
        } else {
            block[i + 1] ^= block[i]; //(5 * 13) * 4 = 260
        }
    }
}

void encrypt_test4D(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles, unsigned int xor_bytes[]) {
    unsigned int nxor_bytes = 0; // = 1
    for(unsigned int cycles = 1; cycles <= ncycles; cycles++) { //1 + 2 * 16 = 33
        sBox(block, 16, sbox); //(1 + 2 * 16 + 4 * 16) * 16 cycles = 1552
        if (cycles % pbox_cycles == 0 && cycles % 4 != 0) {  //5 * 16 = 80
            apply_pbox(block, pbox);
        } else if (cycles % 4 == 0) { // 2 * 4 = 8
            diffusion_slide_encrypt(block);
        }
        //16 bytes per cycle, 16 cycles = 256 xor bytes
        for(unsigned int n = 0; n < 16; n++, nxor_bytes++) { //(1 + 3 * 16) * 16 cycles = 784
            block[n] ^= xor_bytes[nxor_bytes]; //4 * 16 * 16 = 1024
        }

        //Operations per block: 6117
    }
}

void genInvertedBox(unsigned int inv_box[], unsigned int box[], int box_size) {
    //Create the inverted S-Box, P-Box, etc. using the initial box
    for(int n = 0; n < box_size; n++) {
        inv_box[box[n]] = n;
    }
}

void diffusion_slide_decrypt(unsigned int block[]) {
    for(unsigned int i = 15; i >= 2; i--) {
        if (i == 15) {
            block[0] ^= block[15];
        } else {
            block[i] ^= block[i - 1];
        }
    }
}

void decrypt_test4D(unsigned int block[], unsigned int inv_sbox[], unsigned int inv_pbox[], int pbox_cycles, int ncycles, unsigned int xor_bytes[]) {
    unsigned int nxor_bytes = 255; // = 1
    for(unsigned int cycles = ncycles; cycles > 0; cycles--) { //1 + 2 * 16 = 33
        //16 bytes per cycle, 16 cycles = 256 xor bytes
        for(int n = 15; n >= 0; n--, nxor_bytes--) { //(1 + 3 * 16) * 16 cycles = 784
            block[n] ^= xor_bytes[nxor_bytes]; //4 * 16 * 16 = 1024
        }
        if (cycles % pbox_cycles == 0 && cycles % 4 != 0) {  //5 * 16 = 80
            apply_pbox(block, inv_pbox);
        } else if (cycles % 4 == 0) { // 2 * 4 = 8
            diffusion_slide_decrypt(block);
        }

        sBox(block, 16, inv_sbox); //(1 + 2 * 16 + 4 * 16) * 16 cycles = 1552

        //Operations per block: 6117
    }
}

void key_schedule1A(unsigned int input_key[], unsigned int output_key_schedule[][16]) {
    //Get shifted blocks
    unsigned int temp_key_schedule[inner_ks_blocks][16];
    for(unsigned int n_shifts = 0; n_shifts < 16; n_shifts++) {
        for(unsigned int n = n_shifts, n_bytes = 0; n_bytes < 16; n_bytes++) {
            temp_key_schedule[n_shifts][n_bytes] = input_key[n];
            n++;
            if (n > 15) {
                n = 0;
            }
        }
    }

    //Get flipped blocks
    unsigned int n_shifted = 0;
    unsigned int n_bytes = 0;
    unsigned int n_flippers = 0;
    unsigned int bit_flippers[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    for(unsigned int n_blocks = 16; n_blocks < inner_ks_blocks;) {
        for(unsigned int n = 0; n < 16; n++) {
            temp_key_schedule[n_blocks][n] = temp_key_schedule[n_shifted][n];
        }
        temp_key_schedule[n_blocks][n_bytes] ^= bit_flippers[n_flippers];
        n_blocks++;
        n_flippers++;
        if (n_flippers == 8) {
            n_flippers = 0;
            n_bytes++;
        }
        if (n_bytes == 16) {
            n_bytes = 0;
        }
        if (n_blocks % 128 == 0) {
            n_shifted++;
        }
    }

    //Separate temp KS into pieces for setting up S and P box and XOR for encryption
    unsigned int temp_ks_sbox[1024];
    unsigned int temp_ks_pbox[1024];
    unsigned int temp_xor_bytes[256];
    unsigned int n_blocks = 0;
    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 1024; n_ks++) {
        temp_ks_sbox[n_ks] = temp_key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    unsigned int temp_ks_sbox_bits[1024 * 8];
    bytesToBits(temp_ks_sbox, 1024, temp_ks_sbox_bits);

    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 1024; n_ks++) {
        temp_ks_pbox[n_ks] = temp_key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    unsigned int temp_ks_pbox_bits[1024 * 8];
    bytesToBits(temp_ks_pbox, 1024, temp_ks_pbox_bits);

    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 256; n_ks++) {
        temp_xor_bytes[n_ks] = temp_key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }

    //Set up boxes
    unsigned int temp_sbox[256];
    genBox3(temp_sbox, 256, temp_ks_sbox_bits, 32);
    unsigned int temp_pbox[32];
    genBox3(temp_pbox, 32, temp_ks_pbox_bits, 256);

    unsigned int temp_IV[16];
    for(unsigned int n_bytes = 0; n_bytes < 16; n_bytes++) {
        temp_IV[n_bytes] = temp_key_schedule[n_blocks][n_bytes];
    }
    n_blocks++;

    //Create unencrypted KS
    for(unsigned int n_ks_blocks = 0; n_ks_blocks < ks_blocks; n_ks_blocks++, n_blocks++) {
        for(unsigned int n = 0; n < 16; n++) {
            output_key_schedule[n_ks_blocks][n] = temp_key_schedule[n_blocks][n];
        }
    }

    //Apply IV
    for(unsigned int n = 0; n < 16; n++) {
        output_key_schedule[0][n] ^= temp_IV[n];
    }

    //Encrpyt each block with CBC mode
    for(unsigned int n_ks_blocks = 0; n_ks_blocks < ks_blocks; n_ks_blocks++) {
        encrypt_test4D(output_key_schedule[n_ks_blocks], temp_sbox, temp_pbox, 2, 16, temp_xor_bytes);

        //Apply CBC except for last block
        if (n_ks_blocks != 143) {
            for(unsigned int n = 0; n < 16; n++) {
                output_key_schedule[n_ks_blocks + 1][n] ^= output_key_schedule[n_ks_blocks][n];
            }
        }
    }
}

void key_generator1B(unsigned int original_key[]) {
        //START KEY GENERATOR
        unsigned int temp_ks[ks_blocks][16];
        for(unsigned int n = 0; n < 16; n++) {
            original_key[n] = random_source() % 256;
        }

        // //Debug
        // std::cout << "Original key: ";
        // for(unsigned int n = 0; n < 16; n++) {
        //     std::cout << original_key[n] << " ";
        // }
        // std::cout << "\n";

        for(unsigned int nloops = 0; nloops < 4; nloops++) {
            key_schedule1A(original_key, temp_ks);

            for(unsigned int nks_bytes = 0, nks_blocks = 0; nks_blocks < ks_blocks; nks_bytes++) {
                if (nks_bytes > 15) {
                    nks_bytes = 0;
                    nks_blocks++;
                    if (nks_blocks >= ks_blocks) {
                        break;
                    }
                }

                original_key[nks_bytes] ^= temp_ks[nks_blocks][nks_bytes];
            }

            //Inject more PRNG into key, except last loop
            if (nloops != 3) {
                for(unsigned int nbytes = 0; nbytes < 16; nbytes++) {
                    original_key[nbytes] ^= random_source() % 256;
                }
            }
        }
        //END KEY GENERATOR
}