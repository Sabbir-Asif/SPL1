#include "ImgReadAndWrite.cpp"
#include<cmath>
void meanPixel(PGMImage* pgm, const char* ipfile)
{
	PGMImage* pgm2 = new PGMImage;
	if (!readImage(pgm2, ipfile))
	{
		cout << "File not Opened in Smoothing function" << endl;
		
	}

    int arr[4][9] = {
                      {-1, 0, 1, -1, 0 , 1, -1, 0 , 1},
                      {1, 1, 1, 0, 0, 0, 1, 1, 1},
                      {1, 0, 0, 0 , 1, 0, 0, 0, 1},
                      {0, 0, 1, 0, 1, 0, 1, 0, 0},
                    };

     int effect;
     cout << "Enter effects (1, 2, 3, 4) : ";
     cin >> effect;
     effect--;
    for(int i = 1; i < (pgm->height - 1); i++)
    {
        for(int j = 1; j < (pgm->width - 1); j++)
        {
            int x = (pgm2->data[i-1][j-1] * arr[effect][0]) + (pgm2->data[i-1][j] * arr[effect][1])+ (pgm2->data[i-1][j+1] * arr[effect][2]);
            int y = (pgm2->data[i][j-1] * arr[effect][3]) + (pgm2->data[effect][j] * arr[effect][4]) + (pgm2->data[i][j+1] * arr[effect][5]);
            int z = (pgm2->data[i+1][j-1] * arr[effect][6]) + (pgm2->data[i+1][j] * arr[effect][7]) + (pgm2->data[i+1][j+1] * arr[effect][8]);
            pgm->data[i][j] = (x+y+z);
        }
    }

} 

int main()
{
	//PGMImage* pgm = malloc(sizeof(PGMImage));
	PGMImage* pgm = new PGMImage;
	const char* ipfile;
	const char* opfile;

	ipfile = "lena.pgm";
	
	opfile = "outPgm.txt";
	printf("Input file : %s\n", ipfile);
 
	// Process the image and print its details
	if (readImage(pgm, ipfile))
	{
        meanPixel(pgm, ipfile);
        constructImage(pgm,"Lenax.pgm");
		//printImageDetails(pgm, ipfile, opfile);
        
	}
	return 0;
}