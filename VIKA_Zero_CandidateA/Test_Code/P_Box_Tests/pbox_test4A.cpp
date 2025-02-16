/*
    This is part of the VIKA_Zero_CandidateA encryption system tests, the source code file for testing the P-Box setups 4A
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

const unsigned int NTEST_BLOCKS = 500; //FLAG
const unsigned int NTEST_BOXES = 250; //500 * 250 = 125,000

struct block {
    unsigned int original_pblock[16];
    unsigned int original_cblock[16];
    unsigned int flipped_pblocks[128][16];
    unsigned int flipped_cblocks[128][16];
};

struct test_data {
    unsigned int n_sbox_shuffles;
    unsigned int n_pbox_shuffles;
    unsigned int n_ks_bits_per_box;
    unsigned int n_cycles_per_pbox;
    unsigned int n_cycles;
    double mean_differences_set[NTEST_BOXES];
    double mean_differences;
    unsigned int highest_diff;
    unsigned int lowest_diff;
    unsigned int range;
    double score;
};

unsigned int bit_flippers[] = { 128, 64, 32, 16, 8, 4, 2, 1 };

void bytesToBits(unsigned int input[], int input_len, unsigned int bits[]);
void genBox2(unsigned int out_box[], int box_size, unsigned int nks_bits, unsigned int shuffles);
void sBox(unsigned int input[], int input_len, unsigned int sbox[]);
void pBox(unsigned int output[], unsigned int input[], int input_len, unsigned int pbox[]);
void bytesToNybbles(unsigned int input[], int input_len, unsigned int nybbles[]);
void nybblesToBytes(unsigned int output[], int output_len, unsigned int nybbles[]);
void encrypt_test(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles);

std::random_device random_source;

int main(int argc, char *argv[]) {

    unsigned int n_sbox_shuffles[] = { 32, 64, 128, 256, 512, 1024 };
    unsigned int n_ks_bits_per_box[] = { 8192, 16384, 32768, 65536, 131072, 262144 };
    unsigned int n_pbox_shuffles[] = { 256, 512, 1024, 2048, 4096, 8192 };
    unsigned int cycles_per_pbox[] = { 2 };
    unsigned int cycles[] = { 10, 12, 14, 16 };

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
    for(unsigned int n_shuffles = 0; n_shuffles < 6; n_shuffles++) {

        for(unsigned int n_cycles_per_pbox = 0; n_cycles_per_pbox < 1; n_cycles_per_pbox++) {
            for(unsigned int n_cycles = 0; n_cycles < 4; n_cycles++) {

                test_count++;
                std::cout << "Test #: " << test_count << "/24\n";

                struct test_data single_test_data;
                single_test_data.n_sbox_shuffles = n_sbox_shuffles[n_shuffles];
                single_test_data.n_pbox_shuffles = n_pbox_shuffles[n_shuffles];
                single_test_data.n_ks_bits_per_box = n_ks_bits_per_box[n_shuffles];
                single_test_data.n_cycles_per_pbox = cycles_per_pbox[n_cycles_per_pbox];
                single_test_data.n_cycles = cycles[n_cycles];
                single_test_data.mean_differences = 0;
                single_test_data.highest_diff = 0;
                single_test_data.lowest_diff = 128;
                single_test_data.range = 0;
                single_test_data.score = 0;

                for(unsigned int n_boxes = 0; n_boxes < NTEST_BOXES; n_boxes++) {
                    unsigned int sbox[256];
                    genBox2(sbox, 256, n_ks_bits_per_box[n_shuffles], n_sbox_shuffles[n_shuffles]);
                    unsigned int pbox[32];
                    genBox2(pbox, 32, n_ks_bits_per_box[n_shuffles], n_pbox_shuffles[n_shuffles]);

                    unsigned int* differences = new unsigned int[NTEST_BLOCKS * 128];
                    unsigned int difference_n = 0;
                    for(unsigned int nblock_set = 0; nblock_set < NTEST_BLOCKS; nblock_set++) {

                        for(unsigned int n = 0; n < 16; n++) {
                            test_blocks[nblock_set].original_cblock[n] = test_blocks[nblock_set].original_pblock[n];
                        }

                        encrypt_test(test_blocks[nblock_set].original_cblock, sbox, pbox, single_test_data.n_cycles_per_pbox, single_test_data.n_cycles);
                        unsigned int original_cbits[128];
                        bytesToBits(test_blocks[nblock_set].original_cblock, 16, original_cbits);

                        for(unsigned int nblock_single = 0; nblock_single < 128; nblock_single++) {
                            for(unsigned int n = 0; n < 16; n++) {
                                test_blocks[nblock_set].flipped_cblocks[nblock_single][n] = test_blocks[nblock_set].flipped_pblocks[nblock_single][n];
                            }
                            encrypt_test(test_blocks[nblock_set].flipped_cblocks[nblock_single], sbox, pbox, single_test_data.n_cycles_per_pbox, single_test_data.n_cycles);
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
                        }

                    }

                    double total_differences = 0;
                    for(unsigned int n_diffs = 0; n_diffs < NTEST_BLOCKS * 128; n_diffs++) {
                        total_differences += differences[n_diffs];
                    }
                    delete[] differences;

                    single_test_data.mean_differences_set[n_boxes] = (double)total_differences / (NTEST_BLOCKS * 128);

                }

                double total_differences = 0;
                for(unsigned int n_diffs = 0; n_diffs < NTEST_BOXES; n_diffs++) {
                    total_differences += single_test_data.mean_differences_set[n_diffs];
                }

                single_test_data.mean_differences = (double)total_differences / NTEST_BOXES;
                single_test_data.range = single_test_data.highest_diff - single_test_data.lowest_diff;
                single_test_data.score = ((double)64 - single_test_data.mean_differences) > 0? ((double)64 - single_test_data.mean_differences) : -1 * ((double)64 - single_test_data.mean_differences);
                //Lowest (Best): abs(64 - 64) = 0
                //Highest (Worst): abs(64 - 0) (or 64 - 128) = 64

                //FLAG
                std::ofstream pbox_file("pbox_test4A_500x250.txt", std::ios::app);
                if (pbox_file.is_open()) {
                    
                    pbox_file << 
                    "S-Box Shuffles:\t" << single_test_data.n_sbox_shuffles <<
                    "\nP-Box Shuffles:\t" << single_test_data.n_pbox_shuffles <<
                    "\nKS Bits Per Box:\t" << single_test_data.n_ks_bits_per_box <<
                    "\nCycles Per P-Box:\t" << single_test_data.n_cycles_per_pbox <<
                    "\n# of Cycles:\t" << single_test_data.n_cycles <<
                    "\nMean Differences:\t" << single_test_data.mean_differences << "/128"
                    "\nHighest Difference:\t" << single_test_data.highest_diff <<
                    "\nLowest Difference:\t" << single_test_data.lowest_diff <<
                    "\nRange:\t" << single_test_data.range <<
                    "\nScore (0 best, 64 worst):\t" << single_test_data.score <<
                    "\n\n";

                    pbox_file.close();
                }

            }
        }
    }

    delete[] test_blocks;

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

void encrypt_test(unsigned int block[], unsigned int sbox[], unsigned int pbox[], int pbox_cycles, int ncycles) {
    for(unsigned int cycles = 1; cycles <= ncycles; cycles++) {
        sBox(block, 16, sbox);
        if (cycles % pbox_cycles == 0) {
            unsigned int temp_nybbles_input[32];
            bytesToNybbles(block, 16, temp_nybbles_input);
            unsigned int temp_nybbles_output[32];
            pBox(temp_nybbles_output, temp_nybbles_input, 32, pbox);
            nybblesToBytes(block, 16, temp_nybbles_output);
        }
    }
}