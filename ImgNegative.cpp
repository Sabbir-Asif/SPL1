#include "temp2.cpp"
void negativeImage(PGMImage* pgm)
{
     for(int i = 0; i < pgm->height ; i++)
    {
        for(int j = 0 ; j < pgm->width; j++)
        {
            pgm->data[i][j] = 255 - pgm->data[i][j]; 
			if(pgm->data[i][j] < 127)
			{
				pgm->data[i][j]= 0;
			}
			else
			{
				if(pgm->data[i][j] > 220)
				{
					pgm->data[i][j] = 210;
				}
			}
        }
    }
}

void NegativeUtility()
{
	PGMImage* pgm = new PGMImage;
	const char* ipfile;
	const char* opfile;
	ipfile = "mri.pgm";
	opfile = "outPgm.txt";
	if (readPGMImage(pgm, ipfile))
	{
		printf("Input file : %s\n", ipfile);
        negativeImage(pgm);
        constructPGMImage(pgm,"Negative.pgm");
		printf("Negative Image Operation Successful!\n");
	}
	else
	{
		printf("Error Reading Input Image in Negative Utility!\n");
	}
}


