# Group 15 OPENCL YODA

## Directory Setup

The OpenCL source files are in src/.

src/images/ containt the P5 version of the medical images which need to be converted to P2.

src/Opencl/ contains the OpenCL kernels

src/output/ contains the median filtered and edge detected images

## Code

### oclMedianFilter.cpp
The following command compiles the script that sets up and executes the OpenCL medianFilter.cl kernel
```bash
g++ oclMedianFilter.cpp -lOpenCL
```

The following command runs the executable

```bash
./a.out
```

### oclEdgeDetection.cpp
The following command compiles the script that sets up and executes the OpenCL sobelEdge.cl kernel
```bash
g++ oclEdgeDetection.cpp -lOpenCL
```

### yodapre.cpp
This script converts P5 to P2 pgm

### golden.cpp
This script implements the median filter golden standard

