#include "ImgReadAndWrite.cpp"
#include<cmath>
void meanPixel(PGMImage* pgm, const char* ipfile)
{
	PGMImage* pgm2 = new PGMImage;
	if (!readImage(pgm2, ipfile))
	{
		cout << "File not Opened in Smoothing function" << endl;
		
	}

    for(int i = 1; i < (pgm->height - 1); i++)
    {
        for(int j = 1; j < (pgm->width - 1); j++)
        {
            int x = pgm2->data[i-1][j-1] + pgm2->data[i-1][j] + pgm2->data[i-1][j+1];
            int y = pgm2->data[i][j-1] + pgm2->data[i][j] + pgm2->data[i][j+1];
            int z = pgm2->data[i+1][j-1] + pgm2->data[i+1][j] + pgm2->data[i+1][j+1];
            pgm->data[i][j] = (unsigned char)round(((float)(x + y + z)) / 9);
        }
    }

} 

int main()
{
	//PGMImage* pgm = malloc(sizeof(PGMImage));
	PGMImage* pgm = new PGMImage;
	const char* ipfile;
	const char* opfile;

	ipfile = "mri.pgm";
	
	opfile = "outPgm.txt";
	printf("Input file : %s\n", ipfile);

	// Process the image and print its details
	if (readImage(pgm, ipfile))
	{
        meanPixel(pgm, ipfile);
        constructImage(pgm,"smoothOut.pgm");
		printImageDetails(pgm, ipfile, opfile);
        
	}
	return 0;
}