#include "BmpReadAndWrite.cpp"
#include <iostream>
#include <fstream>
#include <cmath>
#define maxValue 700
using namespace std;

double InitialThresholdValue(BMPImage *bmp)
{
    double sum = 0.0;
    double NumOfPixels = bmp->infoHeader.height * bmp->infoHeader.width;
    for (int row = 0; row < bmp->infoHeader.height; row++)
    {
        for (int col = 0; col < bmp->infoHeader.width; col++)
        {
            sum += bmp->pixel[row][col][0];
        }
    }

    return sum / NumOfPixels;
}

void computeClassAverages(BMPImage *bmp, double &lowerClassAverage, double &upperClassAverage, double threshold)
{
    double lowerSum = 0.0;
    double upperSum = 0.0;
    int lowerCount = 0;
    int upperCount = 0;

    for (int row = 0; row < bmp->infoHeader.height; row++)
    {
        for (int col = 0; col < bmp->infoHeader.width; col++)
        {
            if (bmp->pixel[row][col][0] != 0)
            {
                if (bmp->pixel[row][col][0] < threshold)
                {
                    lowerSum += bmp->pixel[row][col][0];
                    lowerCount++;
                }
                else
                {
                    upperSum += bmp->pixel[row][col][0];
                    upperCount++;
                }
            }
        }
    }

    lowerClassAverage = lowerSum / lowerCount;
    upperClassAverage = upperSum / upperCount;
}

bool isThresholdReached(double threshold, double newThreshold, double tolerance)
{
    return std::abs(newThreshold - threshold) <= tolerance;
}

void ApplyThreshold(BMPImage *bmp)
{
    double initialThreshold = InitialThresholdValue(bmp);
    double lowerClassAverage = initialThreshold;
    double upperClassAverage = initialThreshold;
    double tolerance = 0.01;
    double threshold = initialThreshold;
    computeClassAverages(bmp, lowerClassAverage, upperClassAverage, threshold);
    double newThreshold = 0.5 * (lowerClassAverage + upperClassAverage);
    while (!isThresholdReached(threshold, newThreshold, tolerance))
    {
        threshold = newThreshold;
        computeClassAverages(bmp, lowerClassAverage, upperClassAverage, threshold);
        newThreshold = 0.5 * (lowerClassAverage + upperClassAverage);
    }

    for (int row = 0; row < bmp->infoHeader.height; row++)
    {
        for (int col = 0; col < bmp->infoHeader.width; col++)
        {
            if (bmp->pixel[row][col][0] <= threshold)
            {
                bmp->pixel[row][col][0] = static_cast<unsigned char>(0);
                bmp->pixel[row][col][1] = static_cast<unsigned char>(0);
                bmp->pixel[row][col][2] = static_cast<unsigned char>(0);
            }
            else
            {
                bmp->pixel[row][col][0] = static_cast<unsigned char>(255);
                bmp->pixel[row][col][1] = static_cast<unsigned char>(255);
                bmp->pixel[row][col][2] = static_cast<unsigned char>(255);
            }
        }
    }
}

void CloseContourSearch(int E[][maxValue], int Region[][maxValue], int i0, int j0, int &r, int height, int width)
{
    const int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    const int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int queue[maxValue * maxValue][2];
    int front = 0, rear = 0;

    queue[rear][0] = i0;
    queue[rear][1] = j0;
    rear++;

    while (front < rear)
    {
        int i = queue[front][0];
        int j = queue[front][1];
        front++;

        for (int count = 0; count < 8; count++)
        {
            int row = i + dx[count];
            int col = j + dy[count];
            if (row >= 0 && row < height && col >= 0 && col < width)
            {
                if (E[row][col] == E[i][j] && Region[row][col] == 0)
                {
                    Region[row][col] = r;
                    queue[rear][0] = row;
                    queue[rear][1] = col;
                    rear++;
                }
            }
        }
    }
}

