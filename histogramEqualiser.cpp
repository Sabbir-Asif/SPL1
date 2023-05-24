#include "temp2.cpp"
#include<cmath>
int histogram[256];
int* createHistogram(PGMImage* pgm)
{
    //int histogram[pgm->maxColorValue];

    for(int i = 0; i < pgm->maxColorValue; i++)
    {
        histogram[i] = 0;
    }
    for(int i = 0; i < pgm->height; i++)
    {
        for(int j = 0; j < pgm->width; j++)
        {
            histogram[(int)pgm->data[i][j]] += 1;
        }
    }

    return histogram;
}

int* histogramEqualization(PGMImage* pgm, int histogram[])
{
    int newGrayLevel[pgm->maxColorValue];
    for(int i =0; i < pgm->maxColorValue; i++)
    {
        newGrayLevel[i] = 0;
    }

	double totalPixels = pgm->height * pgm->width;
    int curr = 0;
    for (int i = 0; i < 256; i++)
    {
        curr += histogram[i];
  
        // calculating new gray level after multiplying by maximum gray count 
        //and dividing by total number of pixels
        newGrayLevel[i] = round((((float)curr) * 255) / totalPixels);
    }

    //image[col] = (unsigned char)new_gray_level[image[col]];
     for(int i = 0; i < pgm->height; i++)
    {
        for(int j = 0; j < pgm->width; j++)
        {
            pgm->data[i][j] = (unsigned char)newGrayLevel[(int)pgm->data[i][j]];
        }
    }

	return histogram;
}

void printHistogram(int histogram[], int N)
{
	 // Printing histogram
    printf("Hsitogram : \n\n");
    for(int i = 0; i < N; i++)
    {
        if(i < 10)
        {
            printf("[%d]   | ",i);
            for(int j = 0; j <= (histogram[i]%20); j++)
            printf("*");
            printf("(%d)",histogram[i]);
            printf("\n");
        }
        else if(i < 100)
        {
            printf("[%d]  | ",i);
            for(int j = 0; j <= (histogram[i]%20); j++)
            printf("*");
            printf("(%d)",histogram[i]);
            printf("\n");
        }
        else if(i < 256)
        {
            printf("[%d] | ",i);
            for(int j = 0; j <= (histogram[i]%20); j++)
            printf("*");
            printf("(%d)",histogram[i]);
            printf("\n");
        }
    }
}

void HistogramEqualisationUtility()
{
	PGMImage* pgm = new PGMImage;
	const char* ipfile;
	const char* opfile;
    int* histogram;

	ipfile = "images.pgm"; 
	printf("Input file : %s\n", ipfile);
	if (readPGMImage(pgm, ipfile))
	{
		histogram = createHistogram(pgm);
        //printHistogram(histogram,pgm->maxColorValue);
        histogram = histogramEqualization(pgm,histogram);
       // printHistogram(histogram,pgm->maxColorValue);
        constructPGMImage(pgm,"EhistogramtOut.pgm");
        printf("Histogram equalisation Operation Successful!\n");
	}
    else
    {
        printf("Error Loading Image in Histogram equalisation Utility!\n");
    }

}

/*int main()
{
    HistogramEqualisationUtility();
}*/
