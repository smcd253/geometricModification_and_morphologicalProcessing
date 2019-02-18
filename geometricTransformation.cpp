// input: ./shrinking HW3_images/pattern1.raw HW3_images/pattern1-shrink.raw 1 375 375

#include "geometricModification.hpp"

using namespace std;


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
	
	int input[8] = {0};
	int intermediate[8] = {0};
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// center pixel to be changed
			int X = buildInput((unsigned char ***)sourceImageData, input, height, width, i, j);			

			// run input through first filter and build intermediate array
			int M = filterOne(X, input, intermediate);

			// run intermediate array through 2nd filter
			int P = filterTwo(M, intermediate);

			// calculate output
			if (X && (!M || P)) destImageData[i][j][0] = 255;
			else destImageData[i][j][0] = 0;;
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
