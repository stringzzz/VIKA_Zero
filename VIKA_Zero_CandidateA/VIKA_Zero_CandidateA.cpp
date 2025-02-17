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
	
    Project Completed: ?(Not Exactly Done Yet)

	VIKA_Zero_CandidateA class methods

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

void VIKA_Zero_CandidateA::bytesToBits(unsigned char input[], int input_len, unsigned char bits[]) {
    //bits must be size input_len * 8!
    int i = 0;
    for (unsigned int n = 0; n < input_len; n++) {
        for (int n2 = 1; n2 <= 8; n2++) {
            bits[i] = ((unsigned char)input[n] >> (8 - n2)) & 1;
            i++;
        }
    }
}

void VIKA_Zero_CandidateA::genBox3(unsigned char out_box[], int box_size, unsigned char ks_bits[], unsigned int shuffles) {
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

void VIKA_Zero_CandidateA::sBox(unsigned char input[], int input_len, unsigned char sbox[]) {
        for(int i = 0; i < input_len; i++) {
		input[i] = sbox[input[i]];
	}
}

void VIKA_Zero_CandidateA::pBox(unsigned char output[], unsigned char input[], int input_len, unsigned char pbox[]) {
    for(int i = 0; i < input_len; i++) {
        output[pbox[i]] = input[i];
    }
}

void VIKA_Zero_CandidateA::bytesToNybbles(unsigned char input[], int input_len, unsigned char nybbles[]) {
    //nybbles must be size input_len * 2!
    int n = 0;
    for(int i = 0; i < input_len; i++) {
        nybbles[n] = input[i] >> 4; //Left nybble
        n++;
        nybbles[n] = input[i] & 15; //Right nybble
        n++;
    }
}

void VIKA_Zero_CandidateA::nybblesToBytes(unsigned char output[], int output_len, unsigned char nybbles[]) {
    //nybbles must be size output_len * 2!
    int n = 0;
    for(int i = 0; i < output_len; i++) {
        output[i] = ((unsigned char)nybbles[n] << 4) ^ (nybbles[n+1]);
        n += 2;
    }
}

void VIKA_Zero_CandidateA::apply_pbox(unsigned char block[], unsigned char pbox[]) {
    unsigned char temp_nybbles_input[32];
    bytesToNybbles(block, 16, temp_nybbles_input); //(10 * 16 + 1 + 2 * 16) * 4 cycles = 772
    unsigned char temp_nybbles_output[32];
    pBox(temp_nybbles_output, temp_nybbles_input, 32, pbox); //(1 + 2 * 32 + 4 * 32) * 4 cycles = 772
    nybblesToBytes(block, 16, temp_nybbles_output); //(1 + 2 * 16 + 7 * 16) * 4 cycles = 644
}

void VIKA_Zero_CandidateA::diffusion_slide_encrypt(unsigned char block[]) {
    for(unsigned int i = 1; i < 15; i++) { //(1 + 2 * 14) * 4 cycles = 116
        if (i == 14) { //(1 * 14) * 4 cycles = 56
            block[0] ^= block[15]; //(4 * 1) * 4 = 16
        } else {
            block[i + 1] ^= block[i]; //(5 * 13) * 4 = 260
        }
    }
}

void VIKA_Zero_CandidateA::encrypt_test4D(unsigned char block[], unsigned char sbox[], unsigned char pbox[], int pbox_cycles, int ncycles, unsigned char xor_bytes[]) {
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
            block[n] ^= xor_bytes[nxor_bytes]; //4 * 16 * 16 = 1,024
        }

        //Operations per block: 6117
    }
}

void VIKA_Zero_CandidateA::genInvertedBox(unsigned char inv_box[], unsigned char box[], int box_size) {
    //Create the inverted S-Box, P-Box, etc. using the initial box
    for(int n = 0; n < box_size; n++) {
        inv_box[box[n]] = n;
    }
}

