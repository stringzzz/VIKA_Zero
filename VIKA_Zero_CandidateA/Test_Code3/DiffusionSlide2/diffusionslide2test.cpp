/*
    This is part of the VIKA_Zero_CandidateA encryption system tests, 
    the source code file for testing the Diffusion Slide 2
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

const unsigned int NTEST_BLOCKS = 500000;

struct blocks {
    unsigned int plaintext[16];
    unsigned int ciphertext[16];
};

struct test_data {
    double mean_differences;
    unsigned int highest_diff;
    unsigned int lowest_diff;
    unsigned int range;
    double score;
};

void bytesToBits(unsigned int input[], int input_len, unsigned int bits[]);
void diffusion_slide_encrypt(unsigned int block[]);
void diffusion_slide_decrypt(unsigned int block[]);
void diffusion_slide2_encrypt(unsigned int block[]);
void diffusion_slide2_decrypt(unsigned int block[]);

std::random_device random_source;

int main(int argc, char *argv[]) {

    double difference_distribution[129];
    double difference_distribution2[129];
    for(unsigned int n = 0; n <= 128; n++) {
        difference_distribution[n] = 0;
        difference_distribution2[n] = 0;
    }

    struct blocks* test_blocks = new struct blocks[NTEST_BLOCKS];
    struct blocks* test_blocks2 = new struct blocks[NTEST_BLOCKS];

    for(unsigned int nblock_set = 0; nblock_set < NTEST_BLOCKS; nblock_set++) {
        struct blocks temp_block;
        struct blocks temp_block2;
        for(unsigned int nbytes = 0; nbytes < 16; nbytes++) {
            unsigned int temp = random_source() % 256;
            temp_block.plaintext[nbytes] = temp;
            temp_block2.plaintext[nbytes] = temp;
        }
        test_blocks[nblock_set] = temp_block;
        test_blocks2[nblock_set] = temp_block2;
    }

    auto start_keyschedule1A = high_resolution_clock::now();

//Diffusion slide 1
    if (true) {
        struct test_data single_test_data;
        single_test_data.mean_differences = 0;
        single_test_data.highest_diff = 0;
        single_test_data.lowest_diff = 128;
        single_test_data.range = 0;

        unsigned int individual_tests = 0;

        unsigned int* differences = new unsigned int[NTEST_BLOCKS];
        unsigned int difference_n = 0;
        for(unsigned int nblock_set = 0; nblock_set < NTEST_BLOCKS; nblock_set++) {

            individual_tests++;
            system("clear");
            std::cout << "Diffusion Slide Test: " << individual_tests << "/" << NTEST_BLOCKS << "\n";

            for(unsigned int n = 0; n < 16; n++) {
                test_blocks[nblock_set].ciphertext[n] = test_blocks[nblock_set].plaintext[n];
            }

            unsigned int original_cbits[128];
            bytesToBits(test_blocks[nblock_set].plaintext, 16, original_cbits);
            diffusion_slide_encrypt(test_blocks[nblock_set].ciphertext);

            unsigned int temp_cbits[128];
            bytesToBits(test_blocks[nblock_set].ciphertext, 16, temp_cbits);
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

        double total_differences = 0;
        for(unsigned int n_diffs = 0; n_diffs < NTEST_BLOCKS; n_diffs++) {
            total_differences += differences[n_diffs];
        }

        delete[] differences;

        single_test_data.mean_differences = (double)total_differences / (NTEST_BLOCKS);
        single_test_data.range = single_test_data.highest_diff - single_test_data.lowest_diff;

        //FLAG
        std::ofstream keyschedule_file("diffusion_slide_final.txt", std::ios::out);
        if (keyschedule_file.is_open()) {
            
            keyschedule_file << 
            "Highest Difference:\t" << single_test_data.highest_diff << "/128" <<
            "\nLowest Difference:\t" << single_test_data.lowest_diff << "/128" <<
            "\nRange:\t" << single_test_data.range <<
            "\nAverage Difference:\t" << single_test_data.mean_differences << "/128" <<
            "\n\n";

            keyschedule_file.close();
        }

        std::ofstream keyschedule_file2("diffusion_slide_distribution.txt", std::ios::out);
        if (keyschedule_file2.is_open()) {
            
            for(unsigned int n = 0; n <= 128; n++) {
                keyschedule_file2 << n << ":\t" << difference_distribution[n] << "\n";
            }

            keyschedule_file2.close();
        }

        delete[] test_blocks;
    }

//######################################################
    //Diffusion slide2
    struct test_data single_test_data;
    single_test_data.mean_differences = 0;
    single_test_data.highest_diff = 0;
    single_test_data.lowest_diff = 128;
    single_test_data.range = 0;

    unsigned int individual_tests = 0;

    unsigned int* differences = new unsigned int[NTEST_BLOCKS];
    unsigned int difference_n = 0;
    for(unsigned int nblock_set = 0; nblock_set < NTEST_BLOCKS; nblock_set++) {

        individual_tests++;
        system("clear");
        std::cout << "Diffusion Slide 2 Test: " << individual_tests << "/" << NTEST_BLOCKS << "\n";

        for(unsigned int n = 0; n < 16; n++) {
            test_blocks2[nblock_set].ciphertext[n] = test_blocks2[nblock_set].plaintext[n];
        }

        unsigned int original_cbits[128];
        bytesToBits(test_blocks2[nblock_set].plaintext, 16, original_cbits);
        diffusion_slide2_encrypt(test_blocks2[nblock_set].ciphertext);

        unsigned int temp_cbits[128];
        bytesToBits(test_blocks2[nblock_set].ciphertext, 16, temp_cbits);
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
        difference_distribution2[temp_differences]++;
    }

    double total_differences = 0;
    for(unsigned int n_diffs = 0; n_diffs < NTEST_BLOCKS; n_diffs++) {
        total_differences += differences[n_diffs];
    }

    delete[] differences;

    single_test_data.mean_differences = (double)total_differences / (NTEST_BLOCKS);
    single_test_data.range = single_test_data.highest_diff - single_test_data.lowest_diff;

    //FLAG
    std::ofstream keyschedule_file("diffusion_slide2_final.txt", std::ios::out);
    if (keyschedule_file.is_open()) {
        
        keyschedule_file << 
        "Highest Difference:\t" << single_test_data.highest_diff << "/128" <<
        "\nLowest Difference:\t" << single_test_data.lowest_diff << "/128" <<
        "\nRange:\t" << single_test_data.range <<
        "\nAverage Difference:\t" << single_test_data.mean_differences << "/128" <<
        "\n\n";

        keyschedule_file.close();
    }

    std::ofstream keyschedule_file2("diffusion_slide2_distribution.txt", std::ios::out);
    if (keyschedule_file2.is_open()) {
        
        for(unsigned int n = 0; n <= 128; n++) {
            keyschedule_file2 << n << ":\t" << difference_distribution2[n] << "\n";
        }

        keyschedule_file2.close();
    }

    delete[] test_blocks2;

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

void diffusion_slide_encrypt(unsigned int block[]) {
    for(unsigned int i = 1; i < 15; i++) { //(1 + 2 * 14) * 4 cycles = 116
        if (i == 14) { //(1 * 14) * 4 cycles = 56
            block[0] ^= block[15]; //(4 * 1) * 4 = 16
        } else {
            block[i + 1] ^= block[i]; //(5 * 13) * 4 = 260
        }
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

void diffusion_slide2_encrypt(unsigned int block[]) {
    for(unsigned int i = 0; i < 16; i++) {
        if (i == 15) {
            block[0] ^= block[15];
        } else {
            block[i + 1] ^= block[i];
        }
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