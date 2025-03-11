/*

	VIKA_Zero Sample statistics class methods
	By stringzzz, Ghostwarez Co.
	02-28-2025

	03-01-2025: Added code to find potential outliers
	
*/

#include <cmath>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "VZ_sample_statistics.h"

VZ_Sample_Statistics::VZ_Sample_Statistics(std::string difference_file) { 
	std::ifstream file(difference_file);
	std::string line;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			double num;

			while (ss >> num) {
				ss >> num;
				data.push_back(num);
			}
			
		}
		file.close();
	} else {
		std::cerr << "Unable to open file for input.\n";
	}
}

double VZ_Sample_Statistics::count() {
	if (!this->n_calculated) {
		this->n = 0;
		for(unsigned int i = 0; i < n_differences; i++) {
			this->n += data[i];
		}
		this->n_calculated = true;
	}

	return this->n;
}

double VZ_Sample_Statistics::min() { 
	if (!this->min_calculated) {
		unsigned int i = 0;
		while(data[i] == 0) {
			i++;
		}
		this->min_value = i;
		this->min_calculated = true;
	}

	return this->min_value;
}

double VZ_Sample_Statistics::max() {
	if (!this->max_calculated) {
		unsigned int i = n_differences - 1;
		while(data[i] == 0) {
			i--;
		}
		this->max_value = i;
		this->max_calculated = true;
	}

	return this->max_value;
}

double VZ_Sample_Statistics::range() { 
	if (!this->range_calculated) {
		if (!this->max_calculated) {
			max();
		}
		if (!this->min_calculated) {
			min();
		}

		this->range_value = this->max_value - this->min_value;
		this->range_calculated = true;
	}

	return this->range_value;
}

std::vector<double> VZ_Sample_Statistics::quartiles() { 
	//NOTE: Might be a cleaner way to do this
	if (!this->quartiles_calculated) {

		double tempQ1 = ((n_differences + 1.0) / 4.0) - 1;
		if (fabs(tempQ1 - trunc(tempQ1)) > 0.0) {
			this->quartiles_values.push_back(((double)floor(tempQ1) + ceil(tempQ1)) / 2.0);
		} else {
			this->quartiles_values.push_back(tempQ1);
		}

		double tempQ2 = ((n_differences + 1.0) / 2.0) - 1;
		if (fabs(tempQ2 - trunc(tempQ2)) > 0.0) {
			this->quartiles_values.push_back((double)(floor(tempQ2) + ceil(tempQ2)) / 2.0);
		} else {
			this->quartiles_values.push_back(tempQ2);
		}

		double tempQ3 = (3.0 * (n_differences + 1.0) / 4.0) - 1;
		if (fabs(tempQ3 - trunc(tempQ3)) > 0.0) {
			this->quartiles_values.push_back(((double)floor(tempQ3) + ceil(tempQ3)) / 2.0);
		} else {
			this->quartiles_values.push_back(tempQ3);
		}

		this->quartiles_calculated = true;
	}

	return this->quartiles_values;
}

double VZ_Sample_Statistics::iqr() { 
	if (!this->iqr_calculated) {
		if (!this->quartiles_calculated) {
			quartiles();
		}

		this->iqr_value = this->quartiles_values[2] - this->quartiles_values[0];
		this->iqr_calculated = true;
	}

	return this->iqr_value;
}

std::vector<double> VZ_Sample_Statistics::outliers() {
	if (!outliers_calculated) {
		if (!iqr_calculated) {
			iqr();
		}

		double lower_fence = this->quartiles_values[0] - 1.5 * this->iqr_value;
		double upper_fence = this->quartiles_values[2] + 1.5 * this->iqr_value;
		for(unsigned int i = 0; i < n_differences; i++) {
			if (i >= lower_fence) {
				break;
			} else {
				this->outlier_values.push_back(i);
			}
		}
		for(unsigned int i = this->n_differences - 1; i > upper_fence; i--) {
			if (i > upper_fence) {
				this->outlier_values.push_back(i);
			}
		}

		this->outliers_calculated = true;
	}

	return this->outlier_values;
}

double VZ_Sample_Statistics::mean() { 
	if (!this->mean_calculated) {
		if (!this->n_calculated) {
			count();
		}

		double sum = 0;
		for(unsigned int i = 0; i < n_differences; i++) {
			sum += data[i] * i;
		}
		this->mean_value = sum / this->n;
		this->mean_calculated = true;
	}

	return this->mean_value;
}

double VZ_Sample_Statistics::median() { 
	if (!this->median_calculated) {

		if ((unsigned int)n_differences % 2 == 0) {
			this->median_value = ((n_differences / 2 - 1) + (n_differences / 2)) / 2.0;
		} else {
			this->median_value = n_differences / 2; 
		}
		this->median_calculated = true;
	}

	return this->median_value;
}

std::vector<double> VZ_Sample_Statistics::mode() { 
	if (!this->mode_calculated) {

		double max_count = 0;
		for(double i = 0; i < n_differences; i++) {
			if (data[i] > max_count) {
				
				max_count = data[i];
				this->mode_values.clear();
				this->mode_values.push_back(i);

			} else if (data[i] == max_count) {

				bool duplicate_found = false;
				for(double value : this->mode_values) {
					if (value == i) {
						duplicate_found = true;
						break;
					}
				}
				if (!duplicate_found) {
					this->mode_values.push_back(i);
				}

			}
		}
		this->mode_calculated = true;
	}

	return this->mode_values;
}

