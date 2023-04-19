#include "ImgReadAndWrite.cpp"

void brightening(PGMImage* pgm)
{
    cout << "---------------------Brightening--------------------\n\n";
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

    cout << "Operation Successful!\n\n" << endl;
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
		ipfile = "moon.pgm";
	
	opfile = "outPgm.txt";
	printf("\n\nInput file : %s\n", ipfile);
	// Process the image and print its details
	if (readImage(pgm, ipfile))
	{
        brightening(pgm);
        constructImage(pgm,"brightenImage.pgm");
        
	}
	return 0;
}


