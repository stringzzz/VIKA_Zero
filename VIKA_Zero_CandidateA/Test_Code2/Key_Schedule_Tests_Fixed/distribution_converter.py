with open("keyschedule1A_SPAC_test2_distribution_final_fixed.txt", 'r') as distribution_file:
    with open("keyschedule1A_SPAC_test2_distribution_final_fixed_converted.txt", 'w') as converted_file:
        for line in distribution_file.readlines():
            temp_x, temp_y = line.split(":\t")
            converted_file.write(f"{temp_x}: {int(float(temp_y.strip()))}\n")
