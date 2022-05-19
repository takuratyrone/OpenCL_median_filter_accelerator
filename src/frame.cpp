#include <iostream>
#include <string>
#include <fstream>
#include <ios>
#include <sstream>
#include <cstring>
#include <vector>
#include "MyHeader"

/*class FrameSequence
{
	public:
	unsigned char ** frame ; //instance of the class
	FrameSequence (unsigned char ** array2D ){frame = array2D;}//constructor
};*/

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
	if("none"== operation)
	{	std::cout<<"none operation"<<std::endl;
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

	if(operation=="invert")
        {	std::cout<<"invert operation"<<std::endl;
		for(int k =0;k<imageSequence.size();k++)
                {       std::string zita ="";
                        zita = seq_name+std::to_string(k)+".pgm";
                        std::ofstream creatMyFile(zita, std::ios::out | std::ios::binary); //creation of a single file name "name_xx>                        //writing the comments and pgm description;

                        creatMyFile<<"P5"<<"\n#Generated by SMKKUD001"<<"\n"<<width <<" "<<height<<"\n255\n"<<"\n";
                        for (int i =0; i<height;i++ )
                        {
                                for(int j=0;j<width;j++)
                                {
                                        char number = imageSequence[k][i][j];
					int ascii= 255-int(number);
					creatMyFile<<char(ascii);
                                }
                        }
                        //imageSequence[k] [] delete;
                        creatMyFile.close();//done writting to file
                }
        }

 	if(operation=="reverse")
        {	 std::cout<<"reverse operation"<<std::endl;
		int i = 0;
		for (std::vector<unsigned char **>::reverse_iterator k = imageSequence.rbegin(); k != imageSequence.rend(); ++k ) 
		{
			std::string zita ="";
                        zita = seq_name+std::to_string(i++)+".pgm";
                        std::ofstream creatMyFile(zita, std::ios::out | std::ios::binary); //creation of a single file name "name_xx>

                        creatMyFile<<"P5"<<"\n#Generated by SMKKUD001"<<"\n"<<width <<" "<<height<<"\n255\n"<<"\n";
                        for (int i =0; i<height;i++ )
                        {
                                for(int j=0;j<width;j++)
                                {
                                        creatMyFile<< (*k)[i][j];
                                }
                        }
                        //imageSequence[k] [] delete;
                        creatMyFile.close();//done writting to file
                }
        }



	if(operation=="revinvert")
        {	std::cout<<"revinvert operation"<<std::endl;
		int i = 0;
                for (std::vector<unsigned char **>::reverse_iterator k = imageSequence.rbegin(); k != imageSequence.rend(); ++k )    
                {
                        std::string zita ="";
                        zita = seq_name+std::to_string(i++)+".pgm";
                        std::ofstream creatMyFile(zita, std::ios::out | std::ios::binary); //creation of a single file name "name_xx>

                        creatMyFile<<"P5"<<"\n#Generated by SMKKUD001"<<"\n"<<width <<" "<<height<<"\n255\n"<<"\n";
                        for (int i =0; i<height;i++ )
                        {
                                for(int j=0;j<width;j++)
                                {
					char number = (*k)[i][j];
                                        int ascii= 255-int(number);
                                        creatMyFile<<char(ascii);
                                }
                        }
                        //imageSequence[k] [] delete;
                        creatMyFile.close();//done writting to file
                }
        }

}

std::vector<unsigned char **> twoInputs(int** coordinates, int width, int largeWidth, int largeHeight, int height, std::ifstream& imageFile)
{
	unsigned char ** pixels;
	std::vector<unsigned char **> imageSequence;
       	int x1 = coordinates[0][0];
       	int y1 = coordinates[0][1];
	int x2 = coordinates[1][0];
       	int y2 = coordinates[1][1];

        int n=1;//setting default framerate
	int dx, dy;
	std::cout<<"Enter step Pixel(Frame Rate)"<<std::endl;
	std::cin>>n;

	if(n< 0) {n = 1;}//default step of i pixel

	float gradient = (float)(y2-y1)/(x2-x1+0.0001) + 0.00001; //the offsets added when gradient is  0 it prevents a runtime error
	if (gradient < 1){ dx=1*n; dy = gradient*n;}
	else{dy = 1*n;  dx = (1/gradient)*n;}
//	std::cout<<gradient<<" "<<std::endl;
       	while (x1<x2 or y1<y2) //reading frames into the vector
       	{
//		std::cout<<x1<<"  x1 y1 "<<y1<<std::endl;
//		std::cout<<dx<<" dx dy  "<<dy<<std::endl;

               	pixels = extract(x1, y1, x2, y2, width, largeWidth, largeHeight, height,imageFile ); 
               	imageSequence.push_back(pixels); //storing frames in the vector imageSequence
               	x1=x1+dx;
               	y1=y1+dy;
       	}
	return imageSequence;
}

std::vector<unsigned char** > polyLine(int** coordinates, int n_coordinates, int width, int largeWidth, int largeHeight, int height, std::ifstream& imageFile)
{
	unsigned char ** pixels;
	std::vector<unsigned char **> imageSequence;
       	int x,y;
       	for(int f = 0; f< n_coordinates; f++)
       	{
               	x = coordinates[f][0];
               	y = coordinates[f][1];
               	pixels = extract(x, y, 0, 0, width, largeWidth, largeHeight, height,imageFile ); 
               	imageSequence.push_back(pixels); //storing frames in the vector imageSequence
       	}
	return imageSequence;
}
