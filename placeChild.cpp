// input: ./placeChild HW3_images/lighthouse.raw HW3_images/lighthouse1_extracted.raw HW3_images/lighthouse_filled.raw 112 108 1
// input: ./placeChild HW3_images/lighthouse_filled.raw HW3_images/lighthouse2_extracted.raw HW3_images/lighthouse_filled.raw 143 142 2
// input: ./placeChild HW3_images/lighthouse_filled.raw HW3_images/lighthouse3_extracted.raw HW3_images/lighthouse_filled.raw 209 214 3

#include "geometricModification.hpp"

using namespace std;

int cornerCoordinates[2] = {0}; // for use with local variable

int findCornerCoords(unsigned char ***sourceImageData, int width, int height, cornerData cData, int corner)
{
	// flag buffer to make sure we don't find any stray pixels at level 255 (like a simple 1d filter)
	int flagBuffer = 0;
	switch(corner)
	{
		// find corner 1 by scanning top left to bottom tight
		case 1:
			for (int j = cData.corner_startCoords[0][0]; j < cData.corner_endCoords[0][0]; j++)
			{
				for (int i = cData.corner_startCoords[0][1]; i < cData.corner_endCoords[0][1]; i++)
				{
					if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && !flagBuffer)
					{
						cornerCoordinates[0] = j;
						cornerCoordinates[1] = i;
						flagBuffer++;
					}
					else if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && flagBuffer < 5 && flagBuffer)
					{
						flagBuffer++;
					}
					else if (*(((unsigned char *)sourceImageData + i * width) + j) != 255 && flagBuffer < 5 && flagBuffer)
					{
						cornerCoordinates[0] = 0;
						cornerCoordinates[1] = 0;
						flagBuffer = 0;
					}
					if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && flagBuffer >= 5)
					{
						return 1;
					}
				}
			}
		break;
		
		// find corner 2 by scanning top right to bottom left
		case 2:
			for (int j = cData.corner_startCoords[1][0]; j > cData.corner_endCoords[1][0]; j--)
			{
				for (int i = cData.corner_startCoords[1][1]; i < cData.corner_endCoords[1][1]; i++)
				{
					if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && !flagBuffer)
					{
						cornerCoordinates[0] = j;
						cornerCoordinates[1] = i;
						flagBuffer++;
					}
					else if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && flagBuffer < 5 && flagBuffer)
					{
						flagBuffer++;
					}
					else if (*(((unsigned char *)sourceImageData + i * width) + j) != 255 && flagBuffer < 5 && flagBuffer)
					{
						cornerCoordinates[0] = 0;
						cornerCoordinates[1] = 0;
						flagBuffer = 0;
					}
					else if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && flagBuffer >= 5)
					{
						return 1;
					}
				}
			}
		break;
		
		// find corner 3 by scanning bottom right to top left		
		case 3:
			for (int j = cData.corner_startCoords[2][0]; j >= cData.corner_endCoords[2][0]; j--)
			{
				for (int i = cData.corner_startCoords[2][1]; i >= cData.corner_endCoords[2][1]; i--)
				{
					if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && !flagBuffer)
					{
						cornerCoordinates[0] = j;
						cornerCoordinates[1] = i;
						flagBuffer++;
					}
					else if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && flagBuffer < 5 && flagBuffer)
					{
						flagBuffer++;
					}
					else if (*(((unsigned char *)sourceImageData + i * width) + j) != 255 && flagBuffer < 5 && flagBuffer)
					{
						cornerCoordinates[0] = 0;
						cornerCoordinates[1] = 0;
						flagBuffer = 0;
					}
					if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && flagBuffer >= 5)
					{
						return 1;
					}
				}
			}
		break;
		
		// find corner 4 by scanning bottom to top
		case 4:
			for (int j = cData.corner_startCoords[3][0]; j < cData.corner_endCoords[3][0]; j++)
			{
				for (int i = cData.corner_startCoords[3][1]; i >= cData.corner_endCoords[3][1]; i--)
				{
					if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && !flagBuffer)
					{
						cornerCoordinates[0] = j;
						cornerCoordinates[1] = i;
						flagBuffer++;
					}
					else if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && flagBuffer < 5 && flagBuffer)
					{
						flagBuffer++;
					}
					else if (*(((unsigned char *)sourceImageData + i * width) + j) != 255 && flagBuffer < 5 && flagBuffer)
					{
						cornerCoordinates[0] = 0;
						cornerCoordinates[1] = 0;
						flagBuffer = 0;
					}
					if (*(((unsigned char *)sourceImageData + i * width) + j) == 255 && flagBuffer >= 5)
					{
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
	int child_height = 256;
	int child_width = 256;
	int holeNum;
	
	// Check for proper syntax
	if (argc < 3)
	{
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [width = 256] [height = 256] [BytesPerPixelOut = 1]" << endl;
		return 0;
	}
	// take in child width and height
	if (argc < 5) child_width = 256; 
	else child_width = atoi(argv[4]);
	if (argc < 6) child_height = 256; 
	else child_height = atoi(argv[5]);
	if(argc < 7) holeNum = 1;
	else holeNum = atoi(argv[6]);

	if(holeNum <= 0) holeNum = 1;
	else if (holeNum > 3) holeNum = 3;

	// Allocate child image data array
	unsigned char childA_imageData[child_height][child_width][BytesPerPixel];
	// Allocate parent image data arrray
	unsigned char parentImageData[height][width][BytesPerPixel];

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
		cout << "Cannot open child (a) file: " << argv[2] <<endl;
		exit(1);
	}
	fread(childA_imageData, sizeof(unsigned char), child_height*child_width, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int holeCoordinates[4][2] = {0};

	cornerData holes[3];
	holes[0].corner_startCoords[0][0] = 0; // top left of quad 1
	holes[0].corner_startCoords[0][1] = 0;
	holes[0].corner_endCoords[0][0] = width/2; // bottom right of quad 1
	holes[0].corner_endCoords[0][1] = height - 1;
	holes[0].corner_startCoords[1][0] = width/2; // top right of quad 1
	holes[0].corner_startCoords[1][1] = 0;
	holes[0].corner_endCoords[1][0] = 0; // bottom left of quad 1
	holes[0].corner_endCoords[1][1] = height - 1;
	holes[0].corner_startCoords[2][0] = width/2; // bottom right of quad 1
	holes[0].corner_startCoords[2][1] = height - 1;
	holes[0].corner_endCoords[2][0] = 0;  // top left of quad 1
	holes[0].corner_endCoords[2][1] =  0;
	holes[0].corner_startCoords[3][0] = 0; // bottom left of quad 1
	holes[0].corner_startCoords[3][1] = height - 1;
	holes[0].corner_endCoords[3][0] = width/2; // top right of quad 1
	holes[0].corner_endCoords[3][1] = 0;

	holes[1].corner_startCoords[0][0] = width/2; // top left of quad 2
	holes[1].corner_startCoords[0][1] = 0;
	holes[1].corner_endCoords[0][0] = width - 1; // bottom right of quad 2
	holes[1].corner_endCoords[0][1] = height/2;
	holes[1].corner_startCoords[1][0] = width - 1; // top right of quad 2
	holes[1].corner_startCoords[1][1] = 0;
	holes[1].corner_endCoords[1][0] = width/2; // bottom left of quad 2
	holes[1].corner_endCoords[1][1] = height/2;
	holes[1].corner_startCoords[2][0] = width - 1; // bottom right of quad 2
	holes[1].corner_startCoords[2][1] = height/2;
	holes[1].corner_endCoords[2][0] = width/2; 
	holes[1].corner_endCoords[2][1] = 0;
	holes[1].corner_startCoords[3][0] = width/2;
	holes[1].corner_startCoords[3][1] = height/2;
	holes[1].corner_endCoords[3][0] = width - 1;
	holes[1].corner_endCoords[3][1] = 0;

	holes[2].corner_startCoords[0][0] = width/2; // top left of quad 3
	holes[2].corner_startCoords[0][1] = height/2;
	holes[2].corner_endCoords[0][0] = width - 1; // bottom right of quad 3
	holes[2].corner_endCoords[0][1] = height - 1;
	holes[2].corner_startCoords[1][0] = width - 1; // top right of quad 3
	holes[2].corner_startCoords[1][1] = height/2;
	holes[2].corner_endCoords[1][0] = width/2; // bottom left of quad 3
	holes[2].corner_endCoords[1][1] = height - 1;
	holes[2].corner_startCoords[2][0] = width - 1; // bottom right of quad 3
	holes[2].corner_startCoords[2][1] = height - 1;
	holes[2].corner_endCoords[2][0] = width/2;  // top left of quad 3
	holes[2].corner_endCoords[2][1] =  height/2;
	holes[2].corner_startCoords[3][0] = width/2; // bottom left of quad 3
	holes[2].corner_startCoords[3][1] = height - 1;
	holes[2].corner_endCoords[3][0] = width - 1; // top right of quad 3
	holes[2].corner_endCoords[3][1] = height/2;
	
	// find coordinates of all 4 corners of selected hole

	printf("hole %d corner cordinates: \n", holeNum);
	for (int i = 0; i < 4; i++)
	{
		if (!findCornerCoords((unsigned char ***)parentImageData, width, height, holes[holeNum - 1], i + 1))
			printf("corner %d not found\n", i + 1);

		holeCoordinates[i][0] = cornerCoordinates[0];
		holeCoordinates[i][1] = cornerCoordinates[1];

		printf("corner %d = {%d, %d}\n", i + 1, holeCoordinates[i][0], holeCoordinates[i][1]);
	}

	// height = sqrt[(Xc2 - Xc3)^2 + (Yc2 - Yc3)^2]
	int hole_height = sqrtf(powf(holeCoordinates[1][0] - holeCoordinates[2][0],2)
								+ powf(holeCoordinates[1][1] - holeCoordinates[2][1],2));
	// width = sqrt[(Xc1 - Xc2)^2 + (Yc1 - Yc2)^2]
	int hole_width = sqrtf(powf(holeCoordinates[0][0] - holeCoordinates[1][0],2)
								+ powf(holeCoordinates[0][1] - holeCoordinates[1][1],2));
	printf("hole %d dimensions {width,height} = {%d,%d}\n", holeNum,hole_width, hole_height);

	
	// find center
	// int x_center = 0.5 * (holeCoordinates[0][0] + holeCoordinates[2][0]);
	// int y_center = 0.5 * (holeCoordinates[0][1] + holeCoordinates[2][1]);


	// scale image to hole dimensions
	unsigned char scaledChild[hole_height][hole_width];
	float yScale = (float)hole_height/(float)child_height;
	float xScale = (float)hole_width/(float)child_width;
	printf("xScale = %f\nyScale = %f\n", xScale, yScale);

	// calculate u and v using reverse mapping
	for (int u = 0; u < hole_width; u++)
	{
		for (int v = 0; v < hole_height; v++)	
		{
			// scale u and v values to find source pixel value
			float x = (float)u / xScale;
			float y = (float)v / yScale;

			// bilinear interpolate source image
			// float weight_x = x - floorf(x);
			// float weight_y = y - floorf(y);
			// float weightMag = sqrtf(powf(weight_x,2) + powf(weight_y,2));
			int xFloor = (int)x;
			int xCeiling = (int)ceilf(x);
			int yFloor = (int)y;
			int yCeiling = (int)ceilf(y);

			// bilinear interpolation to grab surrounding pixel values
			// have to flip u and v for some reason [u][v] instead of [v][u]. investigate later
			if(x < child_width && y < child_width)
				scaledChild[u][v] = (unsigned char)(0.25 * (
										childA_imageData[xFloor][yFloor][0] + 
										childA_imageData[xFloor][yCeiling][0] + 
										childA_imageData[xCeiling][yFloor][0] + 
										childA_imageData[xCeiling][yCeiling][0]
										));
		}
	}
	// place extracted child image in hole starting at corner 1
	int iC = 0;
	int jC = 0;
	for (int i = holeCoordinates[0][1]; i < holeCoordinates[2][1]; i++)
	{
		for (int j = holeCoordinates[0][0]; j < holeCoordinates[2][0]; j++)
		{
			if (iC < hole_height && jC < hole_width)
			{
				parentImageData[i][j][0] = scaledChild[iC][jC];
				jC++;
				// printf("parentImageData[%d][%d][0] = %d\n", i, j, parentImageData[i][j][0]);
				// printf("childA_imageData[%d][%d][0] = %d\n", iC, jC, childA_imageData[iC][jC][0]);
			}
		}
		jC = 0;
		if (iC < hole_height)
			iC++;
	}
	
	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[3],"wb"))) 
	{
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}

	fwrite(parentImageData, sizeof(unsigned char), height*width, file);
	fclose(file);

	return 0;
}
