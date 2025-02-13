/*
    This is part of the VIKA_Zero_CandidateA encryption system, the source code file for the VIKA_Zero_CandidateA class header
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
	Project Completed: ?(Not Exactly Done Yet)

	VIKA_Zero_CandidateA class

!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!

This system is purely for educational purposes, not actual secure purposes.
It has yet to go through the peer review process, and likely won't in its current state due to my lack
of the necessary tools needed to properly analyze it myself at this time, namely Abstract algebra, Number Theory, and Statistics.
Anyone trying to use this system for secure purposes will be subject to ridicule and much trash thrown at them.

You've been warned! 
--stringzzz, Ghostwarez Co.

!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!

*/

#ifndef VIKA_ZERO_CANDIDATEA_H
#define VIKA_ZERO_CANDIDATEA_H

#include "VIKA_Zero_CandidateA_Exception.h"
#include <vector>

class VIKA_Zero_CandidateA {
private:
	unsigned char initialKey0[16];
	unsigned char initialKey1[16];
	unsigned char keySchedule[528][16];
	unsigned char Sbox[256];
	unsigned char Pbox[32];
	unsigned char revSbox[256];
	unsigned char revPbox[32];
	unsigned char KS_SBOX[4096];
	unsigned char KS_SBOX_BITS[4096 * 8];
	unsigned char KS_PBOX[4096];
	unsigned char KS_PBOX_BITS[4096 * 8];
	unsigned char KS_XOR1[256];
	bool keySet = 0;
	bool reverseSet = 0;

    void bytesToBits(unsigned char input[], int input_len, unsigned char bits[]);
    void genBox3(unsigned char out_box[], int box_size, unsigned char ks_bits[], unsigned int shuffles);
    void sBox(unsigned char input[], int input_len, unsigned char sbox[]);
    void pBox(unsigned char output[], unsigned char input[], int input_len, unsigned char pbox[]);
    void bytesToNybbles(unsigned char input[], int input_len, unsigned char nybbles[]);
    void nybblesToBytes(unsigned char output[], int output_len, unsigned char nybbles[]);
    void apply_pbox(unsigned char block[], unsigned char pbox[]);
    void diffusion_slide_encrypt(unsigned char block[]);
    void encrypt_test4D(unsigned char block[], unsigned char sbox[], unsigned char pbox[], int pbox_cycles, int ncycles, unsigned char xor_bytes[]);
    void genInvertedBox(unsigned char inv_box[], unsigned char box[], int box_size);
    void diffusion_slide_decrypt(unsigned char block[]);
    void decrypt_test4D(unsigned char block[], unsigned char inv_sbox[], unsigned char inv_pbox[], int pbox_cycles, int ncycles, unsigned char xor_bytes[]);
    void key_schedule1A(unsigned char input_key[], unsigned char output_key_schedule[528][16]);
    void distribute_key_schedule(unsigned char key_schedule[528][16]);
    void setup_boxes();
    void key_generator1B(unsigned char original_key[]);



public:
	VIKA_Zero_CandidateA();

	//Public interface
	void genKey();
	void setKeyS(std::string keyString);
	void setKeyH(std::string hexString);
	void showKey();
	void encrypt(std::string file, std::string mode);
	void decrypt(std::string file, std::string mode);

};

#endif
