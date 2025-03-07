#Sort the S-box input/output differential pairs by order of highest frequency first.

#Deal with S-Box data
file = open(f"sbox_io_differentials1.txt", 'r')
tests = file.read().split("\n")
file.close()
tests.pop() #Remove final blank line from splitting on newlines
	
sbox_total = 0
sbox_dictionary = {}
for line in tests:
	[pair, value] = line.split(" ")
	sbox_dictionary[pair] = int(value)
	sbox_total += int(value)
	
sorted_sbox_dictionary = dict(sorted(sbox_dictionary.items(), key=lambda item: item[1], reverse=True))

output_file = open(f"sbox_io_differentials1_sorted.txt", 'w')
for pair in sorted_sbox_dictionary.keys():
	output_file.write(f"{pair} {sorted_sbox_dictionary[pair]} {sorted_sbox_dictionary[pair] / sbox_total * 100:.5f}%\n")
output_file.close()
