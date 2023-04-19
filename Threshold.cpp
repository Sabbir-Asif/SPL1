#include "ImgReadAndWrite.cpp"

void ThresholdImage(PGMImage* pgm)
{
    int threshold;
    cout << "Enter threshold value : ";
    cin >> threshold;
     for(int i = 0; i < pgm->height ; i++)
    {
        for(int j = 0 ; j < pgm->width; j++)
        {
            if(pgm->data[i][j] < threshold)
            {
                pgm->data[i][j] = pgm->data[i][j] * 0;
            } 
            else
            {
                pgm->data[i][j] = (pgm->data[i][j]/pgm->data[i][j]) * pgm->maxColorValue;
            }
        }
    }
}

int main()
{
	//PGMImage* pgm = malloc(sizeof(PGMImage));
	PGMImage* pgm = new PGMImage;
	const char* ipfile;
	const char* opfile;

	ipfile = "trafic.pgm";
	
	opfile = "outPgm.txt";
	printf("Input file : %s\n", ipfile);
 
	// Process the image and print its details
	if (readImage(pgm, ipfile))
	{
        ThresholdImage(pgm);
        constructImage(pgm,"ThresholdTestOut.pgm");
		//printImageDetails(pgm, ipfile, opfile);
        
	}
	return 0;
}
