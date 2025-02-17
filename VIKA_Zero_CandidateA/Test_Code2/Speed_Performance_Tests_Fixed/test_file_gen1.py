# /*
# 104857600 bytes = 100 MB
# (6553600 blocks)

# */
import random

chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
with open("speed_test_file_100MB.txt", "w") as stf100:
    for n in range(0, 104857600):
        stf100.write(random.choice(chars))