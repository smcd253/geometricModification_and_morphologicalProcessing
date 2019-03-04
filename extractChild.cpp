// input: ./extractChild HW3_images/lighthouse1.raw HW3_images/lighthouse1_extracted.raw 1 0 1
// input: ./extractChild HW3_images/lighthouse2.raw HW3_images/lighthouse2_extracted.raw 1 0 0
// input: ./extractChild HW3_images/lighthouse3.raw HW3_images/lighthouse3_extracted.raw 0 1 0
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
	int BytesPerPixel = 1;
	int childA_height = 256;
	int childA_width = 256;
	int flipOverY = 0;
	int flipOverX = 0;
	int rotateCW = 0;
	
	// Check for proper syntax
	if (argc < 3)
	{
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [width = 256] [height = 256] [BytesPerPixelOut = 1]" << endl;
		return 0;
	}
	if(argc < 4) flipOverY = 0;
	else flipOverY = atoi(argv[3]);
	if(argc < 5) flipOverX = 0;
	else flipOverX = atoi(argv[4]);
	if(argc < 6) rotateCW = 0;
	else rotateCW = atoi(argv[5]);

	// Allocate child image data array
	unsigned char childImageData[childA_height][childA_width][BytesPerPixel];
	// Allocate dest image data array
	unsigned char rotatedImageData[childA_height][childA_width][BytesPerPixel];

	// Read child image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) 
	{
		cout << "Cannot open child (a) file: " << argv[1] <<endl;
		exit(1);
	}
	fread(childImageData, sizeof(unsigned char), childA_height*childA_width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int childCornerCoordinates[4][2] = {0};
	// find coordinates of all 4 corners
	for (int i = 0; i < 4; i++)
	{
		if (!findCornerCoords((unsigned char ***)childImageData, childA_height, childA_width, i + 1))
			printf("corner %d not found\n", i + 1);

		childCornerCoordinates[i][0] = cornerCoordinates[0];
		childCornerCoordinates[i][1] = cornerCoordinates[1];

		printf("corner %d = {%d, %d}\n", i + 1, childCornerCoordinates[i][0], childCornerCoordinates[i][1]);
	}

	// calculate angle between corner 4 and corner 3 (in anticipation of rotating c4-c3 edge to 0 deg, i.e. clockwise)
	float rise = abs(childCornerCoordinates[3][0] - childCornerCoordinates[2][0]);
	float run = abs(childCornerCoordinates[3][1] - childCornerCoordinates[2][1]);
	float theta = atan2f(rise,run);

	printf("theta = %f\n", theta); 

	// throw result into new array
	// height = sqrt[(Xc2 - Xc3)^2 + (Yc2 - Yc3)^2]
	int extracted_height = sqrtf(powf(childCornerCoordinates[1][0] - childCornerCoordinates[2][0],2)
								+ powf(childCornerCoordinates[1][1] - childCornerCoordinates[2][1],2));
	// width = sqrt[(Xc1 - Xc2)^2 + (Yc1 - Yc2)^2]
	int extracted_width = sqrtf(powf(childCornerCoordinates[0][0] - childCornerCoordinates[1][0],2)
								+ powf(childCornerCoordinates[0][1] - childCornerCoordinates[1][1],2));
	if(rotateCW)
		printf("new image dimensions {width,height} = {%d,%d}\n", extracted_height, extracted_width);
	else
		printf("new image dimensions {width,height} = {%d,%d}\n", extracted_width, extracted_height);
	
	// extraacted image data locations - dedicated rotate matrix when exchanging width and height values (not all are square)
	unsigned char extractedImageData[extracted_height][extracted_width];
	unsigned char extractedImageData_rot[extracted_width][extracted_height];

	// rotate image around center (reverse mapping)
	int x_center = 0.5 * (childCornerCoordinates[0][0] + childCornerCoordinates[2][0]);
	int y_center = 0.5 * (childCornerCoordinates[0][1] + childCornerCoordinates[2][1]);

	// new corners for extracted image
	int cornerCoordinates_extracted[4][2] = {0};

	// calculate u and v using reverse mapping
	for (int v = 0; v < childA_height; v++)
	{
		for (int u = 0; u < childA_width; u++)
		{
			// calculate new u and v values by multiplying x,y,1 mat with rotation mat
			// x = (u-x_origin)*cos(theta) + (y_origin-v)*sin(theta) + x_origin
			float x = (float)(u - x_center) * cosf(theta) - (float)(v - y_center) * sinf(theta) + x_center;
			// y = x*-sin(theta) + y*cos(theta)
			float y = (float)(u - x_center) * sinf(theta) + (float)(v - y_center) * cosf(theta) + y_center;

			// bilinear interpolate source image
			float weight_x = x - floorf(x);
			float weight_y = y - floorf(y);
			float weightMag = sqrtf(powf(weight_x,2) + powf(weight_y,2));
			int xFloor = (int)x;
			int xCeiling = (int)ceilf(x);
			int yFloor = (int)y;
			int yCeiling = (int)ceilf(y);

			// check to see if x and y correspond to a corner from the source image
			// if so, mark u and v as the same corner and use this to build an image
			for (int i = 0; i < 4; i++)
			{
				if(xFloor == childCornerCoordinates[i][0] && yFloor == childCornerCoordinates[i][1])
				{
					cornerCoordinates_extracted[i][0] = u;
					cornerCoordinates_extracted[i][1] = v;
				}
				else if (xCeiling == childCornerCoordinates[i][0] && yCeiling == childCornerCoordinates[i][1])
				{
					cornerCoordinates_extracted[i][0] = u;
					cornerCoordinates_extracted[i][1] = v;
				}
			}

			// bilinear interpolation to grab surrounding pixel values
			if(x < 256 && y < 256)
				rotatedImageData[v][u][0] = (unsigned char)(0.25 * (
										childImageData[xFloor][yFloor][0] + 
										childImageData[xFloor][yCeiling][0] + 
										childImageData[xCeiling][yFloor][0] + 
										childImageData[xCeiling][yCeiling][0]
										));
		}
	}

	// print out extracted corners
	printf("extracted corners: \n");
	for (int i = 0; i < 4; i++)
	{
		printf("corner %d = {%d,%d}\n", i + 1, cornerCoordinates_extracted[i][0], cornerCoordinates_extracted[i][1]);
	}
	// extract corrected image from rotated image
	// loop from top left corner to bottom right
	int iE = 0;
	int jE = 0;
	if (rotateCW)
	{
		for (int j = cornerCoordinates_extracted[1][0]; j <= cornerCoordinates_extracted[3][0]; j++)
		{
			for (int i = cornerCoordinates_extracted[2][1]; i >= cornerCoordinates_extracted[3][1]; i--)
			{
				if (flipOverY) 
				{
					if (flipOverX)
						extractedImageData_rot[extracted_width - iE][extracted_height - jE] = rotatedImageData[i][j][0];
					else extractedImageData_rot[iE][extracted_height - jE] = rotatedImageData[i][j][0];
				}
				else if (flipOverX) extractedImageData_rot[extracted_width - iE][jE] = rotatedImageData[i][j][0];
				else extractedImageData_rot[iE][jE] = rotatedImageData[i][j][0];
				jE++;
			}
			jE = 0;
			iE++;
		}
	}
		
	else
	{
		for (int i = cornerCoordinates_extracted[0][1]; i <= cornerCoordinates_extracted[2][1]; i++)
		{
			for (int j = cornerCoordinates_extracted[0][0]; j <= cornerCoordinates_extracted[2][0]; j++)
			{
				if (flipOverY) 
				{
					if (flipOverX)
						extractedImageData[extracted_height - iE][extracted_width - jE] = rotatedImageData[i][j][0];
					else extractedImageData[iE][extracted_width - jE] = rotatedImageData[i][j][0];
				}
				else if (flipOverX) extractedImageData[extracted_height - iE][jE] = rotatedImageData[i][j][0];
				else extractedImageData[iE][jE] = rotatedImageData[i][j][0];
				jE++;
			}
			jE = 0;
			iE++;
		}
	}
		
	//printf("ending corner val for extracted image = {%d,%d}\n", jE, iE);
	
	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) 
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	if(rotateCW)
		fwrite(extractedImageData_rot, sizeof(unsigned char), extracted_height*extracted_width, file);
	else
		fwrite(extractedImageData, sizeof(unsigned char), extracted_height*extracted_width, file);
	fclose(file);


	return 0;
}
