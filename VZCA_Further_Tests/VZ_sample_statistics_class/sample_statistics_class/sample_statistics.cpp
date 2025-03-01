/*

	Sample statistics class methods
	By stringzzz, Ghostwarez Co.
	02-28-2025

*/

#include <cmath>
#include <algorithm>
#include <map>
#include "sample_statistics.h"

Sample_Statistics::Sample_Statistics(std::vector<double> input_data) : data(std::move(input_data)) { 
	std::sort(data.begin(), data.end());
}

double Sample_Statistics::count() {
	if (!this->n_calculated) {
		this->n = data.size();
		this->n_calculated = true;
	}

	return this->n;
}

double Sample_Statistics::max() { 
	if (!this->max_calculated) {
		this->max_value = data[data.size() - 1];
		this->max_calculated = true;
	}

	return this->max_value;
}

double Sample_Statistics::min() {
	if (!this->min_calculated) {
		this->min_value = data[0];
		this->min_calculated = true;
	}

	return this->min_value;
}

double Sample_Statistics::range() { 
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

std::vector<double> Sample_Statistics::quartiles() { 
	//NOTE: Might be a cleaner way to do this
	if (!this->quartiles_calculated) {
		if (!this->n_calculated) {
			count();
		}

		double tempQ1 = ((this->n + 1.0) / 4.0) - 1;
		if (fabs(tempQ1 - trunc(tempQ1)) > 0.0) {
			this->quartiles_values.push_back(((double)data[floor(tempQ1)] + data[ceil(tempQ1)]) / 2.0);
		} else {
			this->quartiles_values.push_back(data[tempQ1]);
		}

		double tempQ2 = ((this->n + 1.0) / 2.0) - 1;
		if (fabs(tempQ2 - trunc(tempQ2)) > 0.0) {
			this->quartiles_values.push_back((double)(data[floor(tempQ2)] + data[ceil(tempQ2)]) / 2.0);
		} else {
			this->quartiles_values.push_back(data[tempQ2]);
		}

		double tempQ3 = (3.0 * (this->n + 1.0) / 4.0) - 1;
		if (fabs(tempQ3 - trunc(tempQ3)) > 0.0) {
			this->quartiles_values.push_back(((double)data[floor(tempQ3)] + data[ceil(tempQ3)]) / 2.0);
		} else {
			this->quartiles_values.push_back(data[tempQ3]);
		}

		this->quartiles_calculated = true;
	}

	return this->quartiles_values;
}

double Sample_Statistics::iqr() { 
	if (!this->iqr_calculated) {
		if (!this->quartiles_calculated) {
			quartiles();
		}

		this->iqr_value = this->quartiles_values[2] - this->quartiles_values[0];
		this->iqr_calculated = true;
	}

	return this->iqr_value;
}

double Sample_Statistics::mean() { 
	if (!this->mean_calculated) {
		if (!this->n_calculated) {
			count();
		}

		double sum = 0;
		for(double data_value : data) {
			sum += data_value;
		}
		this->mean_value = sum / this->n;
		this->mean_calculated = true;
	}

	return this->mean_value;
}

double Sample_Statistics::median() { 
	if (!this->median_calculated) {
		if (!this->n_calculated) {
			count();
		}

		if ((unsigned int)this->n % 2 == 0) {
			this->median_value = ((data[this->n / 2 - 1]) + (data[this->n / 2])) / 2.0;
		} else {
			this->median_value = data[this->n / 2]; 
		}
		this->median_calculated = true;
	}

	return this->median_value;
}

std::vector<double> Sample_Statistics::mode() { 
	if (!this->mode_calculated) {
		if (!this->n_calculated) {
			count();
		}

		std::map<double, double> counts;
		double n_counts = 0;
		for(double data_value : data) {
			counts[data_value]++;
		}

		double max_count = 0;
		for(double i = 0; i < this->n; i++) {
			if (counts[data[i]] > max_count) {
				
				max_count = counts[data[i]];
				this->mode_values.clear();
				this->mode_values.push_back(data[i]);

			} else if (counts[data[i]] == max_count) {

				bool duplicate_found = false;
				for(double value : this->mode_values) {
					if (value == data[i]) {
						duplicate_found = true;
						break;
					}
				}
				if (!duplicate_found) {
					this->mode_values.push_back(data[i]);
				}

			}
		}
		this->mode_calculated = true;
	}

	return this->mode_values;
}

double Sample_Statistics::variance() { 
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

double Sample_Statistics::std_deviation() { 
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

double Sample_Statistics::kurtosis() { 
	if (!this->kurtosis_calculated) {
		if (!this->std_deviation_calculated) {
			this->std_deviation();
		}

		double fourth_sums = 0;
		for(double data_value : data) {
			fourth_sums += pow(data_value - this->mean_value, 4);
		}
		double temp = (this->n * (this->n + 1.0)) / ((this->n - 1.0) * (this->n - 2.0) * (this->n - 3.0));
		this->kurtosis_value = (temp * (fourth_sums / pow(this->std_deviation_value, 4)));
		this->kurtosis_calculated = true;		
	}

	return this->kurtosis_value;
}

double Sample_Statistics::kurtosis_excess() { 
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

double Sample_Statistics::mid_range() { 
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

double Sample_Statistics::sum_of_squares() { 
	if (!this->sum_of_squares_calculated) {
		if (!this->mean_calculated) {
			mean();
		}

		this->sum_of_squares_value = 0;
		for(double data_value : data) {
			this->sum_of_squares_value += pow(data_value - this->mean_value, 2);
		}
		this->sum_of_squares_calculated = true;
	}

	return this->sum_of_squares_value;
}

double Sample_Statistics::mad() { 
	if (!this->mad_calculated) {
		if(!this->mean_calculated) {
			mean();
		}

		double absolute_sums = 0;
		for(double data_value : data) {
			absolute_sums += fabs(data_value - this->mean_value);
		}
		this->mad_value = absolute_sums / this->n;
		this->mad_calculated = true;
	}

	return this->mad_value;
}

double Sample_Statistics::rms() { 
	if (!this->rms_calculated) {
		if (!this->n_calculated) {
			count();
		}

		double squared_sums = 0;
		for(double data_value : data) {
			squared_sums += pow(data_value, 2);
		}
		this->rms_value = sqrt(squared_sums / this->n);
		this->rms_calculated = true;
	}

	return this->rms_value;
}

double Sample_Statistics::std_error_of_mean() { 
	if (!this->std_error_of_mean_calculated) {
		if (!this->std_deviation_calculated) {
			std_deviation();
		}

		this->std_error_of_mean_value = this->std_deviation_value / sqrt(this->n);
		this->std_error_of_mean_calculated = true;
	}

	return this->std_error_of_mean_value;
}

double Sample_Statistics::skewness() { 
	if (!this->skewness_calculated) {
		if (!this->std_deviation_calculated) {
			std_deviation();
		}

		double cubed_difference_sums = 0;
		for(double data_value : data) {
			cubed_difference_sums += pow(data_value - this->mean_value, 3);
		}
		double temp3 = this->n / ((this->n - 1.0) * (this->n - 2.0));
		this->skewness_value = temp3 * (cubed_difference_sums / pow(this->std_deviation_value, 3));
		this->skewness_calculated = true;
	}

	return this->skewness_value;
}

double Sample_Statistics::cv() { 
	if (!this->cv_calculated) {
		if (!this->std_deviation_calculated) {
			std_deviation();
		}

		this->cv_value = this->std_deviation_value / this->mean_value;
		this->cv_calculated = true;
	}

	return this->cv_value;
}

double Sample_Statistics::rsv() { 
	if (!this->rsv_calculated) {
		if (!cv_calculated) {
			cv();
		}

		this->rsv_value = this->cv_value * 100;
		this->rsv_calculated = true;
	}

	return this->rsv_value; //Tack on % when outputting to console or other text output
}