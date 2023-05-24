#include "temp2.cpp"
#include<cmath>
void meanPixel(PGMImage* pgm, const char* ipfile)
{
	PGMImage* pgm2 = new PGMImage;
	if (!readPGMImage(pgm2, ipfile))
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

void SmoothingUtility()
{
	PGMImage* pgm = new PGMImage;
	const char* ipfile;
	const char* opfile;
	ipfile = "mri.pgm";
	if (readPGMImage(pgm, ipfile))
	{
		printf("Input file : %s\n", ipfile);
        meanPixel(pgm, ipfile);
        constructPGMImage(pgm,"smoothOut.pgm");
		//printImageDetails(pgm, ipfile, opfile);
		printf("Smoothing Operation Successfull!\n");
        printf("Output File smoothOut.pgm\n");
	}
	else
	{
		printf("Error Loading input Image in smoothing utility!\n");
	}
}

/*int main()
{
	SmoothingUtility();
}*/