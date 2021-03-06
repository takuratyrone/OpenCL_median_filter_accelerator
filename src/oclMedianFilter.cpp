
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

void displayImageInt(int* in, int cols, int rows)

{
	ofstream myFile("Medianfilter.pgm");
	myFile << "P2" << endl;
	myFile << "# written by group 15" << endl;
	myFile << cols << " " << rows << endl;
	myFile << "255" << endl;
	for ( int i = 0; i <rows; i++)

	{	

		for ( int j = 0; j < cols; j++)

		{

			printf("%i ", in[ i * cols + j ]);
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

	start = clock();
	int Size = 3;
	int filterSize = Size*Size;
	unsigned int resWidth;
	unsigned int resHeight;
	unsigned int maxValue;

	ifstream infile("glassware_noisy.ascii.pgm");
	stringstream ss;
	string inputLine = "";

	getline(infile, inputLine);
	if(inputLine.compare("P2") != 0) cerr << "Version error" << endl;
 	else cout << "Version : " << inputLine << endl;

	getline(infile,inputLine);
  	cout << "Comment : " << inputLine << endl;

	ss << infile.rdbuf();
	// Third line : size
	ss >> resWidth >> resHeight >> maxValue;
	cout << resWidth << " columns and " << resHeight << " rows " << maxValue << " max value " << endl;

	int count = resWidth*resHeight;

	int array[count];

	//for(int row = 0; row < resHeight; ++row)
    for (int col = 0; col < (resWidth*resHeight); col++) ss >> array[col];

	// Now print the array to see the result
	/*for(int row = 0; row < resHeight; ++row) {
		for(int col = 0; col < resWidth; col++) {
			cout << array[row*resWidth + col] << " ";
		}
		cout << endl;
	}*/
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
	cl_mem inImage_buffer, outImage_buffer;

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

	//------------------------------------------------------------------------
	
	//***Step 3*** creates a context that allows devices to send and receive kernels and transfer data
	//cl_context clCreateContext(cl_context_properties *properties,
	//				cl_uint num_devices,
	//				const cl_device_id *devices,
	//				void *pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data),
	//				void *user_data,cl_int *errcode_ret)
	cl_context context; //This is your contextID, the line below must just run
	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

	//------------------------------------------------------------------------

	//***Step 4*** get details about the kernel.cl file in order to create it (read the kernel.cl file and place it in a buffer)
	//read file in	
	FILE *program_handle;
	program_handle = fopen("Opencl/medianFilter.cl", "r");

	//get program size
	size_t program_size;//, log_size;
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);

	//sort buffer out
	char *program_buffer;//, *program_log;
	program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);
   	
	//------------------------------------------------------------------------

	//***Step 5*** create program from source because the kernel is in a separate file 'kernel.cl', therefore the compiler must run twice once on main and once on kernel
	//cl_program clCreateProgramWithSource (cl_context context,
	//						cl_uint count, 
	//						const char **strings, 
	//						const size_t *lengths, 
	//						cl_int *errcode_ret)	
	
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, NULL); //this compiles the kernels code

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
	cl_kernel kernel = clCreateKernel(program, "median_filter", &err);
	//------------------------------------------------------------------------
	
	//***Step 8*** create command queue to the target device. This is the queue that the kernels get dispatched too, to get the the desired device.
	//cl_command_queue clCreateCommandQueue(cl_context context,
	//						cl_device_id device, 
	//						cl_command_queue_properties properties,
	//						cl_int *errcode_ret)
	
	//start = clock();
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, NULL);

	//------------------------------------------------------------------------

	//***Step 9*** create data buffers for memory management between the host and the target device
	//TODO: set global_size, local_size and num_groups, in order to control the number of work item in each work group
	
	size_t global_size = count; //total number of work items
	size_t local_size = resWidth; //Size of each work group
	cl_int num_groups = global_size/local_size; //number of work groups needed
	cl_int im_width = resWidth;
	cl_int im_height = resHeight;

	//already got matrixA and matrixB
	//TODO: initialize the output array

   	int output[global_size]; //output array

	
	//Buffer (memory block) that both the host and target device can access 
	//cl_mem clCreateBuffer(cl_context context,
	//			cl_mem_flags flags,
	//			size_t size,
	//			void* host_ptr,
	//			cl_int* errcode_ret);
	
	//TODO: create matrixA_buffer, matrixB_buffer and output_buffer, with clCreateBuffer()

	inImage_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(array), &array, &err);
		
	outImage_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(count), output, &err);

	//------------------------------------------------------------------------

	//***Step 10*** create the arguments for the kernel (link these to the buffers set above, using the pointers for the respective buffers)
	// cl_int clSetKernelArg (cl_kernel kernel, 
	//				cl_uint arg_index, 
	//				size_t arg_size, 
	//				const void *arg_value)
	
	//TODO: create the arguments for the kernel. Note you can create a local buffer only on the GPU as follows: clSetKernelArg(kernel, argNum, size, NULL);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &inImage_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &outImage_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_int), &im_width);
	clSetKernelArg(kernel, 3, sizeof(cl_int), &im_height);
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
	
	end = clock(); //data transfer overhead
	//start = clock();  //data processing 
	cl_int err4 = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL); 
	//system("nvidia-smi");
	

	printf("\nKernel check: %i \n",err4);

	//------------------------------------------------------------------------

	//***Step 12*** Allows the host to read from the buffer object 
	err = clEnqueueReadBuffer(queue, outImage_buffer, CL_TRUE, 0, sizeof(output), output, 0, NULL, NULL);
	
	
	//This command stops the program here until everything in the queue has been run
	clFinish(queue);
	//system("nvidia-smi");
	//end = clock();
	
	//***Step 13*** Check that the host was able to retrieve the output data from the output buffer
	//system("ls");
	printf ("Run Time: %0.8f sec \n",((float) end - start)/CLOCKS_PER_SEC);
	/*if(displayMatrices){
		printf("\nOutput in the output_buffer \n");
		for(int j=0; j<count; j++) {
			printf("%i " ,output[j]);
			if(j%resWidth == (resWidth-1)){
				printf("\n");
			}
		}
	}*/
	
	//displayImageInt( output,resWidth,resHeight);
	
	//------------------------------------------------------------------------

	//***Step 14*** Deallocate resources	
	clReleaseKernel(kernel);
	clReleaseMemObject(inImage_buffer);
	clReleaseMemObject(outImage_buffer);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	return 0;
}
