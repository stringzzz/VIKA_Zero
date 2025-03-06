#####################################
# By stringzzz,
# Ghostwarez Co.
# 03-05-2025
#####################################

import os

def convert_file(filename):
	#Convert exponential notation numbers into numbers
	with open(filename, 'r') as distribution_file:
		lines = distribution_file.readlines()
		
	with open(filename, 'w') as converted_file:
		for line in lines:
			temp_x, temp_y = line.split(" ")
			converted_file.write(f"{temp_x} {int(float(temp_y.strip()))}\n")


for n_cycles in range(1, 16 + 1):

	distribution_file = f"pae_distribution_cycle{n_cycles}.txt"
	print(f"Converting from exponential notation for '{distribution_file}'")
	convert_file(distribution_file)

	descriptive_statistics_file = f"pae_distribution_Descriptive_Statistics_cycle{n_cycles}.txt"
	print(f"Generating descriptive statistics file {descriptive_statistics_file} for file {distribution_file}...")
	os.system(f"./vz_sample_statistics {distribution_file} {descriptive_statistics_file}")
	print("")

print("Descriptive statistics files 1-16 generated.")