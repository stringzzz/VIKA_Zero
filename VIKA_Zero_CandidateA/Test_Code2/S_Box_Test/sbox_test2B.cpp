/*
    This is part of the VIKA_Zero_CandidateA encryption system tests, the source code file for testing different S-Box specs
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

#define N_SBOXES    10000

std::random_device random_source;

void bytesToBits(unsigned int input[], int input_len, unsigned int bits[]);
void genBox2(unsigned int out_box[], int box_size, int shuffles, unsigned int KS_BITS_N);

int main() {

    unsigned int base_bytes[256];
    for(unsigned int n = 0; n < 256; n++) {
        base_bytes[n] = n;
    }

    unsigned int input_bits[256 * 8];
    bytesToBits(base_bytes, 256, input_bits);

    std::ofstream sbox_test2_file("sbox_test2_resultsB1_10000_sboxes.txt", std::ios::out);
    if (sbox_test2_file.is_open()) {

        unsigned int N_SHUFFLES[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
        unsigned int KS_BITS_N[] = { 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144}; 
        for(unsigned int n_specs = 0; n_specs < 11; n_specs++) {

            std::cout << "Testing with specs:\tShuffles: " << N_SHUFFLES[n_specs] << "/1024\tKS_Bits: " << KS_BITS_N[n_specs] << "/262144\n";
            unsigned int** shuffled_sboxes = new unsigned int*[N_SBOXES];
            for (int i = 0; i < N_SBOXES; i++) {
                shuffled_sboxes[i] = new unsigned int[256];
            }

            for(unsigned int i = 0; i < N_SBOXES; i++) {
                genBox2(shuffled_sboxes[i], 256, N_SHUFFLES[n_specs], KS_BITS_N[n_specs]);
            }

            double each_differences[N_SBOXES], highest_diff = 0, lowest_diff = 2048;
            for(unsigned int sbox_n = 0; sbox_n < N_SBOXES; sbox_n++) {
                unsigned int output_bytes[256];
                for(unsigned int byte_n = 0; byte_n < 256; byte_n++) {
                    output_bytes[byte_n] = shuffled_sboxes[sbox_n][base_bytes[byte_n]];
                }
                unsigned int output_bits[256 * 8];
                bytesToBits(output_bytes, 256, output_bits);
                double total_diffs = 0;
                double differences = 0;
                for(unsigned int i = 0; i < 2048; i++) {
                    if (input_bits[i] ^ output_bits[i] == 1) {
                        differences++;
                    }
                }
                each_differences[sbox_n] = differences;
                if (each_differences[sbox_n] > highest_diff) {
                    highest_diff = each_differences[sbox_n];
                }
                if (each_differences[sbox_n] < lowest_diff) {
                    lowest_diff = each_differences[sbox_n];
                }
            }

            for(int i = 0; i < N_SBOXES; i++) {
                delete[] shuffled_sboxes[i];
            }
            delete[] shuffled_sboxes; 

            double total_average_diffs = 0, final_average;
            for(unsigned int i = 0; i < N_SBOXES; i++) {
                total_average_diffs += each_differences[i];
            }

            final_average = total_average_diffs / N_SBOXES;
            double range = abs(highest_diff - lowest_diff);

            //Score: 0 + range + abs(1 - N_SHUFFLES[n_specs]) + abs(1024 - final_average)
            //Lowest (Best): 0 + 0 + abs(1 - 1) + abs(1024 - 1024) = 0
            //Highest (Worst): 0 + 2048 + abs(1 - 1024) + abs(1024 - 0) = 4095

            sbox_test2_file << "S-Box Shuffles:\t" << N_SHUFFLES[n_specs] << "\nKS bytes for each box:\t" << KS_BITS_N[n_specs] 
            << "\nMean Differences:\t" << final_average << "/2048\nRange:\t" << range << "\nScore: " << (0 + range + abs(1 - N_SHUFFLES[n_specs]) + abs(1024 - final_average)) << "\n\n";

        }

    }

    sbox_test2_file.close();

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

void genBox2(unsigned int out_box[], int box_size, int shuffles, unsigned int KS_BITS_N) {
    //Setup out_box to initial values in order from 0 to box_size
    //Suggested not to use in the actual crackme, as it will change evey time the program is ran
    //Meant for setting up the boxes to use in the crackme with the other functions
    //Such as sBox, pBox, etc.
    //shuffles should be greater for greater box_size, actual amount is up to the user
    for(int i = 0; i < box_size; i++) {
        out_box[i] = i;
    }

    unsigned int ks_bits[KS_BITS_N];
    for(unsigned int n = 0; n < KS_BITS_N; n++) {
        ks_bits[n] = random_source() % 2;
    }

    //Shuffle the box by int shuffles amount of times
    //Uses bits to choose from beginning (left) or end (right) to grab from
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