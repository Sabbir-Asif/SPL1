#include<fstream>
#include<iomanip>
#include<cmath>
#include "BmpHeader.h"
using namespace std;

bool readBmpImage(BMPImage* bmp, const char* filename)
{
    ifstream inputFile;
    inputFile.open(filename, ios:: binary);
    if(inputFile.eof())
    {
        cout << "Error opening input BMP image!\n";
        return false;
    }

    inputFile.seekg(0,ios::beg);
    inputFile.read((char*)&bmp->signature,sizeof(bmp->signature));
    inputFile.read((char*)&bmp->fileHeader,sizeof(bmp->fileHeader));
    inputFile.read((char*)&bmp->infoHeader,sizeof(bmp->infoHeader));
    inputFile.read((char*)&bmp->colorHeader,sizeof(bmp->colorHeader));

    int height = bmp->infoHeader.height;
    int width = bmp->infoHeader.width;

    bmp->pixel = (unsigned char***)malloc(height * sizeof(unsigned char**));
    for(int i = 0 ; i < height; i++)
    {
        bmp->pixel[i] = (unsigned char**)malloc(width * sizeof(unsigned char*));
        for(int j = 0; j < width; j++)
        {
            bmp->pixel[i][j] = (unsigned char*)malloc(3 * sizeof(unsigned char));
            inputFile.read(reinterpret_cast<char*>(bmp->pixel[i][j]),3);
        }
    }

    inputFile.read((char*)&bmp->colorTable,sizeof(bmp->colorTable));
    
    inputFile.close();

    return true;
}

void constructBmpImage(BMPImage* bmp, char const* filename)
{
    ofstream outputFile;
    outputFile.open(filename, ios:: binary);
    if(outputFile.eof())
    {
        cout << "Error writing file!\n";
    }

    int height = bmp->infoHeader.height;
    int width = bmp->infoHeader.width;

    outputFile.seekp(0,ios::beg);
    outputFile.write((char*)&bmp->signature,sizeof(bmp->signature));
    outputFile.write((char*)&bmp->fileHeader,sizeof(bmp->fileHeader));
    outputFile.write((char*)&bmp->infoHeader,sizeof(bmp->infoHeader));
    outputFile.write((char*)&bmp->colorHeader,sizeof(bmp->colorHeader));

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
             outputFile.write(reinterpret_cast<char*>(bmp->pixel[i][j]),3);
        }
    }

    outputFile.write((char*)&bmp->colorTable,sizeof(bmp->colorTable));
    outputFile.close();
}
