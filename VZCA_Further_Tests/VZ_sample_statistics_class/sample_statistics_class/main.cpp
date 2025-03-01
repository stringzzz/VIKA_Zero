/*

	Sample statistics example&test main
	By stringzzz, Ghostwarez Co.
	02-28-2025

	03-01-2025: Added code to find potential outliers

*/

#include <iostream>
#include <vector>
#include "sample_statistics.h"

int main() {

	std::vector<double> test_data = { 5, 4, 6, 8, 8, 4, 2, 3, 8, 3, 3, 10, 1, 1, 1, 6, 5, 7, 6, 4, 5, 6, 2, 4, 8, 9, 1, 3, 5, 6, 7, 8, 12, 5, 9, 3, 4, 2, 6, 4 };

	Sample_Statistics ss1(test_data);

	std::cout << "Count: " << ss1.count() << "\n\n";
	std::cout << "Max: " << ss1.max() << "\n\n";
	std::cout << "Min: " << ss1.min() << "\n\n";
	std::cout << "Range: " << ss1.range() << "\n\n";
	
	//quartiles, create vector and print one by one
	std::vector<double> q123 = ss1.quartiles();
	std::cout << "Quartiles: ";
	for(double quartile : q123) {
		std::cout << quartile << " ";
	}
	std::cout << "\n\n";

	std::cout << "Interquartile Range: " << ss1.iqr() << "\n\n";

	//outliers, create vector and print one by one
	//Or, print "None" if empty
	std::vector<double> outliers = ss1.outliers();
	std::cout << "Outliers: ";
	if (outliers.empty()) {
		std::cout << "None";
	} else {
		for(double outlier : outliers) {
			std::cout << outlier << " ";
		}
	}
	std::cout << "\n\n";

	std::cout << "Mean: " << ss1.mean() << "\n\n";
	std::cout << "Median: " << ss1.median() << "\n\n";
	
	//Mode, create vector, print 1by1, blah
	std::vector<double> modes = ss1.mode();
	std::cout << "Mode(s): ";
	for(double mode : modes) {
		std::cout << mode << " ";
	}
	std::cout << "\n\n";
	
	std::cout << "Variance: " << ss1.variance() << "\n\n";
	std::cout << "Standard Deviation: " << ss1.std_deviation() << "\n\n";
	std::cout << "Kurtosis: " << ss1.kurtosis() << "\n\n";
	std::cout << "Kurtosis Excess: " << ss1.kurtosis_excess() << "\n\n";
	std::cout << "Mid Range: " << ss1.mid_range() << "\n\n";
	std::cout << "Sum of Squares: " << ss1.sum_of_squares() << "\n\n";
	std::cout << "Mean Absolute Deviation: " << ss1.mad() << "\n\n";
	std::cout << "Root Mean Square: " << ss1.rms() << "\n\n";
	std::cout << "Standard Error of Mean: " << ss1.std_error_of_mean() << "\n\n";
	std::cout << "Skewness: " << ss1.skewness() << "\n\n";
	std::cout << "Coefficient of Variation: " << ss1.cv() << "\n\n";
	std::cout << "Relative Standard Deviation: " << ss1.rsv() << "%\n\n";

	return 0;
}