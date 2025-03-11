/*

	VIKA_Zero Sample statistics example&test main
	By stringzzz, Ghostwarez Co. (Minus the format_double function)
	02-28-2025

	03-01-2025: Added code to find potential outliers
	+ Command line args for input and output files

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "VZ_sample_statistics.h"

std::string format_double(double value, int precision);

int main(int argc, char *argv[]) {

	if (argc < 3) {
		std::cout << "Need input distribution (1st) and output statistics (2nd) filenames as arguments.\n";
		return 1;
	}
	const std::string input_filename = argv[1];
	const std::string output_filename = argv[2];

	std::ofstream out_file(output_filename);

	if (out_file.is_open()) {

		const unsigned int precision_value = 4;
		
		VZ_Sample_Statistics ss1(input_filename);

		out_file << "Count: " << format_double(ss1.count(), precision_value) << "\n\n";
		out_file << "Max: " << format_double(ss1.max(), precision_value) << "\n\n";
		out_file << "Min: " << format_double(ss1.min(), precision_value) << "\n\n";
		out_file << "Range: " << format_double(ss1.range(), precision_value) << "\n\n";
		
		//quartiles, create vector and print one by one
		std::vector<double> q123 = ss1.quartiles();
		out_file << "Quartiles: ";
		for(double quartile : q123) {
			out_file << quartile << " ";
		}
		out_file << "\n\n";

		out_file << "Interquartile Range: " << format_double(ss1.iqr(), precision_value) << "\n\n";

		//outliers, create vector and print one by one
		//Or, print "None" if empty
		std::vector<double> outliers = ss1.outliers();
		out_file << "Outliers: ";
		if (outliers.empty()) {
			out_file << "None";
		} else {
			for(double outlier : outliers) {
				out_file << format_double(outlier, precision_value) << " ";
			}
		}
		out_file << "\n\n";

		out_file << "Mean: " << format_double(ss1.mean(), precision_value) << "\n\n";
		out_file << "Median: " << format_double(ss1.median(), precision_value) << "\n\n";
		
		//Mode, create vector, print 1by1, blah
		std::vector<double> modes = ss1.mode();
		out_file << "Mode(s): ";
		for(double mode : modes) {
			out_file << mode << " ";
		}
		out_file << "\n\n";
		
		out_file << "Variance: " << format_double(ss1.variance(), precision_value) << "\n\n";
		out_file << "Standard Deviation: " << format_double(ss1.std_deviation(), precision_value) << "\n\n";
		out_file << "Kurtosis: " << format_double(ss1.kurtosis(), precision_value) << "\n\n";
		out_file << "Kurtosis Excess: " << format_double(ss1.kurtosis_excess(), precision_value) << "\n\n";
		out_file << "Mid Range: " << format_double(ss1.mid_range(), precision_value) << "\n\n";
		out_file << "Sum of Squares: " << format_double(ss1.sum_of_squares(), precision_value) << "\n\n";
		out_file << "Mean Absolute Deviation: " << format_double(ss1.mad(), precision_value) << "\n\n";
		out_file << "Root Mean Square: " << format_double(ss1.rms(), precision_value) << "\n\n";
		out_file << "Standard Error of Mean: " << format_double(ss1.std_error_of_mean(), precision_value) << "\n\n";
		out_file << "Skewness: " << format_double(ss1.skewness(), precision_value) << "\n\n";
		out_file << "Coefficient of Variation: " << format_double(ss1.cv(), precision_value) << "\n\n";
		out_file << "Relative Standard Deviation: " << format_double(ss1.rsv(), precision_value) << "%\n\n";

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