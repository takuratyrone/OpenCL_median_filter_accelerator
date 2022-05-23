//TODO: set your arguments for the kernel. Note that you have to indicate if the argument is global or local. Global arguments are accessable by both host and this target device. While local can only be accessed by the device running this kernel. eg __global int* inputMatrixA, __local int* groupMemory

__kernel void matrixMultiplication(__global int* matrixA, __global int* matrixB, __global int* output, int widthA){
	
	//TODO: program your kernel here
	int workItemNum = get_global_id(0); //Work item ID
	int workGroupNum = get_group_id(0); //Work group ID
	int localGroupID = get_local_id(0); //Work items ID within each work group
	//int workIremNum = get_local_id(0);

	//memory buffers
	int arg1 = *matrixA;
	int arg2 = *matrixB;
	uint global_addr = workItemNum;
	
	int row = workGroupNum;
	int col = global_addr % widthA;
	//printf("Hello World\n");
	//printf("Length: %d", arg1[1]);
	int matA = matrixA[workItemNum];
	int matB = matrixB[workItemNum];
	//output[global_addr] = arg1[2] + arg2[2];
	int sum = 0;
	for (int i = 0; i <  widthA; i++) {
		sum +=  matrixA[(row*widthA)+i] * matrixB[(widthA*i)+col];
	}
	output[global_addr] = sum;
	//printf("Hi from work item: %d	work group:%d	Arg1: %d	Arg2: %d	Output: %d\n",global_addr, workGroupNum, matA, matB, output[global_addr]);
	
}