void VIKA_Zero_CandidateA::diffusion_slide_decrypt(unsigned char block[]) {
    for(unsigned int i = 15; i >= 2; i--) {
        if (i == 15) {
            block[0] ^= block[15];
        } else {
            block[i] ^= block[i - 1];
        }
    }
}

void VIKA_Zero_CandidateA::decrypt_test4D(unsigned char block[], unsigned char inv_sbox[], unsigned char inv_pbox[], int pbox_cycles, int ncycles, unsigned char xor_bytes[]) {
    unsigned int nxor_bytes = 255; // = 1
    for(unsigned int cycles = ncycles; cycles > 0; cycles--) { //1 + 2 * 16 = 33
        //16 bytes per cycle, 16 cycles = 256 xor bytes
        for(int n = 15; n >= 0; n--, nxor_bytes--) { //(1 + 3 * 16) * 16 cycles = 784
            block[n] ^= xor_bytes[nxor_bytes]; //4 * 16 * 16 = 1,024
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
    bytesToBits(temp_ks_sbox, box_bytes, temp_ks_sbox_bits);

    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < box_bytes; n_ks++) {
        temp_ks_pbox[n_ks] = temp_key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    unsigned char temp_ks_pbox_bits[box_bytes * 8];
    bytesToBits(temp_ks_pbox, box_bytes, temp_ks_pbox_bits);

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
    genBox3(temp_sbox, 256, temp_ks_sbox_bits, 32);
    unsigned char temp_pbox[32];
    genBox3(temp_pbox, 32, temp_ks_pbox_bits, 256);

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
        encrypt_test4D(output_key_schedule[n_ks_blocks], temp_sbox, temp_pbox, 2, 16, temp_xor_bytes);

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
        this->KS_SBOX[n_ks] = key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    bytesToBits(this->KS_SBOX, box_bytes, this->KS_SBOX_BITS);

    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < box_bytes; n_ks++) {
        this->KS_PBOX[n_ks] = key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
    bytesToBits(this->KS_PBOX, box_bytes, this->KS_PBOX_BITS);

    for(unsigned int n_ks = 0, n_bytes = 0; n_ks < 256; n_ks++) {
        this->KS_XOR1[n_ks] = key_schedule[n_blocks][n_bytes];
        n_bytes++;
        if(n_bytes == 16) {
            n_bytes = 0;
            n_blocks++;
        }
    }
}

void VIKA_Zero_CandidateA::setup_boxes() {
    genBox3(this->Sbox, 256, this->KS_SBOX_BITS, 32);
    genBox3(this->Pbox, 32, this->KS_PBOX_BITS, 256);
}

void VIKA_Zero_CandidateA::key_generator1B(unsigned char original_key[]) {
        //START KEY GENERATOR
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
        //END KEY GENERATOR
}


///////////////////
//Public interface
///////////////////

VIKA_Zero_CandidateA::VIKA_Zero_CandidateA() {

}

void VIKA_Zero_CandidateA::genKey() {
	//Generate a new prng key
	//Also create the key schedule, S-Box and P-Box from it
	this->key_generator1B(this->initialKey0);
    this->key_schedule1A(this->initialKey0, this->keySchedule);
    this->distribute_key_schedule(this->keySchedule);
    this->setup_boxes();
	this->keySet = 1;
	this->reverseSet = 0;
}

void VIKA_Zero_CandidateA::setKeyS(std::string keyString) {
	//Set the key as a 16 character string
	//Also create the key schedule, S-Box and P-Box from it
	if (keyString.length() > 16) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key as string must be exactly 16 characters, input key is greater then 16 characters.");
	}
	if (keyString.length() < 16) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key as string must be exactly 16 characters, input key is less then 16 characters.");
	}

	for (unsigned int i = 0; i < 16; i++) {
		this->initialKey0[i] = (unsigned char)keyString[i];
	}
    this->key_schedule1A(this->initialKey0, this->keySchedule);
    this->distribute_key_schedule(this->keySchedule);
    this->setup_boxes();
	this->keySet = 1;
	this->reverseSet = 0;
}

