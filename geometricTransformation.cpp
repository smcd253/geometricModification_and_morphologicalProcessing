// input: ./shrinking HW3_images/lighthouse.raw HW3_images/lighthouse1.raw HW3_images/lighthouseCombined.raw

#include "geometricModification.hpp"

using namespace std;

void findCornerCoords(unsigned char ***sourceImageData, unsigned char **destImageData, int height, int width, int startingCorner)
{
	switch(startingCorner)
	{
		// start at corner 1 of image to find upper left hand corner of piece
		case 1:
			for (int j = 0; j < width; j++)
			{
				for (int i = 0; i < height; i++)
				{
					while (*(((unsigned char *)sourceImageData + i * width) + j) == 255)
					{

					}
				}
			}
		break;
		
		// start at corner 2 of image to find lower left hand corner of piece
		case 2:
		break;
		
		// start at corner 3 of image to find lower right hand corner of piece		
		case 3:
		break;
		
		// start at corner 4 of image to find upper right hand corner of piece	
		case 4:
		break;
	}
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
	// temp variables for true image within child image array
	int trueChildAHeight, trueChildaWidth;
	unsigned char **trueChildAImage; // allocate memory while detecting image dynamically
	
	// helper function to search for true child image in original child image (store in trueChildImage, modify dynamically)

	// temp variables for hole A in parent image within parent image array
	int holeA_height, holeA_width;
	int holeA_origin[2] = {0};

	// helper function to find hole A in parent image
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			
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
