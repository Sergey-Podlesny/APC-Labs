#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define iterCount 500000
#define arrSize 1000
#define num 345

void cWithOptimize(int* arr1, int* arr2)
{
	clock_t start, end;
	double time;
	int arr3[arrSize], j = 0;


	start = clock();
	for (int j = 0; j < iterCount; j++)
	{
		for (int i = 0; i < arrSize; i++)
		{
			arr3[i] = (arr1[i] > arr2[i]) ? arr1[i] : arr2[i];
		}
	}
	end = clock();
	time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("With optimize C: %lf\n", time);
	printf("arr3[%i] = %i\n", num, arr3[num]);
}


#pragma optimize("", off)
void cWithoutOptimize(int* arr1, int* arr2)
{
	clock_t start, end;
	double time;
	int arr3[arrSize], j = 0;


	start = clock();
	for (int j = 0; j < iterCount; j++)
	{
		for (int i = 0; i < arrSize; i++)
		{
			arr3[i] = (arr1[i] > arr2[i]) ? arr1[i] : arr2[i];
		}
	}
	end = clock();
	time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Without optimize C: %lf\n", time);
	printf("arr3[%i] = %i\n", num, arr3[num]);
}
#pragma optimize("", on)



int main()
{
	srand(time(NULL));
	clock_t start, end;
	double time;
	int arr1[arrSize], arr2[arrSize], arr3[arrSize], j = 0;
	for (int i = 0; i < arrSize; i++)
	{
		arr1[i] = rand() % 100;
		arr2[i] = rand() % 100;			
	}

	cWithOptimize(arr1, arr2);
	cWithoutOptimize(arr1, arr2);

	start = clock();
	while (j < iterCount) {
		_asm {
			lea esi, arr1
			lea edi, arr2
			mov ecx, arrSize
			mov ebx, 0
			l1 :
			mov eax, [esi + ebx * 4]
				mov edx, [edi + ebx * 4]
				cmp eax, edx
				jge l2
				push esi
				lea esi, arr3
				mov[esi + ebx * 4], edx
				pop esi
				inc ebx
				loop l1
				jmp Exit
				l2 :
			push esi
				lea esi, arr3
				mov[esi + ebx * 4], eax
				pop esi
				inc ebx
				loop l1
				Exit :
		}
		j++;
	}
	end = clock();
	time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Assembly: %lf\n", time);
	printf("arr3[%i] = %i\n", num, arr3[num]);
	start = clock();
	_asm
	{
		mov ecx, iterCount
		mov edi, arrSize * 4
	for11:
		xor esi, esi
	for22:
		movq mm0, arr1[esi]
		pmaxub mm0, arr2[esi]
		movq arr3[esi], mm0
		add esi, 8
		cmp esi, edi
		jb for22
		loop for11
		emms
	}
	end = clock();
	time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("MMX: %lf\n", time);
	printf("arr3[%i] = %i\n", num, arr3[num]);
	return 0;
}