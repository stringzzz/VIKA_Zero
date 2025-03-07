#Sort the S-box and P-box input/output pairs by order of highest frequency first.

#Deal with S-Box data
file = open(f"sbox_io1.txt", 'r')
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

output_file = open(f"sbox_io1_sorted.txt", 'w')
for pair in sorted_sbox_dictionary.keys():
	output_file.write(f"{pair} {sorted_sbox_dictionary[pair]} {sorted_sbox_dictionary[pair] / sbox_total * 100:.5f}%\n")
output_file.close()

#Deal with P-Box data
file = open(f"pbox_io1.txt", 'r')
tests2 = file.read().split("\n")
file.close()
tests2.pop()

pbox_total = 0
pbox_dictionary = {}
for line in tests2:
	[pair, value] = line.split(" ")
	pbox_dictionary[pair] = int(value)
	pbox_total += int(value)
	
sorted_pbox_dictionary = dict(sorted(pbox_dictionary.items(), key=lambda item: item[1], reverse=True))

output_file = open(f"pbox_io1_sorted.txt", 'w')
for pair in sorted_pbox_dictionary.keys():
	output_file.write(f"{pair} {sorted_pbox_dictionary[pair]} {sorted_pbox_dictionary[pair] / pbox_total * 100:.5f}%\n")
output_file.close()