#include "ImgReadAndWrite.cpp"
#include<cmath>
void convulation(PGMImage* pgm, const char* ipfile)
{
	PGMImage* pgm2 = new PGMImage;
	if (!readImage(pgm2, ipfile))
	{
		cout << "File not Opened in Smoothing function" << endl;
	}
    //These are all temporary variables
    int x,y,z,temp1,temp2;
    int arr[4][9] = {
                      {-1, 0, 1, -1, 0 , 1, -1, 0 , 1},
                      {-1, -1, -1, 0, 0, 0, 1, 1, 1},
                      {-1, 0, 1, -2 , 0, 2, -1, 0, 1},
                      {-1, -2, -1, 0, 0, 0, 1, 2, 1},
                    };

     int effect;
     cout << "Enter effects (1, 2) : ";
     cin >> effect;
    
    if(effect == 1)
    {
        for(int i = 1; i < (pgm->height - 1); i++)
        {
           for(int j = 1; j < (pgm->width - 1); j++)
           {
                x = (pgm2->data[i-1][j-1] * arr[0][0]) + (pgm2->data[i-1][j] * arr[0][1])+ (pgm2->data[i-1][j+1] * arr[0][2]);
                y = (pgm2->data[i][j-1] * arr[0][3]) + (pgm2->data[effect][j] * arr[0][4]) + (pgm2->data[i][j+1] * arr[0][5]);
                z = (pgm2->data[i+1][j-1] * arr[0][6]) + (pgm2->data[i+1][j] * arr[0][7]) + (pgm2->data[i+1][j+1] * arr[0][8]);
                temp1 = (x+y+z);

                x = (pgm2->data[i-1][j-1] * arr[1][0]) + (pgm2->data[i-1][j] * arr[1][1])+ (pgm2->data[i-1][j+1] * arr[1][2]);
                y = (pgm2->data[i][j-1] * arr[1][3]) + (pgm2->data[effect][j] * arr[1][4]) + (pgm2->data[i][j+1] * arr[1][5]);
                z = (pgm2->data[i+1][j-1] * arr[1][6]) + (pgm2->data[i+1][j] * arr[1][7]) + (pgm2->data[i+1][j+1] * arr[1][8]);
                temp2 = (x+y+z);

               temp1 = temp1 * temp1;
               temp2 = temp2 * temp2;

               pgm->data[i][j] = (unsigned char)sqrt((temp1 + temp2));
           }
        }
    }
    else if(effect == 2)
    {
        for(int i = 1; i < (pgm->height - 1); i++)
        {
           for(int j = 1; j < (pgm->width - 1); j++)
           {
                x = (pgm2->data[i-1][j-1] * arr[2][0]) + (pgm2->data[i-1][j] * arr[2][1])+ (pgm2->data[i-1][j+1] * arr[2][2]);
                y = (pgm2->data[i][j-1] * arr[2][3]) + (pgm2->data[effect][j] * arr[2][4]) + (pgm2->data[i][j+1] * arr[2][5]);
                z = (pgm2->data[i+1][j-1] * arr[2][6]) + (pgm2->data[i+1][j] * arr[2][7]) + (pgm2->data[i+1][j+1] * arr[2][8]);
                temp1 = (x+y+z);

                x = (pgm2->data[i-1][j-1] * arr[3][0]) + (pgm2->data[i-1][j] * arr[3][1])+ (pgm2->data[i-1][j+1] * arr[3][2]);
                y = (pgm2->data[i][j-1] * arr[3][3]) + (pgm2->data[effect][j] * arr[3][4]) + (pgm2->data[i][j+1] * arr[3][5]);
                z = (pgm2->data[i+1][j-1] * arr[3][6]) + (pgm2->data[i+1][j] * arr[3][7]) + (pgm2->data[i+1][j+1] * arr[3][8]);
                temp2 = (x+y+z);

               temp1 = temp1 * temp1;
               temp2 = temp2 * temp2;

               pgm->data[i][j] = (unsigned char)sqrt((temp1 + temp2));
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

	ipfile = "pbmlib.pgm";
	
	opfile = "outPgm.txt";
	printf("Input file : %s\n", ipfile);
 
	// Process the image and print its details
	if (readImage(pgm, ipfile))
	{
        convulation(pgm, ipfile);
        constructImage(pgm,"ImageEdgeType22.pgm");
		//printImageDetails(pgm, ipfile, opfile);
        
	}
	return 0;
}