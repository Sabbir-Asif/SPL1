#include "BmpReadAndWrite.cpp"
#include <iostream>
#include <fstream>
using namespace std;

void CannyEdgeDetection(BMPImage *bmp, const char *ipfile)
{
    BMPImage *bmp2 = new BMPImage;
    if (!readBmpImage(bmp2, ipfile))
    {
        cout << "Error Opening input file.\n";
    }

    int gx[3][3] = {{-1, 0, 1},
                    {-2, 0, 2},
                    {-1, 0, 1}};

    int gy[3][3] = {{1, 2, 1},
                    {0, 0, 0},
                    {-1, -2, -1}};

    // Converting to grayscale image
    for (int row = 0; row < bmp2->infoHeader.height; row++)
    {
        for (int col = 0; col < bmp2->infoHeader.width; col++)
        {
            float gray = (static_cast<float>(bmp2->pixel[row][col][0]) + static_cast<float>(bmp2->pixel[row][col][1]) + static_cast<float>(bmp2->pixel[row][col][0])) / 3;
            bmp->pixel[row][col][0] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][1] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][2] = static_cast<unsigned char>(gray);

            bmp2->pixel[row][col][0] = static_cast<unsigned char>(gray);
            bmp2->pixel[row][col][1] = static_cast<unsigned char>(gray);
            bmp2->pixel[row][col][2] = static_cast<unsigned char>(gray);
        }
    }

    // performing gaussian smoothing

    const float kernel[5][5] = {
        {1.0f / 256, 4.0f / 256, 6.0f / 256, 4.0f / 256, 1.0f / 256},
        {4.0f / 256, 16.0f / 256, 24.0f / 256, 16.0f / 256, 4.0f / 256},
        {6.0f / 256, 24.0f / 256, 36.0f / 256, 24.0f / 256, 6.0f / 256},
        {4.0f / 256, 16.0f / 256, 24.0f / 256, 16.0f / 256, 4.0f / 256},
        {1.0f / 256, 4.0f / 256, 6.0f / 256, 4.0f / 256, 1.0f / 256}};

    for (int row = 2; row < (bmp->infoHeader.height - 2); row++)
    {
        for (int col = 2; col < (bmp->infoHeader.width - 2); col++)
        {
            if (row != bmp2->infoHeader.height && col != bmp2->infoHeader.width)
            {
                float r1Blue = static_cast<float>(bmp->pixel[row - 2][col - 2][0]) * kernel[0][0] + static_cast<float>(bmp->pixel[row - 2][col - 1][0]) * kernel[0][1] + static_cast<float>(bmp->pixel[row - 2][col][0]) * kernel[0][2] + static_cast<float>(bmp->pixel[row - 2][col + 1][0]) * kernel[0][3] + static_cast<float>(bmp->pixel[row - 2][col + 2][0]) * kernel[0][4];
                float r2Blue = static_cast<float>(bmp->pixel[row - 1][col - 2][0]) * kernel[1][0] + static_cast<float>(bmp->pixel[row - 1][col - 1][0]) * kernel[1][1] + static_cast<float>(bmp->pixel[row - 1][col][0]) * kernel[1][2] + static_cast<float>(bmp->pixel[row - 1][col + 1][0]) * kernel[1][3] + static_cast<float>(bmp->pixel[row - 1][col + 2][0]) * kernel[1][4];
                float r3Blue = static_cast<float>(bmp->pixel[row][col - 2][0]) * kernel[2][0] + static_cast<float>(bmp->pixel[row][col - 1][0]) * kernel[2][1] + static_cast<float>(bmp->pixel[row][col][0]) * kernel[2][2] + static_cast<float>(bmp->pixel[row][col + 1][0]) * kernel[2][3] + static_cast<float>(bmp->pixel[row][col + 2][0]) * kernel[2][4];
                float r4Blue = static_cast<float>(bmp->pixel[row + 1][col - 2][0]) * kernel[3][0] + static_cast<float>(bmp->pixel[row + 1][col - 1][0]) * kernel[3][1] + static_cast<float>(bmp->pixel[row + 1][col][0]) * kernel[3][2] + static_cast<float>(bmp->pixel[row + 1][col + 1][0]) * kernel[3][3] + static_cast<float>(bmp->pixel[row + 1][col + 2][0]) * kernel[3][4];
                float r5Blue = static_cast<float>(bmp->pixel[row + 2][col - 2][0]) * kernel[4][0] + static_cast<float>(bmp->pixel[row + 2][col - 1][0]) * kernel[4][1] + static_cast<float>(bmp->pixel[row + 2][col][0]) * kernel[4][2] + static_cast<float>(bmp->pixel[row + 2][col + 1][0]) * kernel[4][3] + static_cast<float>(bmp->pixel[row + 2][col + 2][0]) * kernel[4][4];
                float kernelBlue = r1Blue + r2Blue + r3Blue + r4Blue + r5Blue;
                bmp2->pixel[row][col][0] = static_cast<unsigned char>(kernelBlue);

                float r1Green = static_cast<float>(bmp->pixel[row - 2][col - 2][1]) * kernel[0][0] + static_cast<float>(bmp->pixel[row - 2][col - 1][1]) * kernel[0][1] + static_cast<float>(bmp->pixel[row - 2][col][1]) * kernel[0][2] + static_cast<float>(bmp->pixel[row - 2][col + 1][1]) * kernel[0][3] + static_cast<float>(bmp->pixel[row - 2][col + 2][1]) * kernel[0][4];
                float r2Green = static_cast<float>(bmp->pixel[row - 1][col - 2][1]) * kernel[1][0] + static_cast<float>(bmp->pixel[row - 1][col - 1][1]) * kernel[1][1] + static_cast<float>(bmp->pixel[row - 1][col][1]) * kernel[1][2] + static_cast<float>(bmp->pixel[row - 1][col + 1][1]) * kernel[1][3] + static_cast<float>(bmp->pixel[row - 1][col + 2][1]) * kernel[1][4];
                float r3Green = static_cast<float>(bmp->pixel[row][col - 2][1]) * kernel[2][0] + static_cast<float>(bmp->pixel[row][col - 1][1]) * kernel[2][1] + static_cast<float>(bmp->pixel[row][col][1]) * kernel[2][2] + static_cast<float>(bmp->pixel[row][col + 1][1]) * kernel[2][3] + static_cast<float>(bmp->pixel[row][col + 2][1]) * kernel[2][4];
                float r4Green = static_cast<float>(bmp->pixel[row + 1][col - 2][1]) * kernel[3][0] + static_cast<float>(bmp->pixel[row + 1][col - 1][1]) * kernel[3][1] + static_cast<float>(bmp->pixel[row + 1][col][1]) * kernel[3][2] + static_cast<float>(bmp->pixel[row + 1][col + 1][1]) * kernel[3][3] + static_cast<float>(bmp->pixel[row + 1][col + 2][1]) * kernel[3][4];
                float r5Green = static_cast<float>(bmp->pixel[row + 2][col - 2][1]) * kernel[4][0] + static_cast<float>(bmp->pixel[row + 2][col - 1][1]) * kernel[4][1] + static_cast<float>(bmp->pixel[row + 2][col][1]) * kernel[4][2] + static_cast<float>(bmp->pixel[row + 2][col + 1][1]) * kernel[4][3] + static_cast<float>(bmp->pixel[row + 2][col + 2][1]) * kernel[4][4];
                float kernelGreen = r1Green + r2Green + r3Green + r4Green + r5Green;
                bmp2->pixel[row][col][1] = static_cast<unsigned char>(kernelGreen);

                float r1Red = static_cast<float>(bmp->pixel[row - 2][col - 2][2]) * kernel[0][0] + static_cast<float>(bmp->pixel[row - 2][col - 1][2]) * kernel[0][1] + static_cast<float>(bmp->pixel[row - 2][col][2]) * kernel[0][2] + static_cast<float>(bmp->pixel[row - 2][col + 1][2]) * kernel[0][3] + static_cast<float>(bmp->pixel[row - 2][col + 2][2]) * kernel[0][4];
                float r2Red = static_cast<float>(bmp->pixel[row - 1][col - 2][2]) * kernel[1][0] + static_cast<float>(bmp->pixel[row - 1][col - 1][2]) * kernel[1][1] + static_cast<float>(bmp->pixel[row - 1][col][2]) * kernel[1][2] + static_cast<float>(bmp->pixel[row - 1][col + 1][2]) * kernel[1][3] + static_cast<float>(bmp->pixel[row - 1][col + 2][2]) * kernel[1][4];
                float r3Red = static_cast<float>(bmp->pixel[row][col - 2][2]) * kernel[2][0] + static_cast<float>(bmp->pixel[row][col - 1][2]) * kernel[2][1] + static_cast<float>(bmp->pixel[row][col][2]) * kernel[2][2] + static_cast<float>(bmp->pixel[row][col + 1][2]) * kernel[2][3] + static_cast<float>(bmp->pixel[row][col + 2][2]) * kernel[2][4];
                float r4Red = static_cast<float>(bmp->pixel[row + 1][col - 2][2]) * kernel[3][0] + static_cast<float>(bmp->pixel[row + 1][col - 1][2]) * kernel[3][1] + static_cast<float>(bmp->pixel[row + 1][col][2]) * kernel[3][2] + static_cast<float>(bmp->pixel[row + 1][col + 1][2]) * kernel[3][3] + static_cast<float>(bmp->pixel[row + 1][col + 2][2]) * kernel[3][4];
                float r5Red = static_cast<float>(bmp->pixel[row + 2][col - 2][2]) * kernel[4][0] + static_cast<float>(bmp->pixel[row + 2][col - 1][2]) * kernel[4][1] + static_cast<float>(bmp->pixel[row + 2][col][2]) * kernel[4][2] + static_cast<float>(bmp->pixel[row + 2][col + 1][2]) * kernel[4][3] + static_cast<float>(bmp->pixel[row + 2][col + 2][2]) * kernel[4][4];
                float kernelRed = r1Red + r2Red + r3Red + r4Red + r5Red;
                bmp2->pixel[row][col][2] = static_cast<unsigned char>(kernelRed);
            }
            else
            {
                bmp2->pixel[row][col][0] = 0;
                bmp2->pixel[row][col][1] = 0;
                bmp2->pixel[row][col][2] = 0;
            }
        }

        // obtaining gradient of the image
        for (int row = 1; row < (bmp2->infoHeader.height - 1); row++)
        {
            for (int col = 1; col < (bmp2->infoHeader.width - 1); col++)
            {
                float gxOpBlue1 = bmp2->pixel[row - 1][col - 1][0] * gx[0][0] + bmp2->pixel[row - 1][col][0] * gx[0][1] + bmp2->pixel[row - 1][col + 1][0] * gx[0][2];
                float gxOpBlue2 = bmp2->pixel[row][col - 1][0] * gx[1][0] + bmp2->pixel[row][col][0] * gx[1][1] + bmp2->pixel[row][col + 1][0] * gx[1][2];
                float gxOpBlue3 = bmp2->pixel[row + 1][col - 1][0] * gx[2][0] + bmp2->pixel[row + 1][col][0] * gx[2][1] + bmp2->pixel[row + 1][col + 1][0] * gx[2][2];
                float gxBlue = gxOpBlue1 + gxOpBlue2 + gxOpBlue3;

                float gyOpBlue1 = bmp2->pixel[row - 1][col - 1][0] * gy[0][0] + bmp2->pixel[row - 1][col][0] * gy[0][1] + bmp2->pixel[row - 1][col + 1][0] * gy[0][2];
                float gyOpBlue2 = bmp2->pixel[row][col - 1][0] * gy[1][0] + bmp2->pixel[row][col][0] * gy[1][1] + bmp2->pixel[row][col + 1][0] * gy[1][2];
                float gyOpBlue3 = bmp2->pixel[row + 1][col - 1][0] * gy[2][0] + bmp2->pixel[row + 1][col][0] * gy[2][1] + bmp2->pixel[row + 1][col + 1][0] * gy[2][2];
                float gyBlue = gyOpBlue1 + gyOpBlue2 + gyOpBlue3;

                float gxOpGreen1 = bmp2->pixel[row - 1][col - 1][1] * gx[0][0] + bmp2->pixel[row - 1][col][1] * gx[0][1] + bmp2->pixel[row - 1][col + 1][1] * gx[0][2];
                float gxOpGreen2 = bmp2->pixel[row][col - 1][1] * gx[1][0] + bmp2->pixel[row][col][1] * gx[1][1] + bmp2->pixel[row][col + 1][1] * gx[1][2];
                float gxOpGreen3 = bmp2->pixel[row + 1][col - 1][1] * gx[2][0] + bmp2->pixel[row + 1][col][1] * gx[2][1] + bmp2->pixel[row + 1][col + 1][1] * gx[2][2];
                float gxGreen = gxOpGreen1 + gxOpGreen2 + gxOpGreen3;

                float gyOpGreen1 = bmp2->pixel[row - 1][col - 1][1] * gy[0][0] + bmp2->pixel[row - 1][col][1] * gy[0][1] + bmp2->pixel[row - 1][col + 1][1] * gy[0][2];
                float gyOpGreen2 = bmp2->pixel[row][col - 1][1] * gy[1][0] + bmp2->pixel[row][col][1] * gy[1][1] + bmp2->pixel[row][col + 1][1] * gy[1][2];
                float gyOpGreen3 = bmp2->pixel[row + 1][col - 1][1] * gy[2][0] + bmp2->pixel[row + 1][col][1] * gy[2][1] + bmp2->pixel[row + 1][col + 1][1] * gy[2][2];
                float gyGreen = gyOpGreen1 + gyOpGreen2 + gyOpGreen3;

                float gxOpRed1 = bmp2->pixel[row - 1][col - 1][2] * gx[0][0] + bmp2->pixel[row - 1][col][2] * gx[0][1] + bmp2->pixel[row - 1][col + 1][2] * gx[0][2];
                float gxOpRed2 = bmp2->pixel[row][col - 1][2] * gx[1][0] + bmp2->pixel[row][col][2] * gx[1][1] + bmp2->pixel[row][col + 1][2] * gx[1][2];
                float gxOpRed3 = bmp2->pixel[row + 1][col - 1][2] * gx[2][0] + bmp2->pixel[row + 1][col][2] * gx[2][1] + bmp2->pixel[row + 1][col + 1][2] * gx[2][2];
                float gxRed = gxOpRed1 + gxOpRed2 + gxOpRed3;

                float gyOpRed1 = bmp2->pixel[row - 1][col - 1][2] * gy[0][0] + bmp2->pixel[row - 1][col][2] * gy[0][1] + bmp2->pixel[row - 1][col + 1][2] * gy[0][2];
                float gyOpRed2 = bmp2->pixel[row][col - 1][2] * gy[1][0] + bmp2->pixel[row][col][2] * gy[1][1] + bmp2->pixel[row][col + 1][2] * gy[1][2];
                float gyOpRed3 = bmp2->pixel[row + 1][col - 1][2] * gy[2][0] + bmp2->pixel[row + 1][col][2] * gy[2][1] + bmp2->pixel[row + 1][col + 1][2] * gy[2][2];
                float gyRed = gxOpRed1 + gxOpRed2 + gxOpRed3;

                float sqrtBlue = (float)(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));
                float sqrtGreen = (float)(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
                float sqrtRed = (float)(sqrt(gxRed * gxRed + gyRed * gyRed));

                bmp->pixel[row][col][0] = static_cast<unsigned char>(sqrtBlue);
                bmp->pixel[row][col][1] = static_cast<unsigned char>(sqrtGreen);
                bmp->pixel[row][col][2] = static_cast<unsigned char>(sqrtRed);
                
            }
        }

        const char *opfile = "SobelOut.bmp";

        constructBmpImage(bmp, opfile);
    }
}

int main()
{

    BMPImage *bmp = new BMPImage;
    const char *ipfile;
    ipfile = "blackbuck.bmp";
    printf("Input file : %s\n", ipfile);
    if (readBmpImage(bmp, ipfile))
    {
        CannyEdgeDetection(bmp, ipfile);
    }

    return 0;
}