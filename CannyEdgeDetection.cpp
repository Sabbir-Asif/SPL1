#include "temp2.cpp"
#include <iostream>
#include <fstream>
#include <cmath>
void doubleThreshold(BMPImage *bmp, int lowThreshold, int highThreshold)
{
    int height = bmp->infoHeader.height;
    int width = bmp->infoHeader.width;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            if (bmp->pixel[row][col][0] >= highThreshold)
            {
                
                bmp->pixel[row][col][0] = static_cast<unsigned char>(255);
                bmp->pixel[row][col][1] = static_cast<unsigned char>(255);
                bmp->pixel[row][col][2] = static_cast<unsigned char>(255);
            }
            else if (bmp->pixel[row][col][0] < lowThreshold)
            {
                bmp->pixel[row][col][0] = static_cast<unsigned char>(0);
                bmp->pixel[row][col][1] = static_cast<unsigned char>(0);
                bmp->pixel[row][col][2] = static_cast<unsigned char>(0);
            }
            else
            {
                bool isEdge = false;
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        int rowN = row + i;
                        int colN = col + j;

                        // Check if the neighboring pixel is within the image boundaries
                        if (rowN >= 0 && rowN < height && colN >= 0 && colN < width)
                        {
                            if (bmp->pixel[rowN][colN][0] >= highThreshold)
                            {
                                // Mark the pixel as an edge
                                bmp->pixel[row][col][0] = static_cast<unsigned char>(255);
                                bmp->pixel[row][col][1] = static_cast<unsigned char>(255);
                                bmp->pixel[row][col][2] = static_cast<unsigned char>(255);
                                isEdge = true;
                                break;
                            }
                        }
                    }
                    if (isEdge)
                    {
                        break;
                    }
                }
                // If none of the neighboring pixels are above the high threshold, mark the pixel as not an edge
                if (!isEdge)
                {
                    bmp->pixel[row][col][0] = static_cast<unsigned char>(0);
                    bmp->pixel[row][col][1] = static_cast<unsigned char>(0);
                    bmp->pixel[row][col][2] = static_cast<unsigned char>(0);
                }
            }
        }
    }
}

void CannyEdgeDetection(const char *ipfile)
{
    BMPImage *bmp = new BMPImage;
    BMPImage *bmp2 = new BMPImage;
    if (!readBmpImage(bmp, ipfile))
    {
        cout << "Error Opening input file.\n";
    }
    else
    {
        printf("Input file %s\n",ipfile);
    }

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

    for (int row = 2; row < (bmp->infoHeader.height - 2); row++)
    {
        for (int col = 2; col < (bmp->infoHeader.width - 2); col++)
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
    }

    // obtaining gradient of the image
    float Orientation[bmp->infoHeader.height][bmp->infoHeader.width][3];

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

            Orientation[row][col][0] = atan2(gyBlue, gxBlue);
            Orientation[row][col][1] = atan2(gyGreen, gxGreen);
            Orientation[row][col][2] = atan2(gyRed, gxRed);

            float gray = (sqrtBlue + sqrtGreen + sqrtRed) / 3;
            bmp->pixel[row][col][0] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][1] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][2] = static_cast<unsigned char>(gray);
        }
    }

    for (int row = 1; row < bmp->infoHeader.height - 1; row++)
    {
        for (int col = 1; col < bmp->infoHeader.width - 1; col++)
        {
            float angle = Orientation[row][col][0];
            float mag = static_cast<float>(bmp->pixel[row][col][0]);
            float val1, val2;

            // If angle is zero degrees
            if ((angle >= -22.5 && angle <= 22.5) || (angle < -157.5 && angle >= -180) || (angle > 157.5 && angle <= 180))
            {
                val1 = static_cast<float>(bmp->pixel[row][col - 1][0]);
                val2 = static_cast<float>(bmp->pixel[row][col + 1][0]);
            }
            // If angle is 45 degrees
            else if ((angle > 22.5 && angle <= 67.5) || (angle < -112.5 && angle >= -157.5))
            {
                val1 = static_cast<float>(bmp->pixel[row - 1][col + 1][0]);
                val2 = static_cast<float>(bmp->pixel[row + 1][col - 1][0]);
            }
            // If angle is 90 degrees
            else if ((angle > 67.5 && angle <= 112.5) || (angle < -67.5 && angle >= -112.5))
            {
                val1 = static_cast<float>(bmp->pixel[row - 1][col][0]);
                val2 = static_cast<float>(bmp->pixel[row + 1][col][0]);
            }
            // If angle is 135 degrees
            else if ((angle > 112.5 && angle <= 157.5) || (angle < -22.5 && angle >= -67.5))
            {
                val1 = static_cast<float>(bmp->pixel[row - 1][col - 1][0]);
                val2 = static_cast<float>(bmp->pixel[row + 1][col + 1][0]);
            }

            // Compare bmp->pixel of current pixel with the adjacent pixels
            if (mag >= val1 && mag >= val2)
            {
                bmp->pixel[row][col][0] = static_cast<unsigned char>(mag);
                bmp->pixel[row][col][1] = static_cast<unsigned char>(mag);
                bmp->pixel[row][col][2] = static_cast<unsigned char>(mag);
            }
            else
            {
                bmp->pixel[row][col][0] = static_cast<unsigned char>(0);
                bmp->pixel[row][col][1] = static_cast<unsigned char>(0);
                bmp->pixel[row][col][2] = static_cast<unsigned char>(0);
            }
        }
    }

    doubleThreshold(bmp,60,80);
    const char *opfile = "CannyOut.bmp";
    constructBmpImage(bmp, opfile);
    printf("Canny edge detection Operation Successful!\n");
    printf("Output file CannyOut.bmp\n");
}

void CannyUtility()
{
    CannyEdgeDetection("blackbuck.bmp");
}

/*int main()
{
    CannyUtility();
}*/