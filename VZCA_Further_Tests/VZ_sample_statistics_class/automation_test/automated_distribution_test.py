#####################################
# By stringzzz,
# Ghostwarez Co.
# 03-01-2025
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

n_tests = int(input("\nEnter the number of test pairs to perform: "))
print("")

for test_name in ["pae", "kae"]:

	for n_test in range(1, n_tests + 1):

		print(f"{test_name} Distribution {n_test}/{n_tests}")
		distribution_file = f"{test_name}_Distribution{n_test}.txt"

		print(f"Generating '{distribution_file}', this may take a while...")
		os.system(f"./{test_name}_test1 {distribution_file}")

		print(f"Converting from exponential notation for '{distribution_file}'")
		convert_file(distribution_file)

		descriptive_statistics_file = f"{test_name}_Distribution{n_test}_Descriptive_Statistics.txt"
		print(f"Generating descriptive statistics file {descriptive_statistics_file} for file {distribution_file}...")
		os.system(f"./vz_sample_statistics {distribution_file} {descriptive_statistics_file}")
		print("")
	print("")

print("All tests complete.")

######################################################
#Sample output (output inside files not shown):

# Enter the number of test pairs to perform: 2

# pae Distribution 1/2
# Generating 'pae_Distribution1.txt', this may take a while...
# Tests took 274 seconds.
# Converting from exponential notation for 'pae_Distribution1.txt'
# Generating descriptive statistics file pae_Distribution1_Descriptive_Statistics.txt for file pae_Distribution1.txt...

# pae Distribution 2/2
# Generating 'pae_Distribution2.txt', this may take a while...
# Tests took 287 seconds.
# Converting from exponential notation for 'pae_Distribution2.txt'
# Generating descriptive statistics file pae_Distribution2_Descriptive_Statistics.txt for file pae_Distribution2.txt...


# kae Distribution 1/2
# Generating 'kae_Distribution1.txt', this may take a while...
# Tests took 340 seconds.
# Converting from exponential notation for 'kae_Distribution1.txt'
# Generating descriptive statistics file kae_Distribution1_Descriptive_Statistics.txt for file kae_Distribution1.txt...

# kae Distribution 2/2
# Generating 'kae_Distribution2.txt', this may take a while...
# Tests took 339 seconds.
# Converting from exponential notation for 'kae_Distribution2.txt'
# Generating descriptive statistics file kae_Distribution2_Descriptive_Statistics.txt for file kae_Distribution2.txt...


# All tests complete.
