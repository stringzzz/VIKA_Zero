/*
    This is part of the VIKA_Zero_CandidateA encryption system, the source code for the VIKA_Zero_CandidateA exception error class
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

!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!

This system is purely for educational purposes, not actual secure purposes.
It has yet to go through the peer review process, and likely won't in its current state due to my lack
of the necessary tools needed to properly analyze it myself at this time, namely Abstract algebra, Number Theory, and Statistics.
Anyone trying to use this system for secure purposes will be subject to ridicule and much trash thrown at them.

You've been warned! 
--stringzzz, Ghostwarez Co.

!!!!!!!!!!!!!! VERY IMPORTANT !!!!!!!!!!!

*/

#ifndef VIKA_Zero_CandidateA_EXCEPTION_H
#define VIKA_Zero_CandidateA_EXCEPTION_H

#include <exception>
#include <string>

class VIKA_Zero_CandidateA_Exception : public std::exception {
private:
	std::string errorMessage;
public:
	VIKA_Zero_CandidateA_Exception(std::string errMessage) : errorMessage(errMessage) {}
	virtual const char* what() { return errorMessage.c_str(); }
};

#endif
