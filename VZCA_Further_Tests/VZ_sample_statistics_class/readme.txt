After learning a bit about statistics, I was able to come up with this code to do further analysis on the
Plaintext Avalanche Effect (PAE) & Key Avalanche Effect (KAE) distributions for VIKA Zero Candidate A.
I still have a lot to learn on actually interpreting all of the results, but I am making progress.


To recreate this experiment (I use the term loosely):

1. Run either 'KAE_test1.cpp' or 'PAE_test1.cpp'. (Look for 'output_filename' at the top of source code, search for 'FLAG1' to jump up there)

2. Edit 'main.cpp' to input from either 'KAE_Distribution1.txt' or 'PAE_Distribution1.txt' 
or whatever you named the output files from step 1.

3. Compile and run 'main.cpp VZ_sample_statistics.cpp'

4. The results will be output to a file, nothing on the command line.
The results should also vary slightly from the sample ones provided.


P.S. There is more generic code included in an inner directory with this for doing the same descriptive statistics 
for simple sets of sample data. The other code described above is tailored specifically for the kind of 
data setup that those 'PAE' and 'KAE' programs output.

--stringzzz, 
Ghostwarez Co. 
02-28-2025