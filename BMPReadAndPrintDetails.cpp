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
        return EXIT_FAILURE;
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

void PrintBMPDetails(BMPImage* bmp, const char* filename)
{
    ofstream opfile;
    opfile.open(filename);
    opfile <<"Signature : \n";
    opfile << bmp->signature.signatureData[0] << bmp->signature.signatureData[0] << endl;
    opfile <<"File header : \n";
    opfile << bmp->fileHeader.fileSize << endl;
    opfile << bmp->fileHeader.reserved1 << endl;
    opfile << bmp->fileHeader.reserved2 << endl;
    opfile << bmp->fileHeader.dataOffset << endl;
    opfile <<"Info header : \n";
    opfile <<"header size : "<< bmp->infoHeader.headerSize << endl;
    opfile <<"width : " << bmp->infoHeader.width << endl;
    opfile <<"height : " << bmp->infoHeader.height << endl;
    opfile <<"planes : " << bmp->infoHeader.planes << endl;
    opfile <<"bpPixels : " << bmp->infoHeader.bpPixels << endl;
    opfile <<"compression : " << bmp->infoHeader.compression << endl;
    opfile <<"imageSize : " << bmp->infoHeader.imageSize << endl;
    opfile <<"pixelpmX : " << bmp->infoHeader.pixelPerMeterX << endl;
    opfile <<"pixelpmY : " << bmp->infoHeader.pixelPerMeterY << endl;
    opfile <<"colors in color table : " << bmp->infoHeader.colorsInColorTable << endl;
    opfile <<"important color count : " << bmp->infoHeader.importantcolorCount << endl;
    opfile <<"Color header : \n";
    opfile <<"red : " << bmp->colorHeader.red << endl;
    opfile <<"green : " << bmp->colorHeader.green << endl;
    opfile <<"blue : " << bmp->colorHeader.blue << endl;
    opfile <<"alpha : " << bmp->colorHeader.alpha << endl;
    opfile <<"color : " << bmp->colorHeader.color << endl;

    opfile <<"Data : \n";
    for(int i = 0; i < bmp->infoHeader.height; i++)
    {
        for(int j = 0; j < bmp->infoHeader.width; j++)
        {
            opfile << (int)bmp->pixel[i][j][0] << " " << (int)bmp->pixel[i][j][1] << " "
            << (int)bmp->pixel[i][j][2] << "    ";
        }
        opfile <<"\n";
    }

    opfile <<"Color table : \n";
    for(int i = 0; i < 256; i++)
    {
        opfile << bmp->colorTable.colorDefinition[i] << " ";
    }

    opfile <<"\n\n--Sabbir--\n";

}
int main()
{
	
	BMPImage* bmp = new BMPImage;
	const char* ipfile;
	const char* opfile;

	ipfile = "b52.bmp";
	
	opfile = "outb5.txt";
	printf("Input file : %s\n", ipfile);
 
	// Process the image and print its details
	if (readBmpImage(bmp, ipfile))
	{
        PrintBMPDetails(bmp,opfile);
	}
	return 0;
}