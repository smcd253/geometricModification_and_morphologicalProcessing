// input: ./shrinking HW3_images/pattern1.raw HW3_images/pattern1-shrink.raw

#include "shrinking-old.hpp"

using namespace std;

unsigned char zeroPad(unsigned char ***sourceImageData, int height, int width, int i, int j, int nh, int nw)
{
	if(i + nh >= 0)
	{
		if (i + nh < height)
		{
			if(j + nw >= 0)
			{
				if (j + nw < width)
					return *(((unsigned char *)sourceImageData + (i + nh)*width) + (j + nw));
				else
					return 0;
			}
			else
				return 0;
		}
		else
			return 0;
	}
	else
		return 0;
}

int zeroPad_int(int **mMap, int height, int width, int i, int j, int nh, int nw)
{
	if(i + nh >= 0)
	{
		if (i + nh <= height)
		{
			if(j + nw >= 0)
			{
				if (j + nw < width)
					return *(((int *)mMap + (i + nh)*width) + (j + nw));
				else
					return 0;
			}
			else
				return 0;
		}
		else
			return 0;
	}
	else
		return 0;
}

int buildInput(unsigned char ***sourceImageData, int* inputArr, int height, int width, int i, int j)
{
    unsigned char pixels[9] = {0};
	pixels[0] = zeroPad((unsigned char ***)sourceImageData, height, width, i, j, 0, 1);
    pixels[1] = zeroPad((unsigned char ***)sourceImageData, height, width, i, j, -1, 1);
    pixels[2] = zeroPad((unsigned char ***)sourceImageData, height, width, i, j, -1, 0);
    pixels[3] = zeroPad((unsigned char ***)sourceImageData, height, width, i, j, -1, -1);
    pixels[4] = zeroPad((unsigned char ***)sourceImageData, height, width, i, j, 0, -1);
    pixels[5] = zeroPad((unsigned char ***)sourceImageData, height, width, i, j, 1, -1);
    pixels[6] = zeroPad((unsigned char ***)sourceImageData, height, width, i, j, 1, 0);
    pixels[7] = zeroPad((unsigned char ***)sourceImageData, height, width, i, j, 1, 1);
    pixels[8] = zeroPad((unsigned char ***)sourceImageData, height, width, i, j, 0, 0); // center pixel to be returnd by function

    for (int i = 0; i < 8; i++)
    {	
        if (pixels[i] > 0) *(inputArr + i) = 1;
        else *(inputArr + i) = 0;			
    }
    if (pixels[8] > 0) return 1;
    else return 0;
}

int buildInput_int(int **mMap, int* inputArr, int height, int width, int i, int j)
{
	*(inputArr + 0) = zeroPad_int((int **)mMap, height, width, i, j, 0, 1);
    *(inputArr + 1) = zeroPad_int((int **)mMap, height, width, i, j, -1, 1);
    *(inputArr + 2) = zeroPad_int((int **)mMap, height, width, i, j, -1, 0);
    *(inputArr + 3) = zeroPad_int((int **)mMap, height, width, i, j, -1, -1);
    *(inputArr + 4) = zeroPad_int((int **)mMap, height, width, i, j, 0, -1);
    *(inputArr + 5) = zeroPad_int((int **)mMap, height, width, i, j, 1, -1);
    *(inputArr + 6) = zeroPad_int((int **)mMap, height, width, i, j, 1, 0);
    *(inputArr + 7) = zeroPad_int((int **)mMap, height, width, i, j, 1, 1);
    *(inputArr + 8) = zeroPad_int((int **)mMap, height, width, i, j, 0, 0); // center pixel to be returnd by function

    return *(inputArr + 8);
}

int filterHelper(int *inputArr, int numMasks)
{
	for (int msk = 0; msk < numMasks; msk++)
	{
		int maskMatch = 1;
		for (int i = 0; i < 8; i++)
		{
			// if no match, break and compare to next mask
			// to handle both cond mask and uncond mask lookup
			if (*(inputArr + i) != (numMasks == NUM_COND_MASKS) ? condSMasks[msk][i] : uncondSTMasks[msk][i])
			{
				maskMatch = 0;
			}
		}
		if(maskMatch)
			return 1;
	}	
	return 0;
}

int filterOne(int X, int *inputArr)
{
	int maskMatch = 1;
    if (X == 1)
    {
        if(!filterHelper(inputArr, NUM_COND_MASKS))
		{
			maskMatch = 0;
		}
    }
	return maskMatch;	
}

int filterTwo(int M, int *intermediateArr)
{
    if (M == 1)
    {
		return filterHelper(intermediateArr, NUM_UNCOND_MASKS);
    }
	return 0;				
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width, height;

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
	if (argc < 5) width = 375;
	else width = atoi(argv[4]);

	// check if height is specified
	if (argc < 6) height = 375;
	else height = atoi(argv[5]);
	
	// Allocate source image data array
	unsigned char sourceImageData[height][width][BytesPerPixel];
	// Allocate intermediate image data array
	unsigned char intermediateImageData[height][width][BytesPerPixel];
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
	int runFlag = 1;
	int roundNum = 0;
	while(runFlag)
	{
		int iMap[height][width];
		int mMap[height][width];	
		// build intermediate array by mask matching every binarized pixel in the input image matrix
		for (int j = 0; j < width; j++)
		{
			for (int i = 0; i < height; i++)
			{
				int input[8] = {0};
				
				// center pixel to be changed
				// X value goes into iMap matrix
				iMap[i][j] = buildInput((unsigned char ***)sourceImageData, input, height, width, i, j);			

				// run input through first filter and build intermediate array
				// M value goes into mMap matrix
				mMap[i][j] = filterOne(iMap[i][j], input);
			}
		}
		
		// build final output array by mask matching every value in the mMap matrix
		for (int j = 0; j < width; j++)
		{
			for (int i = 0; i < height; i++)
			{
				int intermediate[8] = {0};
				int M = buildInput_int((int **)mMap, intermediate, height, width, i, j);
				
				// run intermediate array through 2nd filter
				int P = filterTwo(M, intermediate);

				// calculate output
				intermediateImageData[i][j][0] = 255 * (iMap[i][j] && (!M || P));
			}
		}

		// check to see if image has changed, if not, exit loop
		// allocate data to destImageData
		int similar = 1;
		for (int j = 0; j < width; j++)
		{
			for (int i = 0; i < height; i++)
			{
				if(intermediateImageData[i][j][0] != sourceImageData[i][j][0])
				{
					printf("round %d: different at {%d,%d}\n", roundNum, i, j);
					sourceImageData[i][j][0] = intermediateImageData[i][j][0];
					similar = 0;
				}
				destImageData[i][j][0] = sourceImageData[i][j][0];
			}
		}

		if(similar) runFlag = 0;
		else roundNum++;
	}
	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) 
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}

	fwrite(destImageData, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}
