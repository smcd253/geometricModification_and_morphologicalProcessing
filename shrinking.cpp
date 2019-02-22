// input: ./shrinking HW3_images/pattern1.raw HW3_images/pattern1-shrink.raw

#include "shrinking.hpp"

using namespace std;

int zeroPad(unsigned char ***sourceImageData, int height, int width, int i, int j, int nh, int nw)
{
	if(i + nh >= 0)
	{
		if (i + nh < height - 1)
		{
			if(j + nw >= 0)
			{
				if (j + nw < width - 1)
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

int filterOne(unsigned char ***sourceImageData, int height, int width, int i, int j, int X, int *inputArr, int *intermediateArr)
{
	int maskMatch = 1;
    if (X == 1)
    {
        if(!filterHelper(inputArr, NUM_COND_MASKS))
		{
			maskMatch = 0;
		}
		// if mask hit, run mask filter on surrounding pixels
		if (maskMatch)
		{
			int perifInput[9] = {0};
			int iterator = 0;
			for(int n = -1; n <= 1; n++)
			{
				for (int m = -1; m<= 1; m++)
				{
					if ((n != 0) && (m != 0)) // avoid center pixel
					{
						perifInput[8] = buildInput((unsigned char ***)sourceImageData, perifInput, height, width, i + n, j + m);
						// throw result of mask match on periferal number on intermediate array
						if(perifInput[8]) *(intermediateArr + iterator) = filterHelper(perifInput, NUM_COND_MASKS);
						iterator++;
					}
				}
			}
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

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int input[8] = {0};
			int intermediate[8] = {0};

			// center pixel to be changed
			int X = buildInput((unsigned char ***)sourceImageData, input, height, width, i, j);			

			// run input through first filter and build intermediate array
			int M = filterOne((unsigned char ***)sourceImageData, height, width, i, j, X, input, intermediate);

			// run intermediate array through 2nd filter
			int P = filterTwo(M, intermediate);

			// calculate output
			destImageData[i][j][0] = 255 * (X && (!M || P));
			if(X == 1)
				printf("X | M | P = %d | %d | %d\n", X, M, P);
		}
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
