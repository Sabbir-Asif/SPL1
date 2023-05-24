#include"temp2.cpp"
#include<iostream>
#include<fstream>

void BMPToGrayScale(BMPImage* bmp , const char* ipfile)
{
    BMPImage* bmp2 = new BMPImage;
    if(!readBmpImage(bmp2,ipfile))
    {
        cout << "Error Opening temporary input file.\n";
    }

    for(int row = 0; row < bmp2->infoHeader.height; row++)
    {
        for(int col = 0; col < bmp2->infoHeader.width; col++)
        {
            float gray = (static_cast<float>(bmp2->pixel[row][col][0]) + static_cast<float>(bmp2->pixel[row][col][1])
                         + static_cast<float>(bmp2->pixel[row][col][2]))/3;
            bmp->pixel[row][col][0] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][1] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][2] = static_cast<unsigned char>(gray);
        }
    }

    const char* opfile = "GrayOut.bmp";

    constructBmpImage(bmp,opfile);

}

void BMPTOGrayScaleUtility()
{
	
	BMPImage* bmp = new BMPImage;
	const char* ipfile;
	ipfile = "blackbuck.bmp";
	if(readBmpImage(bmp, ipfile))
	{
        printf("Input file : %s\n", ipfile);
        BMPToGrayScale(bmp,ipfile);
        printf("BMP to GrayScale Operation successful!\n");
        printf("Output file GrayOut.bmp\n");
	}
    else
    {
        printf("Error Loading Image in BMPTOGrayScaleUtility!\n");
    }
}
