#include "ImgReadAndWrite.cpp"

void negativeImage(PGMImage* pgm)
{
     for(int i = 0; i < pgm->height ; i++)
    {
        for(int j = 0 ; j < pgm->width; j++)
        {
            pgm->data[i][j] = 255 - pgm->data[i][j]; 
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
        negativeImage(pgm);
        constructImage(pgm,"Negative.pgm");
		//printImageDetails(pgm, ipfile, opfile);
        
	}
	return 0;
}

