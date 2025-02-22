/*
    This is part of the VIKA_Zero_CandidateA encryption system, the source code file containing the VIKA_Zero_CandidateA class methods
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

/*
	VIKA_Zero_CandidateA: Vanishing Illusion Key Automator Zero (Candidate A)

	Project Start date: 12-24-2024

    Mistake Found: 02-15-2025: Too much Key Schedule was being produced, almost 3 times more than needed.
    This was fixed and it is working now, but this may invalidate some of the tests.

    02-16-2025: All affected test code fixed, tests rerun, and the informal paper is now revised.

    02-20-2025: Created new diffusion slide functions that now cover the last byte in the block.
	
	Candidate A Complete: 02-21-2025

!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!

This system is purely for educational purposes, not actual secure purposes.
It has yet to go through the peer review process, and likely won't in its current state due to my lack
of the necessary tools needed to properly analyze it myself at this time, namely Abstract algebra, Number Theory, and Statistics.
Anyone trying to use this system for secure purposes will be subject to ridicule and much trash thrown at them.

You've been warned! 
--stringzzz, Ghostwarez Co.

!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!

*/

#include "VIKA_Zero_CandidateA.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <sstream>
#include <iomanip>
#include <random>

std::random_device random_source;

void VIKA_Zero_CandidateA::bytes_to_bits(unsigned char input[], int input_len, unsigned char bits[]) {
    //Convert array of bytes into array of corresponding bits
    //bits must be size input_len * 8!
    int i = 0;
    for (unsigned int n = 0; n < input_len; n++) {
        for (int n2 = 1; n2 <= 8; n2++) {
            bits[i] = ((unsigned char)input[n] >> (8 - n2)) & 1;
            i++;
        }
    }
}

