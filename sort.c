#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void swap(int arr[], int n1, int n2){
	int temp = arr[n1];
	arr[n1] = arr[n2];
  	arr[n2] = temp;
}

void heapSort(int arr[], int n)
{
	if(n == 0)
		return;
	for(int i = n/2-1; i >= 0;i--){
		int left = 2*i+1;
		int right = left+1;
		if(right < n && arr[i] < arr[right])
			swap(arr, i, right);
		if(left < n && arr[i] < arr[left])
			swap(arr, i, left);
	}
	swap(arr, 0, n-1);
	heapSort(arr, n-1);
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int arr[], int low, int mid, int high){
	int len1 = mid-low;
	int len2 = high-mid+1;
	int * tarr1 = (int*)malloc(sizeof(int)*len1);
	int * tarr2 = (int*)malloc(sizeof(int)*len2);
	extraMemoryAllocated += sizeof(int)*(len1+len2);
	for(int w = 0; w < len1; w++)
		tarr1[w] = arr[low+w];
	for(int w = 0; w < len2; w++)
		tarr2[w] = arr[mid+w];
	int i = 0;
	int t1 = 0;
  int t2 = 0;
	while(t1 < len1 && t2 < len2){
		if(tarr1[t1] > tarr2[t2]){
			arr[low+i] = tarr2[t2];
			t2++;
		}
		else{
			arr[low+i] = tarr1[t1];
			t1++;
		}
		i++;
	}
	while(t1 < len1){
		arr[low+i] = tarr1[t1];
		t1++;
		i++;
	}
	while(t2 < len2){
		arr[low+i] = tarr2[t2];
		t2++;
		i++;
	}
	free(tarr1);
	free(tarr2);
}

void mergeSort(int pData[], int l, int r)
{
	if(l >= r)
		return;
	int mid = (l+r)/2;
	mergeSort(pData, l, mid);
	mergeSort(pData, mid+1, r);
	merge(pData, l, mid+1, r);
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.3lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}