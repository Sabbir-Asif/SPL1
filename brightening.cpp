#include "ImgReadAndWrite.cpp"

void brightening(PGMImage* pgm)
{
    cout << "Input the brightness level between -255 to +255 : ";
    int level;
    cin >> level;

    if(level > 255 || level < -255)
    {
        cout<<"Invalid brightness level";
        return;
    }
    else
    {
         for(int i = 0; i < pgm->height; i++)
        {
           for(int j = 0; j < pgm->width; j++)
           {
              int b = pgm->data[i][j];
              b = b + min(level, (255 - b));
              pgm->data[i][j] = max(0, b);
           }
        }
    }
}

/*
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
        brightening(pgm);
        constructImage(pgm,"brightenOut.pgm");
        
	}
	return 0;
}
*/