void VIKA_Zero_CandidateA::generate_box3(unsigned char out_box[], int box_size, unsigned char ks_bits[], unsigned int shuffles) {
    //Generate S or P-Box of box_size input/output using key schedule bits to shuffle the initial values
    //S-Box: box_size 256, ks_bits 8192, shuffles 32
    //Nybble P-Box: box size 32, ks_bits 8192, shuffles 256

    //Setup out_box to initial values in order from 0 to box_size
    for(int i = 0; i < box_size; i++) {
        out_box[i] = i;
    }

    //Shuffle the box by int shuffles amount of times
    //Uses ks_bits to choose from beginning (left) or end (right) to grab from
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

void VIKA_Zero_CandidateA::use_sbox(unsigned char input[], int input_len, unsigned char sbox[]) {
    //Apply the S-Box to input block
    for(int i = 0; i < input_len; i++) {
		input[i] = sbox[input[i]];
	}
}

void VIKA_Zero_CandidateA::use_pbox(unsigned char output[], unsigned char input[], int input_len, unsigned char pbox[]) {
    //Apply the P-Box to input block
    //Different, but same size array for input and output
    for(int i = 0; i < input_len; i++) {
        output[pbox[i]] = input[i];
    }
}

void VIKA_Zero_CandidateA::bytes_to_nybbles(unsigned char input[], int input_len, unsigned char nybbles[]) {
    //Convert array of bytes to 4-bit nybbles
    //nybbles must be size input_len * 2!
    int n = 0;
    for(int i = 0; i < input_len; i++) {
        nybbles[n] = input[i] >> 4; //Left nybble
        n++;
        nybbles[n] = input[i] & 15; //Right nybble
        n++;
    }
}

void VIKA_Zero_CandidateA::nybbles_to_bytes(unsigned char output[], int output_len, unsigned char nybbles[]) {
    //Convert array of 4-bit nybbles to bytes
    //nybbles must be size output_len * 2!
    int n = 0;
    for(int i = 0; i < output_len; i++) {
        output[i] = ((unsigned char)nybbles[n] << 4) ^ (nybbles[n+1]);
        n += 2;
    }
}

void VIKA_Zero_CandidateA::apply_pbox(unsigned char block[], unsigned char pbox[]) {
    //Break block into 4-bit nybbles, apply nybble P-Box, reassemble nybbles back into bytes
    unsigned char temp_nybbles_input[32];
    bytes_to_nybbles(block, 16, temp_nybbles_input);
    unsigned char temp_nybbles_output[32];
    use_pbox(temp_nybbles_output, temp_nybbles_input, 32, pbox);
    nybbles_to_bytes(block, 16, temp_nybbles_output);
}

void VIKA_Zero_CandidateA::diffusion_slide2_encrypt(unsigned char block[]) {
    //Slide differences across the block
    for(unsigned int i = 0; i < 16; i++) {
        if (i == 15) {
            block[0] ^= block[15];
        } else {
            block[i + 1] ^= block[i];
        }
    }
}

void VIKA_Zero_CandidateA::vzca_encrypt(unsigned char block[], unsigned char sbox[], unsigned char pbox[], int pbox_cycles, int ncycles, unsigned char xor_bytes[]) {
    unsigned int nxor_bytes = 0;
    for(unsigned int cycles = 1; cycles <= ncycles; cycles++) {
        use_sbox(block, 16, sbox);

        if (cycles % pbox_cycles == 0 && cycles % 4 != 0) {
            apply_pbox(block, pbox);
        } else if (cycles % 4 == 0) {
            diffusion_slide2_encrypt(block);
        }

        //16 bytes per cycle, 16 cycles = 256 xor bytes
        for(unsigned int n = 0; n < 16; n++, nxor_bytes++) {
            block[n] ^= xor_bytes[nxor_bytes];
        }

    }
}

void VIKA_Zero_CandidateA::generate_reverse_box(unsigned char inv_box[], unsigned char box[], int box_size) {
    //Create the reverse S-Box, P-Box, etc. using the initial box
    for(int n = 0; n < box_size; n++) {
        inv_box[box[n]] = n;
    }
}

void VIKA_Zero_CandidateA::diffusion_slide2_decrypt(unsigned char block[]) {
    for(unsigned int i = 15; i > 0; i--) {
        if (i == 15) {
            block[0] ^= block[15];
        }
        block[i] ^= block[i - 1];
    }
}

void VIKA_Zero_CandidateA::vzca_decrypt(unsigned char block[], unsigned char inv_sbox[], unsigned char inv_pbox[], int pbox_cycles, int ncycles, unsigned char xor_bytes[]) {
    unsigned int nxor_bytes = 255;
    for(unsigned int cycles = ncycles; cycles > 0; cycles--) {

        //16 bytes per cycle, 16 cycles = 256 xor bytes
        for(int n = 15; n >= 0; n--, nxor_bytes--) {
            block[n] ^= xor_bytes[nxor_bytes];
        }

        if (cycles % pbox_cycles == 0 && cycles % 4 != 0) {
            apply_pbox(block, inv_pbox);
        } else if (cycles % 4 == 0) {
            diffusion_slide2_decrypt(block);
        }

        use_sbox(block, 16, inv_sbox);

    }
}

void VIKA_Zero_CandidateA::key_schedule1A(unsigned char input_key[], unsigned char output_key_schedule[ks_blocks][16]) {
    //Get shifted blocks
    unsigned char temp_key_schedule[inner_ks_blocks][16];
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
    unsigned char bit_flippers[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
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
    unsigned char temp_ks_sbox[box_bytes];
    unsigned char temp_ks_pbox[box_bytes];
    unsigned char temp_xor_bytes[256];
    unsigned int n_blocks = 0;
    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < box_bytes; n_ks++) {
        temp_ks_sbox[n_ks] = temp_key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    unsigned char temp_ks_sbox_bits[box_bytes * 8];
    bytes_to_bits(temp_ks_sbox, box_bytes, temp_ks_sbox_bits);

    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < box_bytes; n_ks++) {
        temp_ks_pbox[n_ks] = temp_key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    unsigned char temp_ks_pbox_bits[box_bytes * 8];
    bytes_to_bits(temp_ks_pbox, box_bytes, temp_ks_pbox_bits);

    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 256; n_ks++) {
        temp_xor_bytes[n_ks] = temp_key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }

    //Set up boxes
    unsigned char temp_sbox[256];
    generate_box3(temp_sbox, 256, temp_ks_sbox_bits, 32);
    unsigned char temp_pbox[32];
    generate_box3(temp_pbox, 32, temp_ks_pbox_bits, 256);

    unsigned char temp_IV[16];
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
        vzca_encrypt(output_key_schedule[n_ks_blocks], temp_sbox, temp_pbox, 2, 16, temp_xor_bytes);

        //Apply CBC except for last block
        if (n_ks_blocks != 143) {
            for(unsigned int n = 0; n < 16; n++) {
                output_key_schedule[n_ks_blocks + 1][n] ^= output_key_schedule[n_ks_blocks][n];
            }
        }
    }
}

