//Name: Aaron Lim
//Student ID: 5985171
//Assn: 2 part 1

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	//defining openCL library
#define __CL_ENABLE_EXCEPTIONS				//enable openCL exceptions

////c++ standard libraries
#include <iostream>
#include <ctime>	//to set the time for random
#include <cstdlib>	//for srand and rand

//openCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

//include functions and errors handler
#include "common.h"

//fixed size for array1 and array2
const int ARRAY1 = 8;
const int ARRAY2 = 16;

//task 1
void createArray(int[], int);
void printArray(int[], int);

int main() {
	cl::Platform platform;	//platform for device
	cl::Device device;		//device used
	cl::Context context;	//context for device used
	cl::Program program;	//OpenCL program obj
	cl::Kernel kernel;		//Single kernel
	cl::CommandQueue queue;	//CommandQueue for context and device

	//cl obj
	//array 1 of random values from 0 ~ 9
	//array 2 of fixed values from 1 to 8 and -8 to -1
	cl_int array1[ARRAY1];
	cl_int array2[ARRAY2];

	//Declaration of buffers
	std::vector<cl_int> output(ARRAY1);
	cl::Buffer array1Buffer;
	cl::Buffer array2Buffer;
	cl::Buffer outputBuffer;

	//Initializing random seed
	srand(time(NULL));

	std::cout << "Assignment 2 part 1" << std::endl;
	std::cout << "===================" << std::endl;

	//creation of array1 and array2
	createArray(array1, ARRAY1);
	createArray(array2, ARRAY2);

	//printing array1 and array2
	std::cout << "Array 1: ";
	printArray(array1, ARRAY1);
	std::cout << "Array 2: ";
	printArray(array2, ARRAY2);
	std::cout << std::endl;

	try {
		//select one device
		if (!select_one_device(&platform, &device)) {
			//if no device selected
			quit_program("Device not selected");
		}

		//create a context from the device
		context = cl::Context(device);

		if (!build_program(&program, &context, "source.cl")) {
			//if openCL program build error
			quit_program("OpenCL program build error.");
		}

		//create a kernel
		kernel = cl::Kernel(program, "kernel1");

		//create command queue
		queue = cl::CommandQueue(context, device);

		//create buffers (local memory)
		array1Buffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * ARRAY1, &array1[0]);
		array2Buffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * ARRAY2, &array2[0]);
		outputBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_int) * ARRAY1);
		
		//enqueue command to write from host to device memory
		queue.enqueueWriteBuffer(outputBuffer, CL_TRUE, 0, sizeof(cl_int) * output.size(), &output[0]);
	
		//set kernel arguments
		kernel.setArg(0, array1Buffer);
		kernel.setArg(1, array2Buffer);
		kernel.setArg(2, outputBuffer);

		//enqueue kernel for execution
		queue.enqueueTask(kernel);

		std::cout << "Kernel enqueued" << std::endl;
		std::cout << "===================" << std::endl;

		//Get content for output array from kernel
		queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, sizeof(cl_int) * output.size(), &output[0]);

		//content from output
		for (int i = 0; i < ARRAY1; i++) {
			std::cout << i << ": " << output[i] << std::endl;
		}
	}
	catch (cl::Error e) {
		//call function to handle the errors
		handle_error(e);
	}

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
#endif

	return 0;
}

//task 1
void createArray(int array[], int size) {
	int randomize;
	if (size == 8) {
		for (int i = 0; i < size; i++) {
			randomize = rand() % 10;
			array[i] = randomize;
		}
	}
	else {
		int j = -8;
		for (int i = 0; i < 8; i++) {
			array[i] = i + 1;
		}
		for (int i = 8; i < size; i++) {
			array[i] = j;
			j++;
		}
	}
}

void printArray(int array[], int size) {
	for (unsigned int i = 0; i < size; i++) {
		std::cout << array[i] << " ";
	}
	std::cout << std::endl;
}