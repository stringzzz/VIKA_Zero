/*

	VIKA_Zero Sample statistics example&test main
	By stringzzz, Ghostwarez Co. (Minus the format_double function)
	02-28-2025

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "VZ_sample_statistics.h"

std::string format_double(double value, int precision);

int main() {

	std::ofstream out_file("PAE_Distribution_Descriptive_Statistics.txt");

	if (out_file.is_open()) {
		
		VZ_Sample_Statistics ss1("PAE_Distribution1.txt");

		out_file << "Count: " << format_double(ss1.count(), 4) << "\n\n";
		out_file << "Max: " << format_double(ss1.max(), 4) << "\n\n";
		out_file << "Min: " << format_double(ss1.min(), 4) << "\n\n";
		out_file << "Range: " << format_double(ss1.range(), 4) << "\n\n";
		
		//quartiles, create vector and print one by one
		std::vector<double> q123 = ss1.quartiles();
		out_file << "Quartiles: ";
		for(double quartile : q123) {
			out_file << quartile << " ";
		}
		out_file << "\n\n";

		out_file << "Interquartile Range: " << format_double(ss1.iqr(), 4) << "\n\n";
		out_file << "Mean: " << format_double(ss1.mean(), 4) << "\n\n";
		out_file << "Median: " << format_double(ss1.median(), 4) << "\n\n";
		
		//Mode, create vector, print 1by1, blah
		std::vector<double> modes = ss1.mode();
		out_file << "Mode(s): ";
		for(double mode : modes) {
			out_file << mode << " ";
		}
		out_file << "\n\n";
		
		out_file << "Variance: " << format_double(ss1.variance(), 4) << "\n\n";
		out_file << "Standard Deviation: " << format_double(ss1.std_deviation(), 4) << "\n\n";
		out_file << "Kurtosis: " << format_double(ss1.kurtosis(), 4) << "\n\n";
		out_file << "Kurtosis Excess: " << format_double(ss1.kurtosis_excess(), 4) << "\n\n";
		out_file << "Mid Range: " << format_double(ss1.mid_range(), 4) << "\n\n";
		out_file << "Sum of Squares: " << format_double(ss1.sum_of_squares(), 4) << "\n\n";
		out_file << "Mean Absolute Deviation: " << format_double(ss1.mad(), 4) << "\n\n";
		out_file << "Root Mean Square: " << format_double(ss1.rms(), 4) << "\n\n";
		out_file << "Standard Error of Mean: " << format_double(ss1.std_error_of_mean(), 4) << "\n\n";
		out_file << "Skewness: " << format_double(ss1.skewness(), 4) << "\n\n";
		out_file << "Coefficient of Variation: " << format_double(ss1.cv(), 4) << "\n\n";
		out_file << "Relative Standard Deviation: " << format_double(ss1.rsv(), 4) << "%\n\n";

		out_file.close();
	} else {
		std::cerr << "Unable to open file for input.\n";
	}

	return 0;
}

std::string format_double(double value, int precision) {
	//Props to Gemini AI for this function, lol :P
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision) << value;
	std::string str = ss.str();
	
	// Remove trailing zeros
	if (str.find('.') != std::string::npos) {
		str.erase(str.find_last_not_of('0') + 1, std::string::npos);
		if (str.back() == '.') {
			str.pop_back();
		}
	}
	return str;
}