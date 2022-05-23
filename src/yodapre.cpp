#include <iostream>
#include <string>
#include <fstream>
#include <ios>
#include <sstream>
#include <cstring>
#include <vector>
using namespace std;


std::string inputPGMFile = "sloan.pgm";
	int largeWidth, largeHeight;

	//read the input image dimensions
	std::string inputLine;
	std::ifstream imageFile;
	imageFile.open(inputPGMFile, std::ios::binary | std::ios::in);

	getline(imageFile,inputLine); //P5
	getline(imageFile,inputLine);

	while(inputLine[0]=='#')
	{
		 getline(imageFile,inputLine);  //comments ;;;;iterate for more comments
	}
	//getline(imageFile,inputLine); //width height
	std::istringstream iss( inputLine );
	iss >> largeWidth;
	iss >> largeHeight;
	getline(imageFile,inputLine); //ecocode
	getline(imageFile,inputLine); //blank line

	std::vector<unsigned char **> imageSequence;

int x1 =0;
int y1 = 0;
int x2 = 500;
int y2=500;
int width = 500;
int height = 500;

imageSequence = extract(x1,y1,x2, y2, width,largeWidth, largeHeight, height,imageFile )

write(imageSequence,operation,seq_name, width,height);

unsigned char** extract(int x1,int y1,int x2, int y2, int width,int largeWidth, int largeHeight, int height,std::ifstream& imageFile ) //returns a 2D array of a single frame
{
	unsigned char pixel;
	unsigned char ** pixels = new unsigned char*[height]; //dynamic 2D array to store a frame
	for (int i =0;i<height;i++)
	{
		pixels[i]= new unsigned char[width];
	}
	//move read pointer to the begin coordinates.
	imageFile.seekg((largeWidth*y1 + x1), std::ios::beg);
	for (int i =0; i<height; i++)
	{
		for (int j =0;j<width;j++)
		{
			imageFile.read((char *) &pixel, 1);
			pixels[i][j] = pixel;
		}

		//skipping pixels, to jump to the next line of pixels 
		imageFile.seekg(largeWidth-width, std::ios::cur);
		/*for (int k =0;k<(largeWidth-width);k++)
                {
                        iss.get(pixel);
                }*/
	}
	return pixels;
}

void write(std::vector<unsigned char **> imageSequence, std::string operation, std::string seq_name, int width, int height)
{
		std::cout<<"none operation"<<std::endl;
		for(int k =0;k<imageSequence.size();k++)
		{	std::string zita ="";
			zita = seq_name+std::to_string(k)+".pgm";
			std::ofstream creatMyFile(zita, std::ios::out | std::ios::binary); //creation of a single file name "name_xxxx" the frame as pgm file.
			//writing the comments and pgm description;

			creatMyFile<<"P5"<<"\n#Generated by SMKKUD001"<<"\n"<<width <<" "<<height<<"\n255\n"<<"\n";
			for (int i =0; i<height;i++ )
			{
				for(int j=0;j<width;j++)
				{
					creatMyFile.write((char *) &(imageSequence[k][i][j]), 1);
				}
			}
			//imageSequence[k] [] delete;
			creatMyFile.close();//done writting to file
		}	
}