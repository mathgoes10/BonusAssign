//Matheus Goes Lima
//Bonus Programming Assignment
//Computer Science 1 COP3502 Section 3

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	//printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	//printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	// Build heap (rearrange array)
      for (int i = n / 2 - 1; i >= 0; i--) {
          int largest = i; // Initialize largest as root
          int left = 2 * i + 1; // left = 2*i + 1
          int right = 2 * i + 2; // right = 2*i + 2

          // If left child is larger than root
          if (left < n && arr[left] > arr[largest])
              largest = left;

          // If right child is larger than largest so far
          if (right < n &&
            arr[right] > arr[largest])
              largest = right;

          // If largest is not root
          if (largest != i) {
              // Swap arr[i] and arr[largest]
              int temp = arr[i];
              arr[i] = arr[largest];
              arr[largest] = temp;

              // Recursively heapify the affected sub-tree
              // Note: We don't need a separate heapify function
              i = largest;
              // Continue heapifying until we reach the leaf node
              while (2 * i + 1 < n) {
                  int leftChild = 2 * i + 1;
                  int rightChild = 2 * i + 2;
                  largest = i;
                  if (arr[leftChild] > arr[largest])
                      largest = leftChild;
                  if (rightChild < n && arr[rightChild] > arr[largest])
                      largest = rightChild;

                  if (largest == i)
                      break;

                  // Swap arr[i] and arr[largest]
                  temp = arr[i];
                  arr[i] = arr[largest];
                  arr[largest] = temp;

                  // Move to the next level
                  i = largest;
              }
          }
      }

      // One by one extract an element from heap
      for (int i = n - 1; i > 0; i--) {
          // Move current root to end
          int temp = arr[0];
          arr[0] = arr[i];
          arr[i] = temp;

          // Heapify the reduced heap
          int j = 0;
          while (2 * j + 1 < i) {
              int leftChild = 2 * j + 1;
              int rightChild = 2 * j + 2;
              int largest = j;
              if (arr[leftChild] > arr[largest])
                  largest = leftChild;
              if (rightChild < i && arr[rightChild] > arr[largest])
                  largest = rightChild;

              if (largest == j)
                  break;

              // Swap arr[j] and arr[largest]
              temp = arr[j];
              arr[j] = arr[largest];
              arr[largest] = temp;

              // Move to the next level
              j = largest;
          }
      }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
if (l < r) {
    // get the mid point
    int m = (l + r) / 2;
    // Sort first and second halves
    mergeSort(pData, l, m);
    mergeSort(pData, m + 1, r);
    // printf("Testing l=%d r=%d m=%d\n", l, r, m);
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    /* create temp arrays */
    int *L = (int *)Alloc(n1 * sizeof(int));
    int *R = (int *)Alloc(n2 * sizeof(int));
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
      L[i] = pData[l + i];
    for (j = 0; j < n2; j++)
      R[j] = pData[m + 1 + j];
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
      if (L[i] <= R[j]) {
        pData[k] = L[i];
        i++;
      } else {
        pData[k] = R[j];
        j++;
      }
      k++;
    }
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
      pData[k] = L[i];
      i++;
      k++;
    }
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
      pData[k] = R[j];
      j++;
      k++;
    }
    DeAlloc(L);
    DeAlloc(R);
  }
}
void insertionSort(int* pData, int n)
{
	for (int i = 1; i < n; i++)
    {
        int key = pData[i];
        int j = i - 1;
        while (j >= 0 && pData[j] > key)
        {
            pData[j + 1] = pData[j];
            j = j - 1;
        }
        pData[j + 1] = key;
    }
}
// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (pData[j] > pData[j + 1])
                swap(&pData[j], &pData[j + 1]);
        }
    }
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	for (int i = 0; i < n - 1; i++)
    {
        int min_index = i;
        for (int j = i + 1; j < n; j++)
        {
            if (pData[j] < pData[min_index])
                min_index = j;
        }
        swap(&pData[min_index], &pData[i]);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (int i=0; i<dataSz; ++i)
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
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
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

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}