void VIKA_Zero_CandidateA::setKeyH(std::string hexString) {
	//Set the key as a 32 digit hexadecimal string
	//Also create the key schedule, S-Box and P-Box from it
	if (hexString.length() > 32) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key as hex string must be exactly 32 digits, input key is greater than 32 digits.");
	}
	if (hexString.length() < 32) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key as hex string must be exactly 32 digits, input key is less than 32 digits.");
	}

	unsigned int j = 0;
	for (size_t i = 0; i < 32; i += 2, j++) {
		std::istringstream strm(hexString.substr(i, 2));
		int h;
		strm >> std::hex >> h;
		this->initialKey0[j] = h;
	}
    this->key_schedule1A(this->initialKey0, this->keySchedule);
    this->distribute_key_schedule(this->keySchedule);
    this->setup_boxes();
	this->keySet = 1;
	this->reverseSet = 0;
}

void VIKA_Zero_CandidateA::showKey() {
	//Output the key as a 32 digit hexadecimal string to the console
	std::cout << "Key: ";
	//Store old cout state:
	std::ios oldState(nullptr);
	oldState.copyfmt(std::cout);

	//Output hexadecimal digits to console:
	std::cout << std::hex << std::setfill('0');
	for (unsigned int i = 0; i < 16; i++) {
		std::cout << std::setw(2) << (int)this->initialKey0[i];
	}
	std::cout << "\n";

	//Restore old cout state
	std::cout.copyfmt(oldState);
}

