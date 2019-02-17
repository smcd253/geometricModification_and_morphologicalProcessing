// input: ./shrinking HW3_images/pattern1.raw HW3_images/pattern1-shrink.raw 1 375 375

#include "shrinking.hpp"

using namespace std;


int zeroPadY(int height, int i, int nh)
{
	if(i + nh >= 0)
	{
		if (i + nh < height - 1)
			return i + nh;
		else
			return 0;
	}
	else
		return 0;
}
int zeroPadX(int width, int j, int nw)
{
	if(j + nw >= 0)
	{
		if (j + nw < width - 1)
			return j + nw;
		else
			return 0;
	}
	else
		return 0;
}

void buildInput(unsigned char ***sourceImageData, int* inputArr, int height, int width, int i, int j)
{
	int index = 0;
	for (int nh = -1; nh <= 1; nh++)
	{
		for (int nw = -1; nw <= 1; nw++)
		{
			unsigned char pixel = *(((unsigned char *)sourceImageData + zeroPadY(height, i, nh)*width) + zeroPadX(width, j, nw));
			if ((nh != 0) && (nw != 0))
			{	
				if (pixel == 255) *(inputArr + index) = 1;
				else *(inputArr + index) = 0;
				index++;				
			}
		}
	}
}

int filterOne(int *inputArr, int *intermediateArr)
{
	int M = 1;
	for (int msk = 0; msk < NUM_COND_MASKS; msk++)
	{
		for (int i = 0; i < 8; i++)
		{
			int boolOp = *(inputArr + i) & condSMasks[msk][i];
			*(intermediateArr + i) |= boolOp; 
			M &= boolOp;
		}
	}	
	return M;	
}

int filterTwo(int *intermediateArr)
{
	int P = 1;
	for (int msk = 0; msk < NUM_COND_MASKS; msk++)
	{
		for (int i = 0; i < 8; i++)
		{
			P &= *(intermediateArr + i) & uncondSTMasks[msk][i];
		}
	}
	return P;		
}
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
	//intermediate array to hold output of first filter
	
	int input[9] = {0};
	int intermediate[9] = {0};
	int M = 0;
	int P = 0;
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// center pixel to be changed
			int X = (int)sourceImageData[i][j][0];
			// built input array
			buildInput((unsigned char ***)sourceImageData, input, height, width, i, j);

			// run input through first filter and build intermediate array
			int M = filterOne(input, intermediate);

			// run intermediate array through 2nd filter
			int P = filterTwo(intermediate);

			// calculate output
			if (X && (!M || P)) destImageData[i][j][0] = 255;
			else destImageData[i][j][0] = 0;
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
