/*
    This is part of the VIKA_Zero_CandidateA encryption system tests, the source code file for testing the keyschedule 1A Plaintext Avalanche Effect
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

const unsigned int NTEST_BLOCKS = 2000; //FLAG
const unsigned int NTEST_KEYS = 3500; //2000 * 3500 = 7,000,000

struct block {
    unsigned int original_pblock[16];
    unsigned int original_cblock[16];
    unsigned int flipped_pblocks[128][16];
    unsigned int flipped_cblocks[128][16];
};

struct test_data {
    double mean_differences_set[NTEST_KEYS];
    double mean_differences;
    unsigned int highest_diff;
    unsigned int lowest_diff;
    unsigned int range;
    double score;
};

unsigned int bit_flippers[] = { 128, 64, 32, 16, 8, 4, 2, 1 };

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

std::random_device random_source;

int main(int argc, char *argv[]) {

    unsigned int n_sbox_shuffles[] = { 32 };
    unsigned int n_ks_bits_per_box[] = { 8192 };
    unsigned int n_pbox_shuffles[] = { 256 };
    unsigned int cycles_per_pbox[] = { 2 };
    unsigned int cycles[] = { 16 };

    double difference_distribution[129];
    for(unsigned int n = 1; n <= 128; n++) {
        difference_distribution[n] = 0;
    }

    auto start_keyschedule1A = high_resolution_clock::now();

    struct block* test_blocks = new struct block[NTEST_BLOCKS];
    for(unsigned int nblock_set = 0; nblock_set < NTEST_BLOCKS; nblock_set++) {
        struct block temp_block;
        for(unsigned int nbytes = 0; nbytes < 16; nbytes++) {
            temp_block.original_pblock[nbytes] = random_source() % 256;
        }

        for(unsigned int flipped_blocks = 0; flipped_blocks < 128; flipped_blocks++) {
            for(unsigned int nbytes = 0; nbytes < 16; nbytes++) {
                temp_block.flipped_pblocks[flipped_blocks][nbytes] = temp_block.original_pblock[nbytes];
            }
        }

        for(unsigned int flipped_blocks = 0, n_flippers = 0, nbytes = 0; flipped_blocks < 128; flipped_blocks++) {
            temp_block.flipped_pblocks[flipped_blocks][nbytes] ^= bit_flippers[n_flippers];
            n_flippers++;
            if (n_flippers == 8) {
                n_flippers = 0;
                nbytes++;
            }
        }
        test_blocks[nblock_set] = temp_block;
    }

    unsigned int test_count = 0;
    for(unsigned int n_shuffles = 0; n_shuffles < 1; n_shuffles++) {

        for(unsigned int n_cycles_per_pbox = 0; n_cycles_per_pbox < 1; n_cycles_per_pbox++) {
            for(unsigned int n_cycles = 0; n_cycles < 1; n_cycles++) {

                test_count++;

                struct test_data single_test_data;
                single_test_data.mean_differences = 0;
                single_test_data.highest_diff = 0;
                single_test_data.lowest_diff = 128;
                single_test_data.range = 0;

                unsigned int individual_tests = 0;

                for(unsigned int n_keys = 0; n_keys < NTEST_KEYS; n_keys++) {
                    
                    //Gen PRNG key
                    unsigned int original_key[16];
                    unsigned int original_ks[528][16];
                    for(unsigned int n = 0; n < 16; n++) {
                        original_key[n] = random_source() % 256;
                    }

                    //START original key schedule
                    //Gen key schedule for original key
                    key_schedule1A(original_key, original_ks);

                    //START Setup cipher parts
                    unsigned int temp_ks_sbox[4096];
                    unsigned int temp_ks_pbox[4096];
                    unsigned int xor_bytes[256];
                    unsigned int n_blocks = 0;
                    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 4096; n_ks++) {
                        temp_ks_sbox[n_ks] = original_ks[n_blocks][n_bytes];
                        n_bytes++;
                        if(n_bytes == 16) {
                            n_bytes = 0;
                            n_blocks++;
                        }
                    }
                    unsigned int temp_ks_sbox_bits[4096 * 8];
                    bytesToBits(temp_ks_sbox, 4096, temp_ks_sbox_bits);

                    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 4096; n_ks++) {
                        temp_ks_pbox[n_ks] = original_ks[n_blocks][n_bytes];
                        n_bytes++;
                        if(n_bytes == 16) {
                            n_bytes = 0;
                            n_blocks++;
                        }
                    }
                    unsigned int temp_ks_pbox_bits[4096 * 8];
                    bytesToBits(temp_ks_pbox, 4096, temp_ks_pbox_bits);

                    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 256; n_ks++) {
                        xor_bytes[n_ks] = original_ks[n_blocks][n_bytes];
                        n_bytes++;
                        if(n_bytes == 16) {
                            n_bytes = 0;
                            n_blocks++;
                        }
                    }

                    //Set up boxes
                    unsigned int sbox[256];
                    genBox3(sbox, 256, temp_ks_sbox_bits, n_sbox_shuffles[n_shuffles]);
                    unsigned int pbox[32];
                    genBox3(pbox, 32, temp_ks_pbox_bits, n_pbox_shuffles[n_shuffles]);

                    unsigned int* differences = new unsigned int[NTEST_BLOCKS * 128];
                    unsigned int difference_n = 0;
                    for(unsigned int nblock_set = 0; nblock_set < NTEST_BLOCKS; nblock_set++) {

                        individual_tests++;
                        system("clear");
                        std::cout << "Test: " << individual_tests << "/" << NTEST_BLOCKS * NTEST_KEYS << "\n";

                        for(unsigned int n = 0; n < 16; n++) {
                            test_blocks[nblock_set].original_cblock[n] = test_blocks[nblock_set].original_pblock[n];
                        }

                        encrypt_test4D(test_blocks[nblock_set].original_cblock, sbox, pbox, cycles_per_pbox[n_cycles_per_pbox], cycles[n_cycles], xor_bytes);
                        unsigned int original_cbits[128];
                        bytesToBits(test_blocks[nblock_set].original_cblock, 16, original_cbits);

                        for(unsigned int nblock_single = 0; nblock_single < 128; nblock_single++) {
                            for(unsigned int n = 0; n < 16; n++) {
                                test_blocks[nblock_set].flipped_cblocks[nblock_single][n] = test_blocks[nblock_set].flipped_pblocks[nblock_single][n];
                            }
                            encrypt_test4D(test_blocks[nblock_set].flipped_cblocks[nblock_single], sbox, pbox, cycles_per_pbox[n_cycles_per_pbox], cycles[n_cycles], xor_bytes);
                        }

                        for(unsigned int nblock_single = 0; nblock_single < 128; nblock_single++) {
                            unsigned int temp_cbits[128];
                            bytesToBits(test_blocks[nblock_set].flipped_cblocks[nblock_single], 16, temp_cbits);
                            unsigned int temp_differences = 0;
                            for(unsigned int i = 0; i < 128; i++) {
                                if (original_cbits[i] ^ temp_cbits[i] == 1) {
                                    temp_differences++;
                                }
                            }
                            differences[difference_n] = temp_differences;
                            difference_n++;
                            if (temp_differences > single_test_data.highest_diff) {
                                single_test_data.highest_diff = temp_differences;
                            }
                            if (temp_differences < single_test_data.lowest_diff) {
                                single_test_data.lowest_diff = temp_differences;
                            }
                            difference_distribution[temp_differences]++;
                        }

                    }

                    double total_differences = 0;
                    for(unsigned int n_diffs = 0; n_diffs < NTEST_BLOCKS * 128; n_diffs++) {
                        total_differences += differences[n_diffs];
                    }
                    delete[] differences;

                    single_test_data.mean_differences_set[n_keys] = (double)total_differences / (NTEST_BLOCKS * 128);

                }

                double total_differences = 0;
                for(unsigned int n_diffs = 0; n_diffs < NTEST_KEYS; n_diffs++) {
                    total_differences += single_test_data.mean_differences_set[n_diffs];
                }

                single_test_data.mean_differences = (double)total_differences / NTEST_KEYS;
                single_test_data.range = single_test_data.highest_diff - single_test_data.lowest_diff;

                //FLAG
                std::ofstream keyschedule_file("keyschedule1A_SPAC_test2_2000x3500_final.txt", std::ios::app);
                if (keyschedule_file.is_open()) {
                    
                    keyschedule_file << 
                    "Highest Difference:\t" << single_test_data.highest_diff << "/128" <<
                    "\nLowest Difference:\t" << single_test_data.lowest_diff << "/128" <<
                    "\nRange:\t" << single_test_data.range <<
                    "\nAverage Difference:\t" << single_test_data.mean_differences << "/128" <<
                    "\n\n";

                    keyschedule_file.close();
                }

                std::ofstream keyschedule_file2("keyschedule1A_SPAC_test2_distribution_final.txt", std::ios::app);
                if (keyschedule_file2.is_open()) {
                    
                    for(unsigned int n = 1; n <= 128; n++) {
                        keyschedule_file2 << n << ":\t" << difference_distribution[n] << "\n";
                    }

                    keyschedule_file2.close();
                }

            }
        }
    }

    delete[] test_blocks;

    auto stop_keyschedule1A = high_resolution_clock::now();
    auto duration_keyschedule1A = duration_cast<seconds>(stop_keyschedule1A - start_keyschedule1A);
    std::cout << "Tests took " << duration_keyschedule1A.count() << " seconds.\n";

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
    unsigned int temp_key_schedule[1057][16];
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
    for(unsigned int n_blocks = 16; n_blocks < 1057;) {
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
    unsigned int temp_ks_sbox[4096];
    unsigned int temp_ks_pbox[4096];
    unsigned int temp_xor_bytes[256];
    unsigned int n_blocks = 0;
    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 4096; n_ks++) {
        temp_ks_sbox[n_ks] = temp_key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    unsigned int temp_ks_sbox_bits[4096 * 8];
    bytesToBits(temp_ks_sbox, 4096, temp_ks_sbox_bits);
    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 4096; n_ks++) {
        temp_ks_pbox[n_ks] = temp_key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    unsigned int temp_ks_pbox_bits[4096 * 8];
    bytesToBits(temp_ks_pbox, 4096, temp_ks_pbox_bits);
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
    for(unsigned int n_ks_blocks = 0; n_ks_blocks < 528; n_ks_blocks++, n_blocks++) {
        for(unsigned int n = 0; n < 16; n++) {
            output_key_schedule[n_ks_blocks][n] = temp_key_schedule[n_blocks][n];
        }
    }

    //Apply IV
    for(unsigned int n = 0; n < 16; n++) {
        output_key_schedule[0][n] ^= temp_IV[n];
    }

    //Encrpyt each block with CBC mode
    for(unsigned int n_ks_blocks = 0; n_ks_blocks < 528; n_ks_blocks++) {
        encrypt_test4D(output_key_schedule[n_ks_blocks], temp_sbox, temp_pbox, 2, 16, temp_xor_bytes);

        //Apply CBC except for last block
        if (n_ks_blocks != 527) {
            for(unsigned int n = 0; n < 16; n++) {
                output_key_schedule[n_ks_blocks + 1][n] ^= output_key_schedule[n_ks_blocks][n];
            }
        }
    }
}