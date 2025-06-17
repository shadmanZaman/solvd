#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define HEADER_SIZE 54

void encrypt_bmp_pixels(const char *input, const char *output, unsigned char key)
{
    FILE *fin = fopen(input, "rb");   // read bin
    FILE *fout = fopen(output, "wb"); // write bin

    if (!fin || !fout)
    {
        printf("File error\n");
        return;
    }

    unsigned char header[HEADER_SIZE];
    fread(header, 1, HEADER_SIZE, fin);   // Read BMP header
    fwrite(header, 1, HEADER_SIZE, fout); // Write header as-is

    fseek(fin, 0, SEEK_END);
    long file_size = ftell(fin); // ftell() returns the position after the last byte, equal to counted num of bytes
    long pixel_data_size = file_size - HEADER_SIZE;
    fseek(fin, HEADER_SIZE, SEEK_SET);

    unsigned char *pixel_data = malloc(pixel_data_size);
    fread(pixel_data, 1, pixel_data_size, fin); // Read pixel data

    // E1
    // for (long i = 0; i < pixel_data_size; i++) // Goes over byte by byte (pointer arithmetic is scaled by the size of the data type)
    // {
    //     pixel_data[i] ^= key; // XOR encrypt/decrypt
    //     pixel_data[i] = ((pixel_data[i] & 0xF0) >> 4) | ((pixel_data[i] & 0x0F) << 4);
    // }

    // E2
    int block = 30;

    for (long i = 0; i < pixel_data_size - block; i += block)
    {
        for (int j = 0; j < block / 2; j++)
        {
            unsigned char tmp = pixel_data[i + j];
            pixel_data[i + j] = pixel_data[i + block - 1 - j];
            pixel_data[i + block - 1 - j] = tmp;
        }
    }

    // E3
    //  for (long i = 0; i < pixel_data_size; i++)
    //  {
    //      pixel_data[i] = (unsigned char)(pixel_data[i] + 100 * sin(i / 10.0));
    //  }

    fwrite(pixel_data, 1, pixel_data_size, fout); // Write encrypted/decrypted pixels

    free(pixel_data);
    fclose(fin);
    fclose(fout);
}

int main()
{
    unsigned char key = 0xAA;

    encrypt_bmp_pixels("original.bmp", "encrypted.bmp", key);
    // encrypt_bmp_pixels("encrypted.bmp", "decrypted.bmp", key); // To restore original

    printf("Done!\n");
    return 0;
}
