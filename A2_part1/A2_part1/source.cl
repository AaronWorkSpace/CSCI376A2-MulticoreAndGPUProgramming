__kernel void kernel1(__global int *array1,		//index 0
						__global int *array2,	//index 1
						__global int *output)	//index 2
{
	//const array size
	const int size = 8;
	int index = get_global_id(0);

	//copy content from array1 into int8 vector
	int8 v = *array1;

	//copy array2 to v1 and v2
	int8 v1 = vload8(index, array2);
	int8 v2 = vload8(index, array2+8);

	//result creation
	int8 result;

	//store the variable of v1 and v2
	int8 temp;

	//check if any value in v is greater than 5;
	bool ok = true;

	//check if array[i] is greater than 5
	if(all(v > 5)){
		ok = false;
	}

	if(!ok){
		for(int i = 0; i < size; i++){
			//copy from v1 if greater than or equal to 5
			if(v[i] >= 5){
				temp[i] = 0;
				result = select(v1, v2, temp);
			}
			//copy from v2 if smaller than 5
			else{
				temp[i] = -1;
				result = select(v1, v2, temp);
			}
		}
	}
	else{
		for(int i = 0; i < 4; i++){
			//copy from v1 first half
			temp[i] = 0;
			result = select(v1, v2, temp);
			
		}
		for(int i = 4; i < size; i++){
			//copy from v2 second half
			temp[i] = -1;
			result = select(v1, v2, temp);
		}

		//result = select(v1, v2, temp);
	}

	//vstore the data in output
	vstore8(result, 0, output);
}