int closedContourMain(int E[][maxValue], int Region[][maxValue], int height, int width)
{
    int r = 0;
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            if (Region[row][col] == 0)
            {
                r++;
                Region[row][col] = r;
                CloseContourSearch(E, Region, row, col, r, height, width);
            }
        }
    }

    return r;
}

void objectSeperation(BMPImage *bmp, BMPImage *bmp2, int Region[][maxValue], int E[][maxValue])
{
    int height = bmp->infoHeader.height;
    int width = bmp->infoHeader.width;
    int pixelCount[height][width];
    int sumIntensity[height][width];
    double averageIntensity[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixelCount[i][j] = 0;
            sumIntensity[i][j] = 0;
            averageIntensity[i][j] = 0.0;
        }
    }

     int numOfRegion = closedContourMain(E, Region, height, width);

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int region = Region[row][col];
            pixelCount[region][region]++;
            sumIntensity[region][region] += bmp2->pixel[row][col][0];
        }
    }

    for (int r = 1; r <= numOfRegion; r++)
    {
        if (pixelCount[r][r] != 0)
        {
            averageIntensity[r][r] = static_cast<double>(sumIntensity[r][r] / pixelCount[r][r]);
        }
    }

    int maxRegion = 1;
    double maxIntensity = averageIntensity[1][1];
    for (int r = 2; r <= numOfRegion; r++)
    {
        if (averageIntensity[r][r] > maxIntensity)
        {
            maxRegion = r;
            maxIntensity = averageIntensity[r][r];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            if (Region[row][col] != maxRegion)
            {
                bmp2->pixel[row][col][0] = static_cast<unsigned char>(0);
                bmp2->pixel[row][col][1] = static_cast<unsigned char>(0);
                bmp2->pixel[row][col][2] = static_cast<unsigned char>(0);
            }
        }
    }
}
void TumorSegmentation(BMPImage *bmp, const char *ipfile)
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

    // corner padding
    for (int row = 0; row < bmp->infoHeader.height; row++)
    {
        for (int col = 0; col < bmp->infoHeader.width; col++)
        {
            /*if (row < 2 || row >= bmp->infoHeader.height - 2 || col < 2 || col >= bmp->infoHeader.width - 2)
            {
                bmp->pixel[row][col][0] = 0;
                bmp->pixel[row][col][1] = 0;
                bmp->pixel[row][col][2] = 0;

                bmp2->pixel[row][col][0] = 0;
                bmp2->pixel[row][col][1] = 0;
                bmp2->pixel[row][col][2] = 0;
            }*/
             if(row > 50 && row <70 && col > 50 && col < 70)
            {
                bmp->pixel[row][col][0] = 255;
                bmp->pixel[row][col][1] = 255;
                bmp->pixel[row][col][2] = 255;

                bmp2->pixel[row][col][0] = 255;
                bmp2->pixel[row][col][1] = 255;
                bmp2->pixel[row][col][2] = 255;
            }
            else
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

     //constructBmpImage(bmp2,"TestImage.bmp");
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

    int Region[maxValue][maxValue];
    int height = bmp->infoHeader.height;
    int width = bmp->infoHeader.width;
    int E[maxValue][maxValue];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            E[i][j] = static_cast<int>(bmp->pixel[i][j][0]);
        }
    }
    const char *opfile = "TumorOut.bmp";
    ApplyThreshold(bmp);
    objectSeperation(bmp,bmp2,Region,E);
    constructBmpImage(bmp2, opfile);
    printf("Enhanced Segmentation Successful!\n");
}

int main()
{

    BMPImage *bmp = new BMPImage;
    const char *ipfile;
    ipfile = "GreenLand.bmp";
    printf("Input file : %s\n", ipfile);
    if (readBmpImage(bmp, ipfile))
    {
        TumorSegmentation(bmp, ipfile);
    }

    return 0;
}