double VZ_Sample_Statistics::variance() { 
	if (!this->variance_calculated) {
		if (!this->sum_of_squares_calculated) {
			sum_of_squares();
		}
		if (!this->n_calculated) {
			count();
		}

		this->variance_value = this->sum_of_squares_value / (this->n - 1.0);
		this->variance_calculated = true;
	}

	return this->variance_value;
}

double VZ_Sample_Statistics::std_deviation() { 
	if (!this->std_deviation_calculated) {
		if (!this->sum_of_squares_calculated) {
			sum_of_squares();
		}
		if (!this->n_calculated) {
			count();
		}

		this->std_deviation_value = sqrt(this->sum_of_squares_value / (this->n - 1.0));
		this->std_deviation_calculated = true;
	}

	return this->std_deviation_value;
}

double VZ_Sample_Statistics::kurtosis() { 
	if (!this->kurtosis_calculated) {
		if (!this->std_deviation_calculated) {
			this->std_deviation();
		}

		double fourth_sums = 0;
		for(unsigned int i = 0; i < n_differences; i++) {
			fourth_sums += pow(i - this->mean_value, 4) * data[i];
		}
		double temp = (this->n * (this->n + 1.0)) / ((this->n - 1.0) * (this->n - 2.0) * (this->n - 3.0));
		this->kurtosis_value = (temp * (fourth_sums / pow(this->std_deviation_value, 4)));
		this->kurtosis_calculated = true;		
	}

	return this->kurtosis_value;
}

double VZ_Sample_Statistics::kurtosis_excess() { 
	if (!this->kurtosis_excess_calculated) {
		if (!this->kurtosis_calculated) {
			kurtosis();
		}

		double temp2 = (3.0 * pow(this->n - 1.0, 2.0)) / ((this->n - 2.0) * (this->n - 3.0));
		this->kurtosis_excess_value = this->kurtosis_value - temp2;
		this->kurtosis_excess_calculated = true;
	}

	return this->kurtosis_excess_value;
}

double VZ_Sample_Statistics::mid_range() { 
	if (!this->mid_range_calculated) {
		if (!this->max_calculated) {
			max();
		}
		if (!this->min_calculated) {
			min();
		}

		this->mid_range_value = ( this->min_value + this->max_value ) / 2.0;
		this->mid_range_calculated = true;
	}

	return this->mid_range_value;
}

double VZ_Sample_Statistics::sum_of_squares() { 
	if (!this->sum_of_squares_calculated) {
		if (!this->mean_calculated) {
			mean();
		}

		this->sum_of_squares_value = 0;
		for(unsigned int i = 0; i < n_differences; i++) {
			this->sum_of_squares_value += pow(i - this->mean_value, 2) * data[i];
		}
		this->sum_of_squares_calculated = true;
	}

	return this->sum_of_squares_value;
}

double VZ_Sample_Statistics::mad() { 
	if (!this->mad_calculated) {
		if(!this->mean_calculated) {
			mean();
		}

		double absolute_sums = 0;
		for(unsigned int i = 0; i < n_differences; i++) {
			absolute_sums += fabs(i - this->mean_value) * data[i];
		}
		this->mad_value = absolute_sums / this->n;
		this->mad_calculated = true;
	}

	return this->mad_value;
}

double VZ_Sample_Statistics::rms() { 
	if (!this->rms_calculated) {
		if (!this->n_calculated) {
			count();
		}

		double squared_sums = 0;
		for(unsigned int i = 0; i < n_differences; i++) {
			squared_sums += pow(i, 2) * data[i];
		}
		this->rms_value = sqrt(squared_sums / this->n);
		this->rms_calculated = true;
	}

	return this->rms_value;
}

double VZ_Sample_Statistics::std_error_of_mean() { 
	if (!this->std_error_of_mean_calculated) {
		if (!this->std_deviation_calculated) {
			std_deviation();
		}

		this->std_error_of_mean_value = this->std_deviation_value / sqrt(this->n);
		this->std_error_of_mean_calculated = true;
	}

	return this->std_error_of_mean_value;
}

double VZ_Sample_Statistics::skewness() { 
	if (!this->skewness_calculated) {
		if (!this->std_deviation_calculated) {
			std_deviation();
		}

		double cubed_difference_sums = 0;
		for(unsigned int i = 0; i < n_differences; i++) {
			cubed_difference_sums += pow(i - this->mean_value, 3) * data[i];
		}
		double temp3 = this->n / ((this->n - 1.0) * (this->n - 2.0));
		this->skewness_value = temp3 * (cubed_difference_sums / pow(this->std_deviation_value, 3));
		this->skewness_calculated = true;
	}

	return this->skewness_value;
}

double VZ_Sample_Statistics::cv() { 
	if (!this->cv_calculated) {
		if (!this->std_deviation_calculated) {
			std_deviation();
		}

		this->cv_value = this->std_deviation_value / this->mean_value;
		this->cv_calculated = true;
	}

	return this->cv_value;
}

double VZ_Sample_Statistics::rsv() { 
	if (!this->rsv_calculated) {
		if (!cv_calculated) {
			cv();
		}

		this->rsv_value = this->cv_value * 100;
		this->rsv_calculated = true;
	}

	return this->rsv_value; //Tack on % when outputting to console or other text output
}