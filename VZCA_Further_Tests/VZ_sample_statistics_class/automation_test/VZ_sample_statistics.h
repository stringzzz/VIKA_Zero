/*

	VIKA_Zero Sample statistics class header
	By stringzzz, Ghostwarez Co.
	02-28-2025

	03-01-2025: Added code to find potential outliers

*/

#ifndef VZ_SAMPLE_STATISTICS_H
#define VZ_SAMPLE_STATISTICS_H

#include <vector>

class VZ_Sample_Statistics {

	std::vector<double> data;
	const unsigned int n_differences = 129;

private:

	//Values
	double n;
	double max_value;
	double min_value;
	double range_value;
	std::vector<double> quartiles_values;
	double iqr_value;
	std::vector<double> outlier_values;
	double mean_value;
	double median_value;
	std::vector<double> mode_values;
	double variance_value;
	double std_deviation_value;
	double kurtosis_value;
	double kurtosis_excess_value;
	double mid_range_value;
	double sum_of_squares_value;
	double mad_value;
	double rms_value;
	double std_error_of_mean_value;
	double skewness_value;
	double cv_value;
	double rsv_value;

	//Bools to check if already calculated
	bool n_calculated = false;
	bool max_calculated = false;
	bool min_calculated = false;
	bool range_calculated = false;
	bool quartiles_calculated = false;
	bool iqr_calculated = false;
	bool outliers_calculated = false;
	bool mean_calculated = false;
	bool median_calculated = false;
	bool mode_calculated = false;
	bool variance_calculated = false;
	bool std_deviation_calculated = false;
	bool kurtosis_calculated = false;
	bool kurtosis_excess_calculated = false;
	bool mid_range_calculated = false;
	bool sum_of_squares_calculated = false;
	bool mad_calculated = false;
	bool rms_calculated = false;
	bool std_error_of_mean_calculated = false;
	bool skewness_calculated = false;
	bool cv_calculated = false;
	bool rsv_calculated = false;

public:

	VZ_Sample_Statistics(std::string difference_file);
	double count();
	double max();
	double min();
	double range();
	std::vector<double> quartiles();
	double iqr();
	std::vector<double> outliers();
	double mean();
	double median();
	std::vector<double> mode();
	double variance();
	double std_deviation();
	double kurtosis();
	double kurtosis_excess();
	double mid_range();
	double sum_of_squares();
	double mad();
	double rms();
	double std_error_of_mean();
	double skewness();
	double cv();
	double rsv();

};

#endif
