#include <iostream>
#include <random>

void diffusion_slide2_encrypt(unsigned int block[]);
void diffusion_slide2_decrypt(unsigned int block[]);

struct blocks {
    unsigned int plaintext[16];
    unsigned int ciphertext[16];
};

std::random_device random_source;

int main() {

    const unsigned int block_count = 1000000;
    bool non_matching = false;

    struct blocks* test_blocks = new struct blocks[block_count];
    for (unsigned int nblocks = 0; nblocks < block_count; nblocks++) {
        for(unsigned int n = 0; n < 16; n++) {
            unsigned int temp = random_source() % 256;
            test_blocks[nblocks].plaintext[n] = temp;
            test_blocks[nblocks].ciphertext[n] = temp;
        }

        diffusion_slide2_encrypt(test_blocks[nblocks].ciphertext);
        diffusion_slide2_decrypt(test_blocks[nblocks].ciphertext);
        for(unsigned int n = 0; n < 16; n++) {
            if (test_blocks[nblocks].plaintext[n] != test_blocks[nblocks].ciphertext[n]) {
                non_matching = true;
                break;
            }
        }
        if (non_matching) {
            break;
        }
    }

    delete[] test_blocks;

    if (non_matching) {
        std::cout << "Non match found!\n";
    } else {
        std::cout << "All blocks match!\n";
    }

    return 0;
}

void diffusion_slide2_encrypt(unsigned int block[]) {
    for(unsigned int i = 0; i < 16; i++) {
        if (i == 15) {
            block[0] ^= block[15];
        } else {
            block[i + 1] ^= block[i];
        }
    }
}

void diffusion_slide2_decrypt(unsigned int block[]) {
    for(unsigned int i = 15; i > 0; i--) {
        if (i == 15) {
            block[0] ^= block[15];
        }
        block[i] ^= block[i - 1];
    }
}