This was for testing the bit differences between original plaintexts and their copies, where the copies were encrypted with a
key schedule setup produced by keys differing from the original by 1 bit. The differences were gathered at the end of 
each encryption cycle to track these differences as it goes through the entire encryption process.

See "KAE_Each_Cycle_Test_Results1.pdf" in this directory for a mini informal paper on this test.

If recreating this experiment, 'main.cpp VZ_sample_statistics.cpp' needs to be compiled to 'vz_sample_statistics' to use
'automated_distribution_test.py'.

--stringzzz, Ghostwarez Co.
03-10-2025
