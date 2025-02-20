/*
    This is part of the VIKA_Zero_CandidateA encryption system tests, 
    the source code file for testing the difference between the plaintext and ciphertext
    (With Diffusion Slide 2)
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

const unsigned int NTEST_KEYS = 3000; //FLAG
const unsigned int NTEST_PLAINTEXTS = 500000;
const unsigned int ks_blocks = 144;
const unsigned int box_bytes = 1024;
const unsigned int inner_ks_blocks = 289;

struct block {
    unsigned int plaintext[16];
    unsigned int ciphertext[16];
};

void bytesToBits(unsigned int input[], int input_len, unsigned int bits[]);
void genBox3(unsigned int out_box[], int box_size, unsigned int ks_bits[], unsigned int shuffles);
void sBox(unsigned int input[], int input_len, unsigned int sbox[]);
void pBox(unsigned int output[], unsigned int input[], int input_len, unsigned int pbox[]);
void bytesToNybbles(unsigned int input[], int input_len, unsigned int nybbles[]);
void nybblesToBytes(unsigned int output[], int output_len, unsigned int nybbles[]);
void apply_pbox(unsigned int block[], unsigned int pbox[]);
void diffusion_slide2_encrypt(unsigned int block[]);
void encrypt_test4D(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles, unsigned int xor_bytes[]);
void genInvertedBox(unsigned int inv_box[], unsigned int box[], int box_size);
void diffusion_slide2_decrypt(unsigned int block[]);
void decrypt_test4D(unsigned int block[], unsigned int inv_sbox[], unsigned int inv_pbox[], int pbox_cycles, int ncycles, unsigned int xor_bytes[]);
void key_schedule1A(unsigned int input_key[], unsigned int output_key_schedule[][16]);

std::random_device random_source;

int main(int argc, char *argv[]) {

    unsigned int n_sbox_shuffles[] = { 32 };
    unsigned int n_ks_bits_per_box[] = { 8192 };
    unsigned int n_pbox_shuffles[] = { 256 };
    unsigned int cycles_per_pbox[] = { 2 };
    unsigned int cycles[] = { 16 };
    unsigned int bit_flippers[] = { 128, 64, 32, 16, 8, 4, 2, 1 };

    double difference_distribution[129];
    for(unsigned int n = 0; n <= 128; n++) {
        difference_distribution[n] = 0;
    }

    auto start_pandc_test = high_resolution_clock::now();

    unsigned int highest_difference = 0, lowest_difference = 128;
    double total_average_difference = 0;
    
    struct block* plaintext_blocks = new struct block[NTEST_PLAINTEXTS];
    for(unsigned int nblocks = 0; nblocks < NTEST_PLAINTEXTS; nblocks++) {
        for(unsigned int n = 0; n < 16; n++) {
            plaintext_blocks[nblocks].plaintext[n] = random_source() % 256;
        }
    }

    for(unsigned int n_tests = 0; n_tests < NTEST_KEYS; n_tests++) {
        system("clear");
        std::cout << "Test " << n_tests + 1 << "/" << NTEST_KEYS << "\n";

        //Copy plaintext into ciphertext:
        for(unsigned int nblocks = 0; nblocks < NTEST_PLAINTEXTS; nblocks++) {
            for(unsigned int n = 0; n < 16; n++) {
                plaintext_blocks[nblocks].ciphertext[n] = plaintext_blocks[nblocks].plaintext[n];
            }
        }

        //Gen PRNG key
        unsigned int original_key[16];
        for(unsigned int n = 0; n < 16; n++) {
            original_key[n] = random_source() % 256;
        }

        //Gen key schedule for original key
        unsigned int original_ks[ks_blocks][16];
        key_schedule1A(original_key, original_ks);

        //START Setup cipher parts
        unsigned int temp_ks_sbox[box_bytes];
        unsigned int temp_ks_pbox[box_bytes];
        unsigned int temp_xor_bytes[256];
        unsigned int n_blocks = 0;
        for(unsigned int n_ks = 0, n_bytes = 0; n_ks < box_bytes; n_ks++) {
            temp_ks_sbox[n_ks] = original_ks[n_blocks][n_bytes];
            n_bytes++;
            if(n_bytes == 16) {
                n_bytes = 0;
                n_blocks++;
            }
        }
        unsigned int temp_ks_sbox_bits[box_bytes * 8];
        bytesToBits(temp_ks_sbox, box_bytes, temp_ks_sbox_bits);

        for(unsigned int n_ks = 0, n_bytes = 0; n_ks < box_bytes; n_ks++) {
            temp_ks_pbox[n_ks] = original_ks[n_blocks][n_bytes];
            n_bytes++;
            if(n_bytes == 16) {
                n_bytes = 0;
                n_blocks++;
            }
        }
        unsigned int temp_ks_pbox_bits[box_bytes * 8];
        bytesToBits(temp_ks_pbox, box_bytes, temp_ks_pbox_bits);

        for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 256; n_ks++) {
            temp_xor_bytes[n_ks] = original_ks[n_blocks][n_bytes];
            n_bytes++;
            if(n_bytes == 16) {
                n_bytes = 0;
                n_blocks++;
            }
        }

        //Set up boxes
        unsigned int temp_sbox[256];
        genBox3(temp_sbox, 256, temp_ks_sbox_bits, n_sbox_shuffles[0]);
        unsigned int temp_pbox[32];
        genBox3(temp_pbox, 32, temp_ks_pbox_bits, n_pbox_shuffles[0]);
        //END Setup Cipher parts

        for(unsigned int nblocks = 0; nblocks < NTEST_PLAINTEXTS; nblocks++) {
            encrypt_test4D(plaintext_blocks[nblocks].ciphertext, temp_sbox, temp_pbox, cycles_per_pbox[0], cycles[0], temp_xor_bytes);
        }

        //END original key schedule

        double total_differences = 0;
        for(unsigned int nplaintext_blocks = 0; nplaintext_blocks < NTEST_PLAINTEXTS; nplaintext_blocks++) {
            unsigned int plaintext_bits[128];
            unsigned int ciphertext_bits[128];
            bytesToBits(plaintext_blocks[nplaintext_blocks].plaintext, 16, plaintext_bits);
            bytesToBits(plaintext_blocks[nplaintext_blocks].ciphertext, 16, ciphertext_bits);
            
            //Get differences between plaintext and ciphertext
            unsigned int temp_differences = 0;
            for(unsigned int n_bits = 0; n_bits < 128; n_bits++) {
                if (ciphertext_bits[n_bits] != plaintext_bits[n_bits]) {
                    temp_differences++;
                }
            }

            if (temp_differences > highest_difference) {
                highest_difference = temp_differences;
            }
            if (temp_differences < lowest_difference) {
                lowest_difference = temp_differences;
            }
            total_differences += temp_differences;
            difference_distribution[temp_differences]++;

        }
        
        total_average_difference += (double)total_differences / NTEST_PLAINTEXTS;

    }

    delete[] plaintext_blocks;

    //Get totals, output to different file
    total_average_difference /= NTEST_KEYS;

    //FLAG
    std::ofstream pandc_file("pandc_comparison_final.txt", std::ios::out);
    if (pandc_file.is_open()) {
        
        pandc_file << 
        "Final Results:" <<
        "\nHighest Difference:\t" << highest_difference << "/128" <<
        "\nLowest Difference:\t" << lowest_difference << "/128" <<
        "\nRange:\t" << highest_difference - lowest_difference <<
        "\nAverage Difference:\t" << total_average_difference << "/128" <<
        "\n\n";

        pandc_file.close();
    }

    //FLAG
    std::ofstream pandc_file2("pandc_comparison_distribution.txt", std::ios::out);
    if (pandc_file2.is_open()) {
        
        for(unsigned int n = 0; n <= 128; n++) {
            pandc_file2 << n << ":\t" << difference_distribution[n] << "\n";
        }

        pandc_file2.close();
    }

    auto stop_pandc_test = high_resolution_clock::now();
    auto duration_pandc_test = duration_cast<seconds>(stop_pandc_test - start_pandc_test);
    std::cout << "Tests took " << duration_pandc_test.count() << " seconds.\n";

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

void diffusion_slide2_encrypt(unsigned int block[]) {
    for(unsigned int i = 0; i < 16; i++) {
        if (i == 15) {
            block[0] ^= block[15];
        } else {
            block[i + 1] ^= block[i];
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
            diffusion_slide2_encrypt(block);
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

void diffusion_slide2_decrypt(unsigned int block[]) {
    for(unsigned int i = 15; i > 0; i--) {
        if (i == 15) {
            block[0] ^= block[15];
        }
        block[i] ^= block[i - 1];
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
            diffusion_slide2_decrypt(block);
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