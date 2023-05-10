#include"BmpReadAndWrite.cpp"
#include<iostream>
#include<fstream>
using namespace std;


void SobelEdgeDetection(BMPImage* bmp , const char* ipfile)
{
    BMPImage* bmp2 = new BMPImage;
    if(!readBmpImage(bmp2,ipfile))
    {
        cout << "Error Opening input file.\n";
    }

    int gx[3][3] = {{-1,0,1},
                   {-2,0,2},
                   {-1,0,1}};

    int gy[3][3] = {{1,2,1},
                    {0,0,0},
                    {-1,-2,-1}};

    for(int row = 0; row < bmp2->infoHeader.height; row++)
    {
        for(int col = 0; col < bmp2->infoHeader.width; col++)
        {
            float gray = (static_cast<float>(bmp2->pixel[row][col][0]) + static_cast<float>(bmp2->pixel[row][col][1])
                         + static_cast<float>(bmp2->pixel[row][col][0]))/3;
            bmp2->pixel[row][col][0] = static_cast<unsigned char>(gray);
            bmp2->pixel[row][col][1] = static_cast<unsigned char>(gray);
            bmp2->pixel[row][col][2] = static_cast<unsigned char>(gray);
        }
    }

    for(int row = 1; row < (bmp2->infoHeader.height-1); row++)
    {
        for(int col = 1; col < (bmp2->infoHeader.width-1); col++)
        {
            int gxOpBlue1 = bmp2->pixel[row-1][col-1][0] * gx[0][0] + bmp2->pixel[row-1][col][0] * gx[0][1] + bmp2->pixel[row-1][col+1][0] * gx[0][2];
            int gxOpBlue2 = bmp2->pixel[row][col-1][0] * gx[1][0] + bmp2->pixel[row][col][0] * gx[1][1] + bmp2->pixel[row][col+1][0] * gx[1][2];
            int gxOpBlue3 = bmp2->pixel[row+1][col-1][0] * gx[2][0] + bmp2->pixel[row+1][col][0] * gx[2][1] + bmp2->pixel[row+1][col+1][0] * gx[2][2];
            int gxBlue = gxOpBlue1 + gxOpBlue2 + gxOpBlue3;

            int gyOpBlue1 = bmp2->pixel[row-1][col-1][0] * gy[0][0] + bmp2->pixel[row-1][col][0] * gy[0][1] + bmp2->pixel[row-1][col+1][0] * gy[0][2];
            int gyOpBlue2 = bmp2->pixel[row][col-1][0] * gy[1][0] + bmp2->pixel[row][col][0] * gy[1][1] + bmp2->pixel[row][col+1][0] * gy[1][2];
            int gyOpBlue3 = bmp2->pixel[row+1][col-1][0] * gy[2][0] + bmp2->pixel[row+1][col][0] * gy[2][1] + bmp2->pixel[row+1][col+1][0] * gy[2][2];
            int gyBlue = gyOpBlue1 + gyOpBlue2 + gyOpBlue3;


            int gxOpGreen1 = bmp2->pixel[row-1][col-1][1] * gx[0][0] + bmp2->pixel[row-1][col][1] * gx[0][1] + bmp2->pixel[row-1][col+1][1] * gx[0][2];
            int gxOpGreen2 = bmp2->pixel[row][col-1][1] * gx[1][0] + bmp2->pixel[row][col][1] * gx[1][1] + bmp2->pixel[row][col+1][1] * gx[1][2];
            int gxOpGreen3 = bmp2->pixel[row+1][col-1][1] * gx[2][0] + bmp2->pixel[row+1][col][1] * gx[2][1] + bmp2->pixel[row+1][col+1][1] * gx[2][2];
            int gxGreen = gxOpGreen1 + gxOpGreen2 + gxOpGreen3;

            int gyOpGreen1 = bmp2->pixel[row-1][col-1][1] * gy[0][0] + bmp2->pixel[row-1][col][1] * gy[0][1] + bmp2->pixel[row-1][col+1][1] * gy[0][2];
            int gyOpGreen2 = bmp2->pixel[row][col-1][1] * gy[1][0] + bmp2->pixel[row][col][1] * gy[1][1] + bmp2->pixel[row][col+1][1] * gy[1][2];
            int gyOpGreen3 = bmp2->pixel[row+1][col-1][1] * gy[2][0] + bmp2->pixel[row+1][col][1] * gy[2][1] + bmp2->pixel[row+1][col+1][1] * gy[2][2];
            int gyGreen = gyOpGreen1 + gyOpGreen2 + gyOpGreen3;


            int gxOpRed1 = bmp2->pixel[row-1][col-1][2] * gx[0][0] + bmp2->pixel[row-1][col][2] * gx[0][1] + bmp2->pixel[row-1][col+1][2] * gx[0][2];
            int gxOpRed2 = bmp2->pixel[row][col-1][2] * gx[1][0] + bmp2->pixel[row][col][2] * gx[1][1] + bmp2->pixel[row][col+1][2] * gx[1][2];
            int gxOpRed3 = bmp2->pixel[row+1][col-1][2] * gx[2][0] + bmp2->pixel[row+1][col][2] * gx[2][1] + bmp2->pixel[row+1][col+1][2] * gx[2][2];
            int gxRed = gxOpRed1 + gxOpRed2 + gxOpRed3;

            int gyOpRed1 = bmp2->pixel[row-1][col-1][2] * gy[0][0] + bmp2->pixel[row-1][col][2] * gy[0][1] + bmp2->pixel[row-1][col+1][2] * gy[0][2];
            int gyOpRed2 = bmp2->pixel[row][col-1][2] * gy[1][0] + bmp2->pixel[row][col][2] * gy[1][1] + bmp2->pixel[row][col+1][2] * gy[1][2];
            int gyOpRed3 = bmp2->pixel[row+1][col-1][2] * gy[2][0] + bmp2->pixel[row+1][col][2] * gy[2][1] + bmp2->pixel[row+1][col+1][2] * gy[2][2];
            int gyRed = gxOpRed1 + gxOpRed2 + gxOpRed3;

            int sqrtBlue = (int)(sqrt(gxBlue*gxBlue + gyBlue*gyBlue));
            int sqrtGreen = (int)(sqrt(gxGreen*gxGreen + gyGreen*gyGreen));
            int sqrtRed = (int)(sqrt(gxRed*gxRed + gyRed*gyRed));

            if(sqrtBlue > 127)
            {
                sqrtBlue = 255;
            }
            if(sqrtGreen > 127)
            {
                sqrtGreen = 255;
            }
            if(sqrtRed > 127)
            {
                sqrtRed = 255;
            }

            float gray = (sqrtBlue + sqrtGreen + sqrtRed)/3;
            bmp->pixel[row][col][0] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][1] = static_cast<unsigned char>(gray);
            bmp->pixel[row][col][2] = static_cast<unsigned char>(gray);

        }
    }

    const char* opfile = "SobelOut.bmp";

    constructBmpImage(bmp,opfile);

}

int main()
{
	
	BMPImage* bmp = new BMPImage;
	const char* ipfile;
	ipfile = "flower.bmp";
	printf("Input file : %s\n", ipfile);
    if(readBmpImage(bmp,ipfile))
    {
        SobelEdgeDetection(bmp,ipfile);
    }
	
	return 0;
}