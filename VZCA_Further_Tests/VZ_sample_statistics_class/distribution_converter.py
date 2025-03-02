#Convert exponential notation numbers into numbers
with open("KAE_Distribution1.txt", 'r') as distribution_file:
    with open("KAE_Distribution1_converted.txt", 'w') as converted_file:
        for line in distribution_file.readlines():
            temp_x, temp_y = line.split(" ")
            converted_file.write(f"{temp_x} {int(float(temp_y.strip()))}\n")
