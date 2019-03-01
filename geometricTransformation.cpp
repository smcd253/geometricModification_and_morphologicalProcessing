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

int inPiece(unsigned char ***sourceImageData, int **cornerCoords)
{
	// define area 
	return 0;
}
int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;
	int width = 512;
	int height = 512;
	int childA_height = 256;
	int childA_width = 256;
	
	// Check for proper syntax
	if (argc < 3)
	{
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [width = 256] [height = 256] [BytesPerPixelOut = 1]" << endl;
		return 0;
	}
	
	// Allocate child image data array
	unsigned char childA_imageData[childA_height][childA_width][BytesPerPixel];
	// Allocate parent image data arrray
	unsigned char parentImageData[height][width][BytesPerPixel];
	// Allocate dest image data array
	unsigned char destImageData[childA_height][childA_width][BytesPerPixel];

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
	for (int i = 0; i < childA_height; i++)
	{
		for (int j = 0; j < childA_width; j++)
		{
			// calculate new u and v values by multiplying x,y,1 mat with rotation mat
			// origin = corner 4 {}
			// u = x*cos(theta) + y*sin(theta)
			int u = (int)((float)(j - cornerCoordinates_childA[3][0]) * cosf(theta) + (float)(cornerCoordinates_childA[3][1] - i) * sinf(theta)) + cornerCoordinates_childA[3][0];
			// v = x*-sin(theta) + y*cos(theta)
			int v = (int)(-1*(float)(j - cornerCoordinates_childA[3][0]) * sinf(theta) + (float)(cornerCoordinates_childA[3][1] - i) * cosf(theta)) + cornerCoordinates_childA[3][1];

			if ((u < 256) && (v < 256))
			{
				if(childA_imageData[i][j][0])
					destImageData[v][u][0] = childA_imageData[i][j][0];
			}
		}
	}

	// clean up image with bilinear interpolation
	// for (int i = 0; i < childA_height; i++)
	// {
	// 	for (int j = 0; j < childA_width; j++)
	// 	{
	// 		if(!destImageData[i][j][0])
	// 		{
	// 			destImageData[i][j][0] = (unsigned char)(0.25 * (
	// 									(float)destImageData[i][j + 1][0] + 
	// 									(float)destImageData[i][j - 1][0] + 
	// 									(float)destImageData[i - 1][j][0] + 
	// 									(float)destImageData[i + 1][j][0]
	// 									));
	// 		}
	// 	}
	// }
	

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[3],"wb"))) 
	{
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}

	fwrite(destImageData, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}
