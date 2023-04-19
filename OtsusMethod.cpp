#include "ImgReadAndWrite.cpp"
#include<cmath>

void Otsu(PGMImage* pgm, const char* ipfile)
{
    PGMImage* pgm2 = new PGMImage;
	if (!readImage(pgm2, ipfile))
	{
		cout << "File not Opened in Otsu function" << endl;
		
	}
    long long totalPixels, varMax = 0, sum = 0, sumOfBack = 0, fg = 0, bg = 0;
    double mean1 = 0.0, mean2 = 0.0, variance = 0.0;
    int threshold;
    totalPixels = pgm->height * pgm->width;
    int histogram[pgm2->maxColorValue];
    for(int i = 0; i < pgm2->maxColorValue; i++)
    {
        histogram[i] = 0;
    }
    for(int i = 0; i < pgm2->height; i++)
    {
        for(int j = 0; j < pgm2->width; j++)
        {
            histogram[(int)pgm2->data[i][j]] += 1;
        }
    }

    for(int i = 0; i <= pgm->maxColorValue; i++)
    {
        sum += i*histogram[i];
    }

    for(int pixel = 0; pixel <= pgm->maxColorValue; pixel++)
    {
        fg += histogram[pixel];
        if(fg == 0) continue;
        bg = totalPixels - fg;

        sumOfBack += pixel * histogram[pixel];
        mean1 = (double) sumOfBack / fg;
        mean2 = (double) (sum - sumOfBack) / bg;

        variance = (fg*bg) * (mean1 - mean2) * (mean1 - mean2);

        if(variance > varMax)
        {
            threshold = pixel;
            varMax = variance;
        }

    }

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
// Driver Code
int main(int argc, char const* argv[])
{
	PGMImage* pgm = new PGMImage;
	const char* ipfile;
	const char* opfile;
    int* histogram;

	if (argc == 2)
		ipfile = argv[1];
	else
		ipfile = "trafic.pgm"; 
	
	opfile = "outPgm.txt";
	printf("Input file : %s\n", ipfile);

	// Process the image and print its details
	if (readImage(pgm, ipfile))
	{
        Otsu(pgm, ipfile);
        constructImage(pgm,"OtsuOut.pgm");
        
	}
	return 0;
}
