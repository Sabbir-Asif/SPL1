#include<iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "BmpHeader.h"
using namespace std;

//contains bmp signature
struct bmpSignature
{
    unsigned char signatureData[2]; //BM is .bmp file
};

//contains bmp file header information

struct bmpFileHeader
{
    unsigned int fileSize; //filesize in byte
    unsigned short reserved1; //reserved 
    unsigned short reserved2; //reserved
    unsigned int dataOffset; //start of pixel data
};
//contains bmp info header information
struct bmpInfoHeader
{
    unsigned int headerSize; //headersize in byte
    unsigned int width; //image width in px
    unsigned int height; //image height in px
    unsigned short planes;  //number of planes 
    unsigned short bpPixels; //number of bits per pixel
    unsigned int compression; //compression type 0 is uncompressed
    unsigned int imageSize; 
    unsigned int pixelPerMeterX; //pixel per meter along x axis
    unsigned int pixelPerMeterY; //pixel per meter along y axis
    unsigned int colorsInColorTable; //number of colors in color table
    unsigned int importantcolorCount; //number of colors used
};
//contains bmp color header information
struct bmpColorHeader{
    unsigned int red; //red channel bitmask
    unsigned int green; //green channel bitmask
    unsigned int blue; //blue channel bitmask
    unsigned int alpha; //alpha channel bitmask
    unsigned int color;    //color space type usually sRGB default
};

//contains bmp color table information
struct bmpColorTable
{
    unsigned int colorDefinition[256];
};

struct BMPImage
{
    bmpSignature signature;
    bmpFileHeader fileHeader;
    bmpInfoHeader infoHeader;
    bmpColorHeader colorHeader;
    bmpColorTable colorTable;
    unsigned char ***pixel;
};