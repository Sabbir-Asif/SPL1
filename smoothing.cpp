#include "ImgReadAndWrite.cpp"
#include<cmath>
void meanPixel(PGMImage* pgm)
{
    for(int i = 1; i < (pgm->height - 1); i++)
    {
        for(int j = 1; j < (pgm->width - 1); j++)
        {
            int x = pgm->data[i-1][j-1] + pgm->data[i-1][j] + pgm->data[i-1][j+1];
            int y = pgm->data[i][j-1] + pgm->data[i][j] + pgm->data[i][j+1];
            int z = pgm->data[i+1][j-1] + pgm->data[i+1][j] + pgm->data[i+1][j+1];
            pgm->data[i][j] = round(((float)(x + y + z)) / 9);
        }
    }
}

int main(int argc, char const* argv[])
{
	//PGMImage* pgm = malloc(sizeof(PGMImage));
	PGMImage* pgm = new PGMImage;
	const char* ipfile;
	const char* opfile;
    int* histogram;

	if (argc == 2)
		ipfile = argv[1];
	else
		ipfile = "baboon.pgm";
	
	opfile = "outPgm.txt";
	printf("Input file : %s\n", ipfile);

	// Process the image and print its details
	if (readImage(pgm, ipfile))
	{
        meanPixel(pgm);
        constructImage(pgm,"smoothOut.pgm");
        
	}
	return 0;
}