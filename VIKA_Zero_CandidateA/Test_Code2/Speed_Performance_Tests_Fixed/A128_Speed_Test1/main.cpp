/*
    This is part of the ANGELITA128 encryption system, the example main for using it
    Copyright (C) 2022 stringzzz, Ghostwarez Co.

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

This main file in particular is just to show how to properly use the ANGELITA128 class and catch any errors
Uncomment lines and compile to use the different features

I just supplied the encryption system, it is up to the user to create a more useful interface or 
do essentially whatever they want with it.

!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!
Also to note, this system hasn't gone through any kind of proper peer review process yet, so it should not be used
for any real secure purposes. You have been warned!
!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!

*/

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Key Schedule	Bytes	% Key	Key Bytes	Key Bits
;
; S-Box		1216	59.375	9.5		76
; P-Box		320	15.625	2.5		20
; XOR1		256	12.5	2		16
; XOR2		256	12.5	2		16
; Total		2048	100	16		128
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;; ANGELITA128 Algorithm ;;;;;;;;;;;;;;;;;;;
; 1. Choose e/d (encryption/decryption)
; 2. Choose the key option
; 3. Input or generated key is expanded 128 times by
;	the key schedule (KISS)
; 4. The Key Schedule is split, some bytes used
;	to initialize the S-Box and P-Box. The rest is
;	used in the encryption/decryption loop
; 5. The encryption/decryption loop works on 128-Bit
;	blocks, for 16 cycles. Cycle below (Encryption):
;
;	b. XOR with KS 1
;	c. S-Box
;	d. XOR with KS 2
;	e. If cycles is multiple of 2, P-Box the pairs of bits of the block
;
;	Decryption is simply the reverse of this
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#include <iostream>
#include "ANGELITA128.h"

int main() {
    try {
        srand(time(0)); //Do here, not in functions
        ANGELITA128 a1;

        /*
        //Example encrypt:    
        std::cout << "Encrypting...\n";
        a1.genKey();  //GLORIA PRNG Key
        a1.showKey(); //Show generated key in hex
        //a1.setKeyH("e5077dce18a81e4e80a6df19b64dcf25");
        a1.encrypt("speed_test_file_100MB.txt", "ecb");
        std::cout << "Encryption complete.\n";
        */
        

        ///*
        //Example decrypt:
        std::cout << "Decrypting...\n";
        a1.setKeyH("964a944c50414295b94c20fb479add3c");
        a1.decrypt("speed_test_file_100MB.txt.ANGELITA128", "ecb");
        std::cout << "Decryption complete\n";
        //*/

        //Detect errors:
        //a1.setKeyH("e5077dce18a81e4e80a6df19b64dcf255"); //Hex key too long
        //a1.setKeyH("e5077dce18a81e4e80a6df19b64dcf2"); //Hex key too short
        //a1.encrypt("n1.txt", "abc"); //Invalid encrypt mode, must be "ecb" or "cbc"
       
    }
    catch (ANGELITA128_Exception err) {
        std::cout << err.what() << "\n";
        exit(1);
    }
    return 0;
}