void VIKA_Zero_CandidateA::encrypt(std::string file, std::string mode) {
	//Encrypt the file using the set key and either "ecb" or "cbc" mode
	//ecb: Electronic Code Book mode
	//cbc: Cipher Block Chaining mode
	if (!keySet) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key must be set to encrypt.");
	}
	if (mode != "ecb" && mode != "cbc") {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Invalid encrypt mode, must be \"ecb\" or \"cbc\".");
	}

	//Input file as chars
	std::streampos size;
	std::ifstream fileHandle(file, std::ios::in | std::ios::binary | std::ios::ate);
	size = fileHandle.tellg();
	std::vector <char> inputFile0;
	inputFile0.resize(size);
	if (fileHandle.is_open()) {
		fileHandle.seekg(0, std::ios::beg);
		fileHandle.read(&inputFile0[0], size);
		fileHandle.close();
	}
	else {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Could not open file for read and encrypt.");
	}

	//Convert input to unsigned chars
	std::vector <unsigned char> inputFile;
	inputFile.resize(size);
	for (unsigned int i = 0; i < inputFile0.size(); i++) {
		inputFile[i] = (reinterpret_cast<unsigned char&>(inputFile0[i]));
	}

	//Use padding to make 16n blocks even
	unsigned int paddingSize = 16 - (inputFile.size() % 16);
	for (unsigned int i = 0; i < paddingSize; i++) {
		inputFile.push_back((unsigned char)paddingSize);
	}

	//Resize output vector
	std::vector <unsigned char> outputFile1;
	if (mode == "ecb") {
		outputFile1.resize(size + (std::streampos)paddingSize);
	}
	else if (mode == "cbc") {
		outputFile1.resize(size + (std::streampos)paddingSize + (std::streampos)16);
	}

	//Encrypt either ecb or cbc mode
	unsigned blockCount = inputFile.size() / 16;
	if (mode == "ecb") {
		unsigned char plaintextBlock[16];
		unsigned int blockIndex = 0;
		unsigned int blockIndex2 = 0;
		for (unsigned int blockNumber = 0; blockNumber < blockCount; blockNumber++) {
			for (unsigned int i = 0; i < 16; i++, blockIndex++) {
				plaintextBlock[i] = inputFile[blockIndex];
			}
			this->encrypt_test4D(plaintextBlock, this->Sbox, this->Pbox, 2, 16, this->KS_XOR1);
			for (unsigned int i = 0; i < 16; i++, blockIndex2++) {
				outputFile1[blockIndex2] = plaintextBlock[i];
			}
		}
	}
	else if (mode == "cbc") {
		unsigned char plaintextBlock2[16];
        this->key_generator1B(plaintextBlock2);
		unsigned char plaintextBlock1[16];
		unsigned int blockIndex = 0;
		unsigned int blockIndex2 = 0;
		for (unsigned int blockNumber = 1; blockNumber <= blockCount; blockNumber++) {
			for (unsigned int i = 0; i < 16; i++, blockIndex++) {
				plaintextBlock1[i] = inputFile[blockIndex];
			}
			for (unsigned int i = 0; i < 16; i++) {
				plaintextBlock1[i] ^= plaintextBlock2[i];
			}

			for (unsigned int i = 0; i < 16; i++, blockIndex2++) {
				outputFile1[blockIndex2] = plaintextBlock2[i];
			}
            this->encrypt_test4D(plaintextBlock1, this->Sbox, this->Pbox, 2, 16, this->KS_XOR1);
            for(unsigned int n = 0; n < 16; n++) {
			    plaintextBlock2[n] = plaintextBlock1[n];
            }

		}
		for (unsigned int i = 0; i < 16; i++, blockIndex2++) {
			outputFile1[blockIndex2] = plaintextBlock2[i];
		}

	}

	//Make char output for file write
	std::vector <char> outputFile2;
	std::streampos size2;
	if (mode == "ecb") {
		size2 = size + (std::streampos)paddingSize;
	}
	else if (mode == "cbc") {
		size2 = size + (std::streampos)paddingSize + (std::streampos)16;
	}
	outputFile2.resize(size2);

	//Convert unsigned char to char
	for (unsigned int i = 0; i < outputFile1.size(); i++) {
		outputFile2[i] = (reinterpret_cast<char&>(outputFile1[i]));
	}

	//Output the encrypted file bytes
	std::ofstream file2(file, std::ios::out | std::ios::binary);
	if (file2.is_open()) {
		file2.write(&outputFile2[0], size2);
		file2.close();
	}
	else {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Could not open file for write after encrypt.");
	}

	//Rename the encrypted file with the new extension
	std::string newFileName = file + ".VIKA_Zero_CandidateA";
	std::string sysOut = "mv " + file + " " + newFileName;
	char* sysOutc = new char[150];
	unsigned int i = 0;
	for (; i < sysOut.length(); i++) {
		sysOutc[i] = sysOut[i];
	}
	sysOutc[i] = '\0';
	system(sysOutc);
	delete[] sysOutc;
}

