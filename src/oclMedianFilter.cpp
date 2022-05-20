//code

//Author: Christopher Hill For the EEE4120F course at UCT

#include<stdio.h>
#include<CL/cl.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cmath>
#include <tuple>
#include <vector>
#include <cstring>

using namespace std;
void displayImageInt(float *in, int cols, int rows)

{
	ofstream myFile("medianFilterOutput.pgm");
	myFile << "P2" << endl;
	myFile << "# written by group 15" << endl;
	myFile << cols << " " << rows << endl;
	myFile << "255" << endl;
	for ( int i = 0; i <rows; i++)

	{	

		for ( int j = 0; j < cols; j++)

		{

			printf("%.0f ", in[ i * cols + j ]);
			myFile << in[ i * cols + j ] << " ";

		}
		myFile << endl;
		printf("\n");

	}

	

	printf("\n");

}
int main(void)
{

	clock_t start, end;  //Timers


	int Size = 3;
	int filterSize = Size*Size;
	unsigned int resWidth;
	unsigned int resHeight;
	unsigned int maxValue;

	ifstream infile("sloan_image.pgm");
	stringstream ss;
	string inputLine = "";

	getline(infile, inputLine);
	if(inputLine.compare("P2") != 0) cerr << "Version error" << endl;
 	else cout << "Version : " << inputLine << endl;

	getline(infile,inputLine);
  	cout << "Comment : " << inputLine << endl;

	ss << infile.rdbuf();
	// Third line : size
	ss >> resWidth >> resHeight;
	cout << resWidth << " columns and " << resHeight << " rows" << endl;

	getline(infile,inputLine);
  	cout << "Max scale : " << inputLine << endl;

	float array[resHeight][resWidth];

	for(int row = 0; row < resHeight; ++row)
    for (int col = 0; col < resWidth; ++col) ss >> array[row][col];

	// Now print the array to see the result
	for(int row = 0; row < resHeight; ++row) {
		for(int col = 0; col < resWidth; ++col) {
		cout << array[row][col] << " ";
		}
		cout << endl;
	}
	infile.close();
	
	/* OpenCL structures you need to program*/
	//cl_device_id device; step 1 and 2 
	//cl_context context;  step 3
	//cl_program program;  steps 4,5 and 6
	//cl_kernel kernel; step 7
	//cl_command_queue queue; step 8
	
	//------------------------------------------------------------------------
	 
	 
	//Initialize Buffers, memory space the allows for communication between the host and the target device
	//TODO: initialize matrixA_buffer, matrixB_buffer and output_buffer
	cl_mem inImage_buffer, outImage_buffer, bufferFilter;

	//***step 1*** Get the platform you want to use
	//cl_int clGetPlatformIDs(cl_uint num_entries,
	//				cl_platform_id *platforms, 
	//				cl_uint *num_platforms)
  	
    	//------------------------------------------------------------------------
    
	cl_uint platformCount; //keeps track of the number of platforms you have installed on your device
	cl_platform_id *platforms;
	// get platform count
	clGetPlatformIDs(5, NULL, &platformCount); //sets platformCount to the number of platforms

	// get all platforms
	platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
	clGetPlatformIDs(platformCount, platforms, NULL); //saves a list of platforms in the platforms variable
    

	cl_platform_id platform = platforms[0]; //Select the platform you would like to use in this program (change index to do this). If you would like to see all available platforms run platform.cpp.
	
	
	//Outputs the information of the chosen platform
	char* Info = (char*)malloc(0x1000*sizeof(char));
	clGetPlatformInfo(platform, CL_PLATFORM_NAME      , 0x1000, Info, 0);
	printf("Name      : %s\n", Info);
	clGetPlatformInfo(platform, CL_PLATFORM_VENDOR    , 0x1000, Info, 0);
	printf("Vendor    : %s\n", Info);
	clGetPlatformInfo(platform, CL_PLATFORM_VERSION   , 0x1000, Info, 0);
	printf("Version   : %s\n", Info);
	clGetPlatformInfo(platform, CL_PLATFORM_PROFILE   , 0x1000, Info, 0);
	printf("Profile   : %s\n", Info);
	
	//------------------------------------------------------------------------

	//***step 2*** get device ID must first get platform
	//cl_int clGetDeviceIDs(cl_platform_id platform,
	//			cl_device_type device_type, 
	//			cl_uint num_entries, 
	//			cl_device_id *devices, 
	//			cl_uint *num_devices)
	
	cl_device_id device; //this is your deviceID
	cl_int err;
	
	/* Access a device */
	//The if statement checks to see if the chosen platform uses a GPU, if not it setups the device using the CPU
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if(err == CL_DEVICE_NOT_FOUND) {
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	}
	printf("Device ID = %i\n",err);
        printf("Dhinzalo\n");
	//------------------------------------------------------------------------
	
	//***Step 3*** creates a context that allows devices to send and receive kernels and transfer data
	//cl_context clCreateContext(cl_context_properties *properties,
	//				cl_uint num_devices,
	//				void *pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data),
	//				void *user_data,cl_int *errcode_ret)
    printf("Dean 1\n");
	cl_context context; //This is your contextID, the line below must just run
	printf("cl_context\n");
	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
	printf("context set\n");
	//------------------------------------------------------------------------

	//***Step 4*** get details about the kernel.cl file in order to create it (read the kernel.cl file and place it in a buffer)
	//read file in	
	FILE *program_handle;
	program_handle = fopen("Opencl/medianFilter.cl", "r");
	printf("program_handle\n");
	//get program size
	size_t program_size;//, log_size;
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	printf("rewind\n");

	//sort buffer out
	char *program_buffer;//, *program_log;
	program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);
	printf("fclose\n");
   	
	//------------------------------------------------------------------------

	//***Step 5*** create program from source because the kernel is in a separate file 'kernel.cl', therefore the compiler must run twice once on main and once on kernel
	//cl_program clCreateProgramWithSource (cl_context context,
	//						cl_uint count, 
	//						const char **strings, 
	//						const size_t *lengths, 
	//						cl_int *errcode_ret)	
	
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, NULL); //this compiles the kernels code
	printf("cl_program\n");
	//------------------------------------------------------------------------

	//***Step 6*** build the program, this compiles the source code from above for the devices that the code has to run on (ie GPU or CPU)
	//cl_int clBuildProgram(cl_program program,
	//		cl_uint num_devices,
	//		const cl_device_id* device_list,
	//		const char* options,
	//		void (CL_CALLBACK* pfn_notify)(cl_program program, void* user_data),
	//		void* user_data);
	
	cl_int err3= clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	printf("program ID = %i\n", err3);
	
	//------------------------------------------------------------------------

	//***Step 7*** creates the kernel, this creates a kernel from one of the functions in the cl_program you just built
	//cl_kernel clCreateKernel(cl_program program,
	//			const char* kernel_name,
	//			cl_int* errcode_ret);

	//TODO: select the kernel you are running
	cl_kernel kernel = clCreateKernel(program, "median_filter_kernel", &err);
	printf("cl_kernel %i\n", err);
	//------------------------------------------------------------------------
	
	//***Step 8*** create command queue to the target device. This is the queue that the kernels get dispatched too, to get the the desired device.
	//cl_command_queue clCreateCommandQueue(cl_context context,
	//						cl_device_id device, 
	//						cl_command_queue_properties properties,
	//						cl_int *errcode_ret)
	
	start = clock();
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, NULL);
	printf("cl_command\n");

	//------------------------------------------------------------------------

	//***Step 9*** create data buffers for memory management between the host and the target device
	//TODO: set global_size, local_size and num_groups, in order to control the number of work item in each work group
	
	size_t global_size[2] = {resWidth, resHeight}; //total number of work items
	size_t local_size = resWidth; //Size of each work group
	size_t im_width = resWidth;
	size_t im_height = resHeight;
	cl_int num_groups = resHeight; //number of work groups needed
	cl_int windowSize = Size;
	printf("cl_int\n");
	//cl_int numElem = Size*Size;

	//already got matrixA and matrixB
	//TODO: initialize the output array
   	//int output[global_size]; //output array
	//float out_image[resWidth][resHeight];
	printf("out_image\n");
        // *out_image;
	float *data = (float *)malloc(resWidth*resHeight*sizeof(float));
    int sizeInBytes = resWidth*resHeight*sizeof(float);
    float *out_image = (float*)malloc( sizeInBytes);
        //if(!out_image) throw_error();
	printf("mkoo\n");
	
	//Buffer (memory block) that both the host and target device can access 
	//cl_mem clCreateBuffer(cl_context context,
	//			cl_mem_flags flags,
	//			size_t size,
	//			void* host_ptr,
	//			cl_int* errcode_ret);
	
	//TODO: Allocate OpenCl imge memory buffer
	static const cl_image_format format = { CL_RGBA, CL_UNSIGNED_INT8};
        cl_image_desc image_desc;
        image_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
        image_desc.image_width = im_width;
        image_desc.image_height = im_height;
		image_desc.image_depth = 1;
        image_desc.image_array_size = 1;
        image_desc.image_row_pitch = 0;
        image_desc.image_slice_pitch = 0;
        image_desc.num_mip_levels = 0;
        image_desc.num_samples = 0;
        image_desc.buffer = NULL;
	printf("image_desc\n");
	//outImage_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(matrixB), &matrixB, &err);
	memcpy(data, array, resWidth*resHeight*sizeof(float));
	inImage_buffer = clCreateImage(context,CL_MEM_READ_ONLY| CL_MEM_COPY_HOST_PTR,&format,&image_desc,data, &err); // could not put host pointer
	printf("input \n");
	outImage_buffer = clCreateImage(context, CL_MEM_READ_WRITE| CL_MEM_COPY_HOST_PTR,&format,&image_desc,out_image, &err);
    printf("outimage\n");
	    //outImage_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,sizeInBytes,out_image, &err);
	//bufferFilter = clCreateBuffer(context, 0, filterSize*sizeof(float), NULL, NULL);
		size_t origin[3] = {0, 0, 0};
        size_t region[3] = {resWidth, resHeight, 1};
        clEnqueueWriteImage(queue, inImage_buffer, CL_FALSE, origin, region, 0, 0, array, 0, NULL, NULL);
        //clEnqueueWriteBuffer(queue, bufferFilter, CL_FALSE, 0, filterSize*sizeof(float), &inImage_buffer, 0, NULL, NULL);
	//------------------------------------------------------------------------

	//***Step 10*** create the arguments for the kernel (link these to the buffers set above, using the pointers for the respective buffers)
	// cl_int clSetKernelArg (cl_kernel kernel, 
	//				cl_uint arg_index, 
	//				const void *arg_value)
	
	//TODO: create the arguments for the kernel. Note you can create a local buffer only on the GPU as follows: clSetKernelArg(kernel, argNum, size, NULL);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &inImage_buffer );
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &outImage_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_int), &windowSize);
	//clSetKernelArg(kernel, 3, sizeof(cl_int), &widthA);
	//------------------------------------------------------------------------

	
	

	//***Step 11*** enqueue kernel, deploys the kernels and determines the number of work-items that should be generated to execute the kernel (global_size) and the number of work-items in each work-group (local_size).
	
	// cl_int clEnqueueNDRangeKernel (cl_command_queue command_queue, 
	//					cl_kernel kernel, 
	//					cl_uint work_dim, 
	//					const size_t *global_work_offset, 
	//					const size_t *global_work_size, 
	//					const size_t *local_work_size, 
	//					cl_uint num_events_in_wait_list, 
	//					const cl_event *event_wait_list, 
	//					cl_event *event)
	
	//end = clock(); //data transfer overhead
	//start = clock();  //data processing 
	cl_int err4 = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, NULL, 0, NULL, NULL); 
	//system("nvidia-smi");
	

	printf("\nKernel check: %i \n",err4);

	//------------------------------------------------------------------------
	//***Step 12*** Allows the host to read from the buffer object 
	err = clEnqueueReadImage(queue,outImage_buffer, CL_TRUE, origin, region, 0, 0, out_image, 0, NULL, NULL);
    //err = clEnqueueReadBuffer(queue, outImage_buffer, CL_TRUE, 0,sizeInBytes,out_image, 0, NULL, NULL);
        //void (*map_ptr)[2] = (float (*)[2]) out_image;
	printf("clEnqueue = %i\n", err);
	printf("clFinish\n");
	end = clock();
	printf("Run Time: %0.8f sec \n",((float) end - start)/CLOCKS_PER_SEC);
	//for(int row = 0; row < resWidth; ++row) {
	//	for(int col = 0; col < resHeight; ++col) {
                //out_image[row][col]=col;
		//cout << out_image[row][col] << " ";
          //      cout << out_image;
	//	}
	//	cout << endl;
//	}

	/*if(displayMatrices){
		printf("\nOutput in the output_buffer \n");
		for(int j=0; j<countA; j++) {
			if(j%Size == (Size-1)){
				printf("\n");
			}
		}
	}*/
	displayImageInt( out_image,resWidth,resHeight);
	
	//------------------------------------------------------------------------

	//***Step 14*** Deallocate resources	
	clReleaseKernel(kernel);
	clReleaseMemObject(outImage_buffer);
	clReleaseMemObject(inImage_buffer);
	//clReleaseMemObject(matrixB_buffer);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	return 0;
}


