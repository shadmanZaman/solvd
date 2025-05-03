
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WIDTH 100
#define HEIGHT 100
#define BMP_HEADER_SIZE 54

bool visited[HEIGHT][WIDTH];
uint8_t *rowData;
uint32_t rowSize;

void dfs(int x, int y, int depth)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || visited[y][x] || depth <= 0) // visited[x][y] can be deactivated
        return;

    visited[y][x] = true;

    // Since BMP stores pixels from bottom to top, flip Y before calculating where the row starts.
    int bmpY = HEIGHT - 1 - y;
    uint8_t *row = rowData + bmpY * rowSize; // Just points to the correct row (y) for manipualtion

    // x coordinate manipulation is done here
    row[x * 3 + 0] = 0;   // Blue
    row[x * 3 + 1] = 0;   // Green
    row[x * 3 + 2] = 255; // Red

    // It behaves like a grid of coodinates.
    // The value of x actually ranges from  0 to < width
    // It uses the value of y along with x to calculate its position in memeory

    int dx[] = {3, -3, 0, 0};//offsets
    int dy[] = {0, 0, 3, -3};

    for (int i = 0; i < 2; i++) // This applies dfs in all direction
    {
        int dir = rand() % 4; // Move only one dir at a time because it cannot move in both dir at the same time
        int dxval = dx[dir];
        int dyval = dy[dir];

        if (dxval != 0)
        {
            int step = (dxval > 0) ? 1 : -1;
            for (int i = step; i != dxval + step; i += step)
            {
                // r = 1 byte, g = 1 byte, b = 1 byte + 1 byte padding = stored inside int of 4 bytes.
                // So when we increment i, going from one int to another skipping 4 bytes, not skipping 1 byte which have r g or b stored in them
                int pixel_x = x + i;
                row[pixel_x * 3 + 0] = 0;   // Blue
                row[pixel_x * 3 + 1] = 0;   // Green
                row[pixel_x * 3 + 2] = 255; // Red
            }
        }

        if (dyval != 0)
        {
            int step = (dyval > 0) ? 1 : -1;
            for (int j = step; j != dyval + step; j += step)
            {
                row += (step == 1) ? -rowSize : rowSize;
                row[x * 3 + 0] = 0;   // Blue
                row[x * 3 + 1] = 0;   // Green
                row[x * 3 + 2] = 255; // Red
            }
        }
        // DFS in the same direction
        dfs(x + dxval, y + dyval, depth - 1);
    }
}

void write_bmp(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("Failed to open file");
        return;
    }

    rowSize = (WIDTH * 3 + 3) & ~3;
    uint32_t imageSize = rowSize * HEIGHT;
    uint32_t fileSize = BMP_HEADER_SIZE + imageSize;

    // BMP headers
    uint8_t header[BMP_HEADER_SIZE] = {0};
    header[0] = 'B';
    header[1] = 'M';
    *(uint32_t *)(header + 2) = fileSize;
    *(uint32_t *)(header + 10) = BMP_HEADER_SIZE;
    *(uint32_t *)(header + 14) = 40;
    *(uint32_t *)(header + 18) = WIDTH;
    *(uint32_t *)(header + 22) = HEIGHT;
    *(uint16_t *)(header + 26) = 1;
    *(uint16_t *)(header + 28) = 24;
    *(uint32_t *)(header + 34) = imageSize;

    fwrite(header, 1, BMP_HEADER_SIZE, file);

    rowData = (uint8_t *)calloc(HEIGHT, rowSize); // Returns pointer to the very first pixel
    if (!rowData)
    {
        perror("Failed to allocate memory");
        fclose(file);
        return;
    }

    // Fill image using DFS
    dfs(WIDTH / 2, HEIGHT / 2, 50);

    // Write all rows to file
    fwrite(rowData, 1, imageSize, file);
    free(rowData);
    fclose(file);
}

int main()
{
    srand(time(NULL));
    write_bmp("dfs_image.bmp");
    printf("Image generated with DFS crack pattern.\n");
    return 0;
}
