#include "temp2.cpp"
#include <iostream>
#include <fstream>

void CalculateNDVI(BMPImage *bmp1, BMPImage *bmp2, const char *ipfile)
{
    double ndvi, numerator, denominator;
    long long barrenCount = 0;
    long long sparsCount = 0;
    long long grassLandCount = 0;
    long healthyVegetationCount = 0;
    long long totalPixels = 0;
    for (int i = 0; i < bmp1->infoHeader.height; i++)
    {
        for (int j = 0; j < bmp1->infoHeader.width; j++)
        {
            numerator = bmp2->pixel[i][j][2] - bmp1->pixel[i][j][0];
            denominator = bmp2->pixel[i][j][2] + bmp1->pixel[i][j][0];
            if (denominator == 0)
            {
                denominator = 1.0;
            }
            ndvi = numerator / denominator;
            if (ndvi < 0)
            {
                barrenCount++;
            }
            else if (ndvi >= 0 && ndvi < 0.1)
            {
                sparsCount++;
            }
            else if (ndvi >= 0.1 && ndvi < 0.3)
            {
                grassLandCount++;
            }
            else
            {
                healthyVegetationCount++;
            }
            totalPixels++;
        }
    }

    // cout << barrenCount << grassLandCount << healthyVegetationCount << sparsCount << endl;
    cout << "Vegetation Index of the area : \n";
    cout << "Barren Land = " << (barrenCount * 100) / totalPixels << "%\n";
    cout << "Sparsly vegetated = " << (sparsCount * 100) / totalPixels << "%\n";
    cout << "Grass Land = " << (grassLandCount * 100) / totalPixels << "%\n";
    cout << "Healthy Plant = " << (healthyVegetationCount * 100) / totalPixels << "%\n";
}
void NdviUtility()
{

    BMPImage *bmp1 = new BMPImage;
    BMPImage *bmp2 = new BMPImage;
    const char *ipfile1;
    const char *ipfile2;
    const char *opfile;

    ipfile1 = "b42.bmp";
    ipfile2 = "b52.bmp";
    opfile = "outNDVI.bmp";
    if (readBmpImage(bmp1, ipfile1) && readBmpImage(bmp2, ipfile2))
    {
        printf("Input file : %s %s\n", ipfile1, ipfile2);
        CalculateNDVI(bmp1, bmp2, ipfile1);
        printf("NDVI Operation successful!\n");
    }
    else
    {
        printf("Error loading image in NDVI utility!\n");
    }
}

