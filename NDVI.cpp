#include"BmpReadAndWrite.cpp"
#include<iostream>
#include<fstream>
using namespace std;


void CalculateNDVI(BMPImage* bmp1,const char* ipfile)
{
    BMPImage* bmp2 = new BMPImage;
    float ndvi,numerator,denominator;
    if(readBmpImage(bmp2,ipfile))
    {
        for(int i = 0; i < bmp1->infoHeader.height; i++)
        {
            for(int j  = 0; j < bmp1->infoHeader.width; j++)
            {
                numerator = static_cast<float>(bmp1->pixel[i][j][1] - bmp1->pixel[i][j][2]);
                denominator = static_cast<float>(bmp1->pixel[i][j][1] + bmp1->pixel[i][j][2]);
                if(denominator == 0.0)
                {
                    denominator = 1.0;
                }
                else
                {
                    ndvi = static_cast<float>(numerator/denominator);
                    //bmp2->pixel[i][j][0] = static_cast<unsigned char>(200 - ((ndvi*100)+100));
                    bmp2->pixel[i][j][1] = static_cast<unsigned char>((ndvi*100)+150);
                    //bmp3->pixel[i][j][2] = static_cast<unsigned char>(200 - ((ndvi*100)+100));
                }
            }
        }
    }
    const char* opfile;
    opfile = "outNDVI.bmp";
    constructBmpImage(bmp2,opfile);
}
int main()
{
	
	BMPImage* bmp1 = new BMPImage;
	const char* ipfile;

	ipfile = "dhaka.bmp";
	
	printf("Input file : %s\n", ipfile);
 
	// Process the image and print its details
	if (readBmpImage(bmp1, ipfile))
	{
        CalculateNDVI(bmp1,ipfile);
	}
	return 0;
}