void VIKA_Zero_CandidateA::decrypt(std::string file, std::string mode) {
	//Decrypt the file using the set key and either "ecb" or "cbc" mode
	//ecb: Electronic Code Book mode
	//cbc: Cipher Block Chaining mode
	if (!this->keySet) {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Key must be set to decrypt.");
	}
	if (mode != "ecb" && mode != "cbc") {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Invalid decrypt mode, must be \"ecb\" or \"cbc\".");
	}

	//If reverse S-Box and P-Box are not set, set them
	if (!this->reverseSet) {
		this->genInvertedBox(this->revSbox, this->Sbox, 256);
		this->genInvertedBox(this->revPbox, this->Pbox, 32);
		this->reverseSet = 1;
	}

	//Input the file to decrypt
	std::streampos size;
	std::ifstream fileHandle(file, std::ios::in | std::ios::binary | std::ios::ate);
	size = fileHandle.tellg();
	std::vector <char> inputFile0;
	inputFile0.resize(size);
	if (fileHandle.is_open()) {
		fileHandle.seekg(0, std::ios::beg);
		fileHandle.read(&inputFile0[0], size);
		fileHandle.close();
	}
	else {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Could not open file for read and encrypt.");
	}

	//Convert char to unsigned char
	std::vector <unsigned char> inputFile;
	inputFile.resize(size);
	std::vector <unsigned char> outputFile1;
	if (mode == "ecb") {
		outputFile1.resize(size);
	}
	else if (mode == "cbc") {
		outputFile1.resize(size - (std::streampos)16);
	}
	for (unsigned int i = 0; i < inputFile0.size(); i++) {
		inputFile[i] = (reinterpret_cast<unsigned char&>(inputFile0[i]));
	}

	//Decrypt in either ecb or cbc mode
	unsigned int blockCount = inputFile.size() / 16;
	if (mode == "ecb") {
		unsigned char ciphertextBlock[16];
		unsigned int blockIndex = 0;
		unsigned int blockIndex2 = 0;
		for (unsigned int blockNumber = 0; blockNumber < blockCount; blockNumber++) {
			for (unsigned int i = 0; i < 16; i++, blockIndex++) {
				ciphertextBlock[i] = inputFile[blockIndex];
			}
			this->decrypt_test4D(ciphertextBlock, this->revSbox, this->revPbox, 2, 16, this->KS_XOR1);
			for (unsigned int i = 0; i < 16; i++, blockIndex2++) {
				outputFile1[blockIndex2] = ciphertextBlock[i];
			}
		}
	}
	else if (mode == "cbc") {
		unsigned char ciphertextBlock1[16];
		unsigned char ciphertextBlock2[16];
		int blockIndex = inputFile.size() - 1;
		int blockIndex2 = outputFile1.size() - 1;
		for (unsigned int blockNumber = blockCount; blockNumber > 1; blockNumber--) {
			if (blockNumber == blockCount) {
				for (int i = 15; i >= 0; i--, blockIndex--) {
					ciphertextBlock1[i] = inputFile[blockIndex];
				}
			}
            this->decrypt_test4D(ciphertextBlock1, this->revSbox, this->revPbox, 2, 16, this->KS_XOR1);
            for(unsigned int n = 0; n < 16; n++) {
                ciphertextBlock2[n] = ciphertextBlock1[n];
            }
			for (int i = 15; i >= 0; i--, blockIndex--) {
				ciphertextBlock1[i] = inputFile[blockIndex];
			}
			for (int i = 0; i < 16; i++) {
				ciphertextBlock2[i] ^= ciphertextBlock1[i];
			}
			for (int i = 15; i >= 0; i--, blockIndex2--) {
				outputFile1[blockIndex2] = ciphertextBlock2[i];
			}
		}
	}

	//Get the padding size and remove the padding from decrypted output
	unsigned int paddingSize = outputFile1[outputFile1.size() - 1];
	outputFile1.erase(outputFile1.end() - paddingSize, outputFile1.end());

	//Get the adjusted size minus the padding or IV for cbc mode
	std::streampos size2;
	if (mode == "ecb") {
		size2 = size - (std::streampos)paddingSize;
	}
	else if (mode == "cbc") {
		size2 = size - (std::streampos)paddingSize - (std::streampos)16;
	}
	std::vector <char> outputFile2;
	outputFile2.resize(size2);

	//Convert decrypted output in unsigned chars into chars
	for (unsigned int i = 0; i < outputFile1.size(); i++) {
		outputFile2[i] = (reinterpret_cast<char&>(outputFile1[i]));
	}

	//Output decrypted file
	std::ofstream file2(file, std::ios::out | std::ios::binary);
	if (file2.is_open()) {
		file2.write(&outputFile2[0], size2);
		file2.close();
	}
	else {
		throw VIKA_Zero_CandidateA_Exception("VIKA_Zero_CandidateA: Could not open file for write after decrypt.");
	}

	//Remove the "VIKA_Zero_CandidateA" extension from the file name
	std::string newFileName = std::regex_replace(file, std::regex("(\\.VIKA_Zero_CandidateA)$"), "");
	std::string sysOut = "mv " + file + " " + newFileName;
	char* sysOutc = new char[150];
	unsigned int i = 0;
	for (; i < sysOut.length(); i++) {
		sysOutc[i] = sysOut[i];
	}
	sysOutc[i] = '\0';
	system(sysOutc);
	delete[] sysOutc;
}