void VIKA_Zero_CandidateA::distribute_key_schedule(unsigned char key_schedule[ks_blocks][16]) {
    //Separate temp KS into pieces for setting up S and P box and XOR for encryption
    unsigned int n_blocks = 0;
    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < box_bytes; n_ks++) {
        this->ks_sbox[n_ks] = key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    bytes_to_bits(this->ks_sbox, box_bytes, this->ks_sbox_bits);

    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < box_bytes; n_ks++) {
        this->ks_pbox[n_ks] = key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    bytes_to_bits(this->ks_pbox, box_bytes, this->ks_pbox_bits);

    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 256; n_ks++) {
        this->ks_xor_bytes[n_ks] = key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
}

void VIKA_Zero_CandidateA::setup_boxes() {
    generate_box3(this->sbox, 256, this->ks_sbox_bits, 32);
    generate_box3(this->pbox, 32, this->ks_pbox_bits, 256);
}

void VIKA_Zero_CandidateA::key_generator1B(unsigned char original_key[]) {
        //Generate an 128-bit key by using random_device,
        //create temporary key schedule and XOR it back into the key 4 times
        unsigned char temp_ks[ks_blocks][16];
        for(unsigned int n = 0; n < 16; n++) {
            original_key[n] = random_source() % 256;
        }

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
}


///////////////////
//Public interface
///////////////////

VIKA_Zero_CandidateA::VIKA_Zero_CandidateA() {

}

void VIKA_Zero_CandidateA::genKey() {
	//Generate a new prng key
	//Also create the key schedule, S-Box and P-Box from it
	this->key_generator1B(this->initial_key0);
    this->key_schedule1A(this->initial_key0, this->key_schedule);
    this->distribute_key_schedule(this->key_schedule);
    this->setup_boxes();
	this->key_set = true;
	this->reverse_set = false;
}

void VIKA_Zero_CandidateA::setKeyS(std::string key_string) {
	//Set the key as a 16 character string
	//Also create the key schedule, S-Box and P-Box from it
	if (key_string.length() > 16) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key as string must be exactly 16 characters, input key is greater then 16 characters.");
	}
	if (key_string.length() < 16) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key as string must be exactly 16 characters, input key is less then 16 characters.");
	}

	for (unsigned int i = 0; i < 16; i++) {
		this->initial_key0[i] = (unsigned char)key_string[i];
	}

    this->key_schedule1A(this->initial_key0, this->key_schedule);
    this->distribute_key_schedule(this->key_schedule);
    this->setup_boxes();
	this->key_set = true;
	this->reverse_set = false;
}

void VIKA_Zero_CandidateA::setKeyH(std::string hex_string) {
	//Set the key as a 32 digit hexadecimal string
	//Also create the key schedule, S-Box and P-Box from it
	if (hex_string.length() > 32) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key as hex string must be exactly 32 digits, input key is greater than 32 digits.");
	}
	if (hex_string.length() < 32) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key as hex string must be exactly 32 digits, input key is less than 32 digits.");
	}

    //Convert key as hex string into key as array of unsigned chars
	unsigned int j = 0;
	for (size_t i = 0; i < 32; i += 2, j++) {
		std::istringstream string_stream(hex_string.substr(i, 2));
		int hex_byte;
		string_stream >> std::hex >> hex_byte;
		this->initial_key0[j] = hex_byte; //hex_byte = 0-255, valid unsigned char
	}

    this->key_schedule1A(this->initial_key0, this->key_schedule);
    this->distribute_key_schedule(this->key_schedule);
    this->setup_boxes();
	this->key_set = true;
	this->reverse_set = false;
}

void VIKA_Zero_CandidateA::showKey() {
	//Output the key as a 32 digit hexadecimal string to the console
	std::cout << "Key: ";
	//Store old cout state:
	std::ios old_state(nullptr);
	old_state.copyfmt(std::cout);

	//Output hexadecimal digits to console:
	std::cout << std::hex << std::setfill('0');
	for (unsigned int i = 0; i < 16; i++) {
		std::cout << std::setw(2) << (int)this->initial_key0[i];
	}
	std::cout << "\n";

	//Restore old cout state
	std::cout.copyfmt(old_state);
}

