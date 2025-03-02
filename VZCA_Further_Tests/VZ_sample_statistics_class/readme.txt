After learning a bit about statistics, I was able to come up with this code to do further analysis on the
Plaintext Avalanche Effect (PAE) & Key Avalanche Effect (KAE) distributions for VIKA Zero Candidate A.
I still have a lot to learn on actually interpreting all of the results, but I am making progress.


To recreate this experiment (I use the term loosely):

1. Compile and Run either 'KAE_test1.cpp' or 'PAE_test1.cpp' with the desired output filename as a command line argument.
Optionally, you may also need to run 'distribution_converter.py' on the output file to convert from exponential notation.

2. Compile and run 'main.cpp VZ_sample_statistics.cpp' with the 1st argument as the output filename from step 1, 
and the 2nd argument as the desired output statistics filename.

3. The results will be output to a file, nothing on the command line.
The results should also vary slightly from the sample ones provided due to the PRNG involved in the tests.

!!!

OR, if you prefer a more automated process for this, see the included directory 'automation_test' for 
a simple python script to do all of this. 

The C++ programs will have to be compiled first, though:

main.cpp VZ_sample_statistics.cpp -o vz_sample_statistics
PAE_test1.cpp -o pae_test1
KAE_test1.cpp -o kae_test1

!!!

P.S. There is more generic code included in an inner directory with this for doing the same descriptive statistics 
for simple sets of sample data. The other code described above is tailored specifically for the kind of 
data setup that those 'PAE' and 'KAE' programs output.

--stringzzz, 
Ghostwarez Co. 
02-28-2025
