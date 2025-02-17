/*
    This is part of the VIKA_Zero_CandidateA encryption system, the example main for using it
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

This main file in particular is just to show how to properly use the VIKA_Zero_CandidateA class and catch any errors
Uncomment lines and compile to use the different features

!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!

This system is purely for educational purposes, not actual secure purposes.
It has yet to go through the peer review process, and likely won't in its current state due to my lack
of the necessary tools needed to properly analyze it myself at this time, namely Abstract algebra, Number Theory, and Statistics.
Anyone trying to use this system for secure purposes will be subject to ridicule and much trash thrown at them.

You've been warned! 
--stringzzz, Ghostwarez Co.

!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!

*/

#include <iostream>
#include "VIKA_Zero_CandidateA.h"

int main() {
    try {
        VIKA_Zero_CandidateA a1;

        /*
        //Example encrypt:    
        std::cout << "Encrypting...\n";
        a1.genKey();
        a1.showKey(); //Show generated key in hex
        //a1.setKeyH("e5077dce18a81e4e80a6df19b64dcf25");
        a1.encrypt("speed_test_file_100MB.txt", "ecb");
        std::cout << "Encryption complete.\n";
        */
        

        ///*
        //Example decrypt:
        std::cout << "Decrypting...\n";
        a1.setKeyH("5795dc6bad2c266b2a5a20a235801ca8");
        a1.decrypt("speed_test_file_100MB.txt.VIKA_Zero_CandidateA", "ecb");
        std::cout << "Decryption complete\n";
        //*/

        //Detect errors:
        //a1.setKeyH("e5077dce18a81e4e80a6df19b64dcf255"); //Hex key too long
        //a1.setKeyH("e5077dce18a81e4e80a6df19b64dcf2"); //Hex key too short
        
        //a1.genKey();
        //a1.encrypt("n1.txt", "abc"); //Invalid encrypt mode, must be "ecb" or "cbc"
       
    }
    catch (VIKA_Zero_CandidateA_Exception err) {
        std::cout << err.what() << "\n";
        exit(1);
    }
    return 0;
}
