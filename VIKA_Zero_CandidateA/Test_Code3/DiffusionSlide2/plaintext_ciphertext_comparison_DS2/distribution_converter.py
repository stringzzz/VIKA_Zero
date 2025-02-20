with open("pandc_comparison_distribution.txt", 'r') as distribution_file:
    with open("pandc_comparison_distribution_converted.txt", 'w') as converted_file:
        for line in distribution_file.readlines():
            temp_x, temp_y = line.split(":\t")
            converted_file.write(f"{temp_x}: {int(float(temp_y.strip()))}\n")
