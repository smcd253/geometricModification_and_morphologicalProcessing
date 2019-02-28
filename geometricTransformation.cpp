// input: 

#include "geometricModification.hpp"

using namespace std;

int cornerCoordinates[2] = {0}; // for use with local variable

int findCornerCoords(unsigned char ***sourceImageData, int height, int width, int corner)
{
	switch(corner)
	{
		// find corner 1 by scanning left to right
		case 1:
			for (int j = 0; j < width; j++)
			{
				for (int i = 0; i < height; i++)
				{
					if (*(((unsigned char *)sourceImageData + i * width) + j) != 255)
					{
						cornerCoordinates[0] = i;
						cornerCoordinates[1] = j;
						return 1;
					}
				}
			}
		break;
		
		// find corner 2 by scanning top to bottom
		case 2:
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)	
				{
					if (*(((unsigned char *)sourceImageData + i * width) + j) != 255)
					{
						cornerCoordinates[0] = i;
						cornerCoordinates[1] = j;
						return 1;
					}
				}
			}
		break;
		
		// find corner 3 by right to left		
		case 3:
			for (int j = width - 1; j >= 0; j--)
			{
				for (int i = 0; i < height; i++)
				{
					if (*(((unsigned char *)sourceImageData + i * width) + j) != 255)
					{
						cornerCoordinates[0] = i;
						cornerCoordinates[1] = j;
						return 1;
					}
				}
			}
		break;
		
		// find corner 4 by scanning bottom to top
		case 4:
			for (int i = height - 1; i >= 0; i--)
			{
				for (int j = width - 1; j >= 0; j--)
				{
					if (*(((unsigned char *)sourceImageData + i * width) + j) != 255)
					{
						cornerCoordinates[0] = i;
						cornerCoordinates[1] = j;
						return 1;
					}
				}
			}
		break;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width, height;
	int childA_height = 256;
	int childA_width = 256;
	
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
	if (argc < 5) width = 512;
	else width = atoi(argv[4]);

	// check if height is specified
	if (argc < 6) height = 512;
	else height = atoi(argv[5]);
	
	// Allocate child image data array
	unsigned char childA_imageData[childA_height][childA_width][BytesPerPixel];
	// Allocate parent image data arrray
	unsigned char parentImageData[height][width][BytesPerPixel];
	// Allocate dest image data array
	unsigned char destImageData[height][width][BytesPerPixel];

	// Read parent image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) 
	{
		cout << "Cannot open parent file: " << argv[1] <<endl;
		exit(1);
	}
	fread(parentImageData, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	// Read child image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[2],"rb"))) 
	{
		cout << "Cannot open child (a) file: " << argv[1] <<endl;
		exit(1);
	}
	fread(childA_imageData, sizeof(unsigned char), childA_height*childA_width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int cornerCoordinates_childA[4][2] = {0};
	// find coordinates of all 4 corners
	for (int i = 0; i < 4; i++)
	{
		if (!findCornerCoords((unsigned char ***)childA_imageData, childA_height, childA_width, i + 1))
			printf("corner %d not found\n", i + 1);

		cornerCoordinates_childA[i][0] = cornerCoordinates[0];
		cornerCoordinates_childA[i][1] = cornerCoordinates[1];

		printf("corner %d = {%d, %d}\n", i + 1, cornerCoordinates_childA[i][0], cornerCoordinates_childA[i][1]);
	}

	// calculate angle between corner 4 and corner 3 (in anticipation of rotating c4-c3 edge to 0 deg, i.e. clockwise)
	float rise = abs(cornerCoordinates_childA[3][0] - cornerCoordinates_childA[2][0]);
	float run = abs(cornerCoordinates_childA[3][1] - cornerCoordinates_childA[2][1]);
	float theta = atan2f(rise,run);

	printf("theta = %f\n", theta); 

	// rotate image
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// calculate new u and v values using matrix multiplcation
			// u = x*cos(theta) + y*sin(theta)
			int u = (int)((float)j * cosf(theta) + (float)i * sinf(theta));
			// v = x*-sin(theta) + y*cos(theta)
			int v = (int)(-1*(float)j * sinf(theta) + (float)i * cosf(theta));

			if ((i > height/2 - 10 && i < height/2 + 10) && (j > width/2 -10 && j < width/2 + 10))
			{
				printf("{i,j} = {%d,%d}\n", i, j);
				printf("{v,u} = {%d,%d}\n", v, u);
			}
		}
	}

	// // Write image data (filename specified by second argument) from image data matrix
	// if (!(file=fopen(argv[2],"wb"))) 
	// {
	// 	cout << "Cannot open file: " << argv[2] << endl;
	// 	exit(1);
	// }

	// fwrite(destImageData, sizeof(unsigned char), height*width*BytesPerPixel, file);
	// fclose(file);

	return 0;
}
