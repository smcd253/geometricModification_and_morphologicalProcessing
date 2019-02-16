// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of width 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different widths and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

// hit masks
const char *SB1_1 = "01000000";
const char *SB1_2 = "00010000";
const char *SB1_3 = "00000100";
const char *SB1_4 = "00000001";

const char *SB2_1 = "10000000";
const char *SB2_2 = "00100000";
const char *SB2_3 = "00001000";
const char *SB2_4 = "00000010";



// void diffuseErrorLeft(int ***errorMatrix, int height, int width, int i, int j, int N, int select, int sub)
// {
// 	for (int nh = -(N-1)/2; nh <= (N-1)/2; nh++)
// 	{
// 		for (int nw = -(N-1)/2; nw <= (N-1)/2; nw++)
// 		{
//             switch(select)
//             {
//                 case 1:
// 			        *(((int *)errorMatrix + (i+nh)*width) + (j + nw)) += (FS[nh][(N-1)/2 - nw]/16.0) * (*(((int *)errorMatrix + (i)*width) + (j)) - sub);
//                 break;
//                 case 2:
// 			        *(((int *)errorMatrix + (i+nh)*width) + (j + nw)) += (JJN[nh][(N-1)/2 - nw]/48.0) * (*(((int *)errorMatrix + (i)*width) + (j)) - sub);
//                 break;
//                 case 3:
// 			        *(((int *)errorMatrix + (i+nh)*width) + (j + nw)) += (STU[nh][(N-1)/2 - nw]/42.0) * (*(((int *)errorMatrix + (i)*width) + (j)) - sub);
//                 break;
//             }
//         }    
// 	}
// }

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width, height;
	int select;
    int th;
	
	// Check for proper syntax
	if (argc < 3)
	{
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [width = 256] [height = 256] [BytesPerPixelOut = 1]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4) BytesPerPixel = 1; // default is grey image
	else BytesPerPixel = atoi(argv[3]);

	// Check if width is specified
	if (argc < 5) width = 256;
	else width = atoi(argv[4]);

	// check if height is specified
	if (argc < 6) height = 256;
	else height = atoi(argv[5]);

	// check if I for index matrix is specified
	if (argc < 7) select = 1;
	else select = atoi(argv[6]);

    // threshold
    if (argc < 8) th = 127;
	else th = atoi(argv[7]);
	
	
    // int array to hold error diffusion data (2 rows and columns bigger to accomodate diffusion at edges)
    int errDiff[height + 2][width + 2][BytesPerPixel];
	// Allocate source image data array
	unsigned char sourceImageData[height][width][BytesPerPixel];
	// Allocate dest image data array
	unsigned char destImageData[height][width][1];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) 
	{
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(sourceImageData, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    // change window size depending on the diff matrix we watnt to use
    int N = 3;
    switch(select)
    {
        case 2:
            N = 5;
        break;
        case 3:
            N = 5;
        break;   
    }
    // compare pixels to threshold matrix and half-tone accordingly
	for (int i = 0; i < height; i++)
	{
        if(i%2) // snake right
        {
            for (int j = 0; j < width; j++)
            {
                if(sourceImageData[i][j][0] + errDiff[i][j][0] > th)
                {
                    diffuseErrorRight((int ***)errDiff, height, width, i, j, N, select, 255);
                    destImageData[i][j][0] = 255;   
                }
                else
                {
                    diffuseErrorRight((int ***)errDiff, height, width, i, j, N, select, 0);
                    destImageData[i][j][0] = 0;   
                }
            }
        }
        else // snake left
        {
            for (int j = width - 1; j >= 0; j--)
            {
                if(sourceImageData[i][j][0] + errDiff[i][j][0] > th)
                {
                    diffuseErrorLeft((int ***)errDiff, height, width, i, j, N, select, 255);
                    destImageData[i][j][0] = 255;   
                }
                else
                {
                    diffuseErrorLeft((int ***)errDiff, height, width, i, j, N, select, 0);
                    destImageData[i][j][0] = 0;   
                }
            }
        }
	}

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) 
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}

	fwrite(destImageData, sizeof(unsigned char), height*width, file);
	fclose(file);

	return 0;
}