void VIKA_Zero_CandidateA::encrypt(std::string file, std::string mode) {
	//Encrypt the file using the set key and either "ecb" or "cbc" mode
	//ecb: Electronic Code Book mode
	//cbc: Cipher Block Chaining mode
	if (!this->key_set) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key must be set to encrypt.");
	}
	if (mode != "ecb" && mode != "cbc") {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Invalid encrypt mode, must be \"ecb\" or \"cbc\".");
	}

	//Input file as chars
	std::streampos size;
	std::ifstream file_handle(file, std::ios::in | std::ios::binary | std::ios::ate);
	size = file_handle.tellg();
	std::vector <char> input_file0;
	input_file0.resize(size);
	if (file_handle.is_open()) {
		file_handle.seekg(0, std::ios::beg);
		file_handle.read(&input_file0[0], size);
		file_handle.close();
	}
	else {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Could not open file for read and encrypt.");
	}

	//Convert input to unsigned chars
	std::vector <unsigned char> input_file;
	input_file.resize(size);
	for (unsigned int i = 0; i < input_file0.size(); i++) {
		input_file[i] = (reinterpret_cast<unsigned char&>(input_file0[i]));
	}

	//Use padding to make 16n blocks even
	unsigned int padding_size = 16 - (input_file.size() % 16);
	for (unsigned int i = 0; i < padding_size; i++) {
		input_file.push_back((unsigned char)padding_size);
	}

	//Resize output vector
	std::vector <unsigned char> output_file1;
	if (mode == "ecb") {
		output_file1.resize(size + (std::streampos)padding_size);
	}
	else if (mode == "cbc") {
		output_file1.resize(size + (std::streampos)padding_size + (std::streampos)16);
	}

	//Encrypt either ecb or cbc mode
	unsigned block_count = input_file.size() / 16;
	if (mode == "ecb") {
		unsigned char plaintext_block[16];
		unsigned int block_index = 0;
		unsigned int block_index2 = 0;
		for (unsigned int block_number = 0; block_number < block_count; block_number++) {
			for (unsigned int i = 0; i < 16; i++, block_index++) {
				plaintext_block[i] = input_file[block_index];
			}
			this->vzca_encrypt(plaintext_block, this->sbox, this->pbox, 2, 16, this->ks_xor_bytes);
			for (unsigned int i = 0; i < 16; i++, block_index2++) {
				output_file1[block_index2] = plaintext_block[i];
			}
		}
	}
	else if (mode == "cbc") {
		unsigned char plaintext_block2[16];
        this->key_generator1B(plaintext_block2);
		unsigned char plaintext_block1[16];
		unsigned int block_index = 0;
		unsigned int block_index2 = 0;
		for (unsigned int block_number = 1; block_number <= block_count; block_number++) {
			for (unsigned int i = 0; i < 16; i++, block_index++) {
				plaintext_block1[i] = input_file[block_index];
			}
			for (unsigned int i = 0; i < 16; i++) {
				plaintext_block1[i] ^= plaintext_block2[i];
			}

			for (unsigned int i = 0; i < 16; i++, block_index2++) {
				output_file1[block_index2] = plaintext_block2[i];
			}
            this->vzca_encrypt(plaintext_block1, this->sbox, this->pbox, 2, 16, this->ks_xor_bytes);
            for(unsigned int n = 0; n < 16; n++) {
			    plaintext_block2[n] = plaintext_block1[n];
            }

		}
		for (unsigned int i = 0; i < 16; i++, block_index2++) {
			output_file1[block_index2] = plaintext_block2[i];
		}

	}

	//Make char output for file write
	std::vector <char> output_file2;
	std::streampos size2;
	if (mode == "ecb") {
		size2 = size + (std::streampos)padding_size;
	}
	else if (mode == "cbc") {
		size2 = size + (std::streampos)padding_size + (std::streampos)16;
	}
	output_file2.resize(size2);

	//Convert unsigned char to char
	for (unsigned int i = 0; i < output_file1.size(); i++) {
		output_file2[i] = (reinterpret_cast<char&>(output_file1[i]));
	}

	//Output the encrypted file bytes
	std::ofstream file2(file, std::ios::out | std::ios::binary);
	if (file2.is_open()) {
		file2.write(&output_file2[0], size2);
		file2.close();
	}
	else {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Could not open file for write after encrypt.");
	}

	//Rename the encrypted file with the new extension
	std::string new_filename = file + ".VIKA_Zero_CandidateA";
	std::string system_command = "mv " + file + " " + new_filename;
	char* system_command_chars = new char[150];
	unsigned int i = 0;
	for (; i < system_command.length(); i++) {
		system_command_chars[i] = system_command[i];
	}
	system_command_chars[i] = '\0';
	system(system_command_chars);
	delete[] system_command_chars;
}

