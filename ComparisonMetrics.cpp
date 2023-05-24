#include "BmpReadAndWrite.cpp"
void CalculateMSE(const char* outputFile, const char* originalFile)
{
    BMPImage *bmp1 = new BMPImage;
    BMPImage *bmp2 = new BMPImage;

    // Process the image and print its details
    if (readBmpImage(bmp1, outputFile))
    {
        cout << "Edge detected Image Loaded Successfully!\n";
        if (readBmpImage(bmp2, originalFile))
        {
            cout << "Original Image Loaded Successfully!\n";
        }
        else
        {
            cout << "Error Loading Original Image!\n";
            return;
        }
    }
    else
    {
        cout << "Error Loading Edge detected Image!\n";
        return;
    }

    int height = bmp1->infoHeader.height;
    int width = bmp1->infoHeader.width;
   if(height != bmp2->infoHeader.height && width != bmp2->infoHeader.width)
    {
        cout << "Invalid Dimension!\n";
        return;
    }

    double sumSquaredErr = 0.0;
    double sum1 = 0.0;
    double sum2 = 0.0;
    int cnt = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double err = bmp2->pixel[i][j][0] - bmp1->pixel[i][j][0];
            sumSquaredErr = err * err;
            sum1 += bmp1->pixel[i][j][0];
            sum2 += bmp2->pixel[i][j][0];
            cnt++;
        }
    }

    double mse = sumSquaredErr/cnt;
    cout << "MSE = " << mse << endl;
    cout << cnt  <<" " << sum1 << " " << sum2 << endl;
}
int main()
{
    CalculateMSE("blackbuck.bmp", "SobelOut.bmp");
}