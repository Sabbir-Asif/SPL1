#include"BmpReadAndWrite.cpp"
#include<iostream>
#include<fstream>
using namespace std;


void CalculateNDVI(BMPImage* bmp1, BMPImage* bmp2, BMPImage* bmp3, const char* ipfile)
{
    float ndvi,numerator,denominator;
    if(readBmpImage(bmp3,ipfile))
    {
        for(int i = 0; i < bmp1->infoHeader.height; i++)
        {
            for(int j  = 0; j < bmp1->infoHeader.width; j++)
            {
                numerator = bmp2->pixel[i][j][2] - bmp1->pixel[i][j][0];
                denominator = bmp2->pixel[i][j][2] + bmp1->pixel[i][j][0];
                if(denominator == 0)
                {
                    denominator = 1.0;
                }
                else
                {
                    ndvi = numerator/denominator;
                    
                }
            }
        }
    }
}
int main()
{
	
	BMPImage* bmp1 = new BMPImage;
    BMPImage* bmp2 = new BMPImage;
    BMPImage* bmp3 = new BMPImage;
	const char* ipfile1;
    const char* ipfile2;
	const char* opfile;

	ipfile1 = "b4.bmp";
	ipfile2 = "b5.bmp";
	opfile = "outNDVI.bmp";
	printf("Input file : %s %s\n", ipfile1, ipfile2);
 
	// Process the image and print its details
	if (readBmpImage(bmp1, ipfile1) && readBmpImage(bmp2,ipfile2))
	{
        CalculateNDVI(bmp1,bmp2,bmp3,ipfile1);
        constructBmpImage(bmp3,opfile);
	}
	return 0;
}