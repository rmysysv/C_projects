#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
@brief 

@param dizileri birleþtirme
@param dizi merge edilecek dizi
@param l birleþtirileeck sol alt dizinin baþlangýç adresi
@param m ortadaki indeks
@param r birleþtirileeck sað alt dizinin bitiþ adresi
@param orderType determines the type of ording, if 0 Ascending, if 1 Descending.
@return
*/
void merge(int dizi[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;


    int* L = (int*)malloc(n1 * sizeof(int));
    
	if (L == NULL) {
        printf("Bellek ayirma basarisiz oldu.\n");
        return NULL;
    }
    int* R = (int*)malloc(n2 * sizeof(int));
	
	if (R == NULL) {
        printf("Bellek ayirma basarisiz oldu.\n");
        return NULL;
    }
    

    for (i = 0; i < n1; i++)
        L[i] = dizi[l + i];
    for (j = 0; j < n2; j++)
        R[j] = dizi[m + 1 + j];

    i = 0,j = 0,k = l;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            dizi[k] = L[i];
            i++;
        }
        else {
            dizi[k] = R[j];
            j++;
        }
        k++;
    }


    while (i < n1) {
        dizi[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        dizi[k] = R[j];
        j++;
        k++;
    }
    
    free(L);
    free(R);
}
/*
@brief birleþtirilen elemanlarý sýralama
@param arr wdizinin kendisi
@param l sol indeks
@param r sað indeks

@return
*/

void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

/*
@brief diziyi yazdýrma
@param dizi yazdýrýlacak dizi
@param size dizinin boyutu

@return 
*/
void printArray(int dizi[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", dizi[i]);
    printf("\n");
}

/*
@brief sýralanmamýþ diziyi k parçaya bölerek bunun üzerinden mergesort iþlemi yapmak

@param dizi parçalara ayrýlacak dizi
@param size dizi boyutu
@param k diziyi kaç parçaya böleceðimizi veren parametre

@return
*/
void kWayMerge(int dizi[], int size, int k) {
    int partSize = size / k;
    int i,j,start,end;
    int minIndex,minValue;
    
    int* startIndex = (int*)malloc((k+1) * sizeof(int));
    
	if (startIndex == NULL) {
        printf("Bellek ayirma basarisiz oldu.\n");
        return NULL;
    }
    
    int* output = (int*)malloc(size * sizeof(int));
	if (output == NULL) {
        printf("Bellek ayirma basarisiz oldu.\n");
        return NULL;
    }

    for ( i = 0; i < k; i++) {
        start = i * partSize;
        end = (i == k - 1) ? size - 1 : (i + 1) * partSize - 1;
        mergeSort(dizi, start, end);
		startIndex[i] = start;
    }
    startIndex[k] = size;
    
    
    for (i = 0; i < size; i++) {
        minIndex = -1;
		minValue = INT_MAX;
        for (j = 0; j < k; j++) {
            if (startIndex[j] < startIndex[j + 1] && dizi[startIndex[j]] < minValue) {
                minIndex = j;
                minValue = dizi[startIndex[j]];
            }
        }
        output[i] = minValue;
        startIndex[minIndex]++;
    }

    for (i = 0; i < size; i++) {
        dizi[i] = output[i];
    }
    free(startIndex);
    free(output);
}
/*
@brief unique eleman oluþturma
@param size dizinin boyutu
@return oluþturulan arr
*/
int* shuffle(int size) {
	int i,j;
	
	int* arr = (int*)malloc(size * sizeof(int));
	if (arr == NULL) {
        printf("Bellek ayirma basarisiz oldu.\n");
        return NULL;
    }
    // Rastgele sayý üretimi için srand ile zaman baþlatýlýyor
    srand(time(NULL));
    for(i=0;i<size;i++){
    	arr[i]=i;
	}
	
    for ( i = size - 1; i > 0; i--) {
        // 0 ile i arasýnda rastgele bir indeks seçiyoruz
         j = rand() % (i + 1);

        // arr[i] ile arr[j] elemanlarýný takas ediyoruz
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    return arr;
}

// Driver code
int main(){
	int i,j,k;
	
	int k_count=9;
	int arr_size[6]={100,1000,10000,100000,1000000,10000000};
	int K[9]={2,3,4,5,6,7,8,9,10};
	int running=10;
	clock_t start, end;
    double cpu_time_used;

	srand(time(0));
	
	
	FILE *file = fopen("data10000000.csv", "a+"); 
    if (file == NULL) {
        printf("Dosya acilamadi.\n");
        return 1;
    }
    
	int arr_size_count=4;
    for (i=3;i<arr_size_count;i++){
    	fprintf(file, "N,k,running,time\n");
    	printf("dizi olustu\n");
		for(j=0;j<running;j++){
			int* arr=shuffle(arr_size[i]);
			for(k=0;k<k_count;k++){
				printf("%d----%d\n",j,k);
				start = clock();
			    kWayMerge(arr, arr_size[i], K[k]);
			    end = clock();
			    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
				fprintf(file, "%d,%d,%d,%.6f\n",arr_size[i],j,K[k],cpu_time_used);
				printf("The code executed in %.6f seconds.\n", cpu_time_used);
				start=0,end=0;
			}
			free(arr);
		}
	}
    fclose(file);
    return 0;
}
