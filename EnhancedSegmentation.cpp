#include "BmpReadAndWrite.cpp"
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

double InitialThresholdValue(BMPImage* bmp)
{
    double sum = 0.0;
    double NumOfPixels = bmp->infoHeader.height * bmp->infoHeader.width;
    for(int row = 0; row < bmp->infoHeader.height; row++)
    {
        for(int col = 0; col < bmp->infoHeader.width; col++)
        {
            sum+= bmp->pixel[row][col][0];
        }
    }
    
    return sum/NumOfPixels;
}

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


    //corner padding
    for (int row = 0; row < bmp->infoHeader.height; row++)
    {
        for (int col = 0; col < bmp->infoHeader.width; col++)
        {
            if (row < 2 || row >= bmp->infoHeader.height - 2 || col < 2 || col >= bmp->infoHeader.width - 2)
            {
                bmp->pixel[row][col][0] = 0;
                bmp->pixel[row][col][1] = 0;
                bmp->pixel[row][col][2] = 0;

                bmp2->pixel[row][col][0] = 0;
                bmp2->pixel[row][col][1] = 0;
                bmp2->pixel[row][col][2] = 0;
            }
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

            float gray = (sqrtBlue + sqrtGreen + sqrtRed) / 3;
            bmp->pixel[row][col][0] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][1] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][2] = static_cast<unsigned char>(gray);
        }
    }


    const char *opfile = "CannyOut.bmp";

    constructBmpImage(bmp, opfile);
}

int main()
{

    BMPImage *bmp = new BMPImage;
    const char *ipfile;
    ipfile = "f3.bmp";
    printf("Input file : %s\n", ipfile);
    if (readBmpImage(bmp, ipfile))
    {
        CannyEdgeDetection(bmp, ipfile);
    }

    return 0;
}