void VIKA_Zero_CandidateA::decrypt(std::string file, std::string mode) {
	//Decrypt the file using the set key and either "ecb" or "cbc" mode
	//ecb: Electronic Code Book mode
	//cbc: Cipher Block Chaining mode
	if (!this->key_set) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key must be set to decrypt.");
	}
	if (mode != "ecb" && mode != "cbc") {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Invalid decrypt mode, must be \"ecb\" or \"cbc\".");
	}

	//If reverse S-Box and P-Box are not set, set them
	if (!this->reverse_set) {
		this->generate_reverse_box(this->reverse_sbox, this->sbox, 256);
		this->generate_reverse_box(this->reverse_pbox, this->pbox, 32);
		this->reverse_set = true;
	}

	//Input the file to decrypt
	std::streampos size;
	std::ifstream file_handle(file, std::ios::in | std::ios::binary | std::ios::ate);
	size = file_handle.tellg();
	std::vector <char> input_file0;
	input_file0.resize(size);
	if (file_handle.is_open()) {
		file_handle.seekg(0, std::ios::beg);
		file_handle.read(&input_file0[0], size);
		file_handle.close();
	}
	else {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Could not open file for read and decrypt.");
	}

	//Convert char to unsigned char
	std::vector <unsigned char> input_file;
	input_file.resize(size);
	std::vector <unsigned char> output_file1;
	if (mode == "ecb") {
		output_file1.resize(size);
	}
	else if (mode == "cbc") {
		output_file1.resize(size - (std::streampos)16);
	}
	for (unsigned int i = 0; i < input_file0.size(); i++) {
		input_file[i] = (reinterpret_cast<unsigned char&>(input_file0[i]));
	}

	//Decrypt in either ecb or cbc mode
	unsigned int block_count = input_file.size() / 16;
	if (mode == "ecb") {
		unsigned char ciphertext_block[16];
		unsigned int block_index = 0;
		unsigned int block_index2 = 0;
		for (unsigned int block_number = 0; block_number < block_count; block_number++) {
			for (unsigned int i = 0; i < 16; i++, block_index++) {
				ciphertext_block[i] = input_file[block_index];
			}
			this->vzca_decrypt(ciphertext_block, this->reverse_sbox, this->reverse_pbox, 2, 16, this->ks_xor_bytes);
			for (unsigned int i = 0; i < 16; i++, block_index2++) {
				output_file1[block_index2] = ciphertext_block[i];
			}
		}
	}
	else if (mode == "cbc") {
		unsigned char ciphertext_block1[16];
		unsigned char ciphertext_block2[16];
		int block_index = input_file.size() - 1;
		int block_index2 = output_file1.size() - 1;
		for (unsigned int block_number = block_count; block_number > 1; block_number--) {
			if (block_number == block_count) {
				for (int i = 15; i >= 0; i--, block_index--) {
					ciphertext_block1[i] = input_file[block_index];
				}
			}
            this->vzca_decrypt(ciphertext_block1, this->reverse_sbox, this->reverse_pbox, 2, 16, this->ks_xor_bytes);
            for(unsigned int n = 0; n < 16; n++) {
                ciphertext_block2[n] = ciphertext_block1[n];
            }
			for (int i = 15; i >= 0; i--, block_index--) {
				ciphertext_block1[i] = input_file[block_index];
			}
			for (int i = 0; i < 16; i++) {
				ciphertext_block2[i] ^= ciphertext_block1[i];
			}
			for (int i = 15; i >= 0; i--, block_index2--) {
				output_file1[block_index2] = ciphertext_block2[i];
			}
		}
	}

	//Get the padding size and remove the padding from decrypted output
	unsigned int padding_size = output_file1[output_file1.size() - 1];
	output_file1.erase(output_file1.end() - padding_size, output_file1.end());

	//Get the adjusted size minus the padding or IV for cbc mode
	std::streampos size2;
	if (mode == "ecb") {
		size2 = size - (std::streampos)padding_size;
	}
	else if (mode == "cbc") {
		size2 = size - (std::streampos)padding_size - (std::streampos)16;
	}
	std::vector <char> output_file2;
	output_file2.resize(size2);

	//Convert decrypted output in unsigned chars into chars
	for (unsigned int i = 0; i < output_file1.size(); i++) {
		output_file2[i] = (reinterpret_cast<char&>(output_file1[i]));
	}

	//Output decrypted file
	std::ofstream file2(file, std::ios::out | std::ios::binary);
	if (file2.is_open()) {
		file2.write(&output_file2[0], size2);
		file2.close();
	}
	else {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Could not open file for write after decrypt.");
	}

	//Remove the "VIKA_Zero_CandidateA" extension from the file name
	std::string new_filename = std::regex_replace(file, std::regex("(\\.VIKA_Zero_CandidateA)$"), "");
	std::string system_command = "mv " + file + " " + new_filename;
	char* system_command_chars = new char[150];
	unsigned int i = 0;
	for (; i < system_command.length(); i++) {
		system_command_chars[i] = system_command[i];
	}
	system_command_chars[i] = '\0';
	system(system_command_chars);
	delete[] system_command_chars;
}
