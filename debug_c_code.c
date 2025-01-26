#include <stdio.h>
#include <string.h>
#include <stdlib.h>


enum mod{
	DEBUG=0,
	NORMAL=1,
};

/*
@brief çakýþma olmamasý durumunda kullanýlan hash fonksiyonu
@param value deðiþken isminin horner karþýlýðý
@param m hash tablosunun uzunluðu
@return hash tablosunda yerleþtirileceði indis
*/
int hashFunction1(int value,int m ){
	return value%m;
}
/*
@brief çakýþma olmasý durumunda 1.hash ile birlikte kullanýlan hash fonksiyonu
@param value deðiþken isminin horner karþýlýðý
@param m hash tablosunun uzunluðu
@return hash tablosunda yerleþtirileceði indis
*/
int hashFunction2(int value,int m){
	int m2=m-3;
	return 1+(value%m2);
}
/*
@brief belirlenen deðiþken sayýsýna göre hash tablosunun boyutunu belirleme
@param var_size geçerli deðiþken sayýsý
@return hash tablosunun boyutu
*/
int findTableSize(int var_size){
	int m=2*var_size+1;
	while(1){
		if(isPrime(m)){
			return m;
		}
		else{
			m+=2;
		}
	}
}
/*
@brief hash tablosunun boyutu asal sayý olmalý, hash tablosunun boyutunu bulurken asallýk kontrolü için kullanýlan fonksiyon
@param m olasý hash tablosunun deðeri
@return sayý asal ise 1 eðer asal deðilse 0 döndürür
*/
int isPrime(int m){
	int i;
	int sinir=sqrt(m);
	if (m <= 1) {
        return 0;  
    }
	for ( i = 2; i <= sinir; i++) {
        if (m % i == 0) {
            return 0;  
        }
    }
    return 1;
}
/*
@brief ayný isimde birden fazla deðiþken var mý diye kontrol eder
@param vartable elde edilen deðiþkenlerin saklandýðý ve sonrasýnda kontrol edildiði matris
@param var_size o ana kadar elde edilen deðiþken sayýsý
@param token kontrol edilecek deðiþkenin ismi
@return eðer deðiþken yoksa 0 varsa 1 deðeri döndürür
*/
int lookup(char vartable[100][2][100], int var_size, char* token) {
	int i;
    for ( i = 0; i < var_size; i++) {
        if (strcmp(vartable[i][1], token) == 0) {
            return 1;  // Deðiþken zaten var
        }
    }
    return 0;  // Deðiþken yok
}

/*
@brief ismi verilen deðiþkenin horner formülü ile sayýsal karþýlýðýný hesaplama
@param name deðiþken ismi
@return deðiþkenin sayýsal karþýlýðý
*/
int calcHorner(char* name){
	int j,k,len,prime=31;
	long  toplam=0;
	len=strlen(name);
	for(j=0;j<len;j++){
		long power = 1;
        for ( k = 0; k < len - 1 - j; k++) {
            power *= prime;
        }
		toplam+=power*name[j];
	}
	return toplam;
}


/*
@brief hash tablosunda deðiþken ekler
@param name deðiþken ismi
@param type deðiþken tipi
@param m hashValue hesaplayabilmek için hashtable in büyüklüðü
@param hashtable kontrol yapýlacak hash table
return ret hash tablosunda çakýþma olduysa bu sayýnýn hash tablosundaki ilk ve son deðerini ret üzerinden main e gönderiyoruz, 
bu sayede debug modda deðiþkenlerin hash tablosundaki ilk ve son deðerlerini görebiliryoruz
*/


int* insert(char* name,char* type,int m,char*** hashtable){
	int horner=calcHorner(name);
	char str[100];
	int* ret = (int*)malloc(2 * sizeof(int));
	int hashValue=hashFunction1(horner,m);
	//printf("%d in mod11e gore degeri:%d\n",horner,hashValue);
	ret[0]=hashValue;
	int hashQuad=1;
	while(strcmp(hashtable[hashValue][2],"null")!=0){ // null olmazsa diziyi sýfýrla ya da belli bir deðere set et sonra bu deðer deðiþmiþ diye kontrol et
		//printf("hashfunc2:%d\n",hashFunction2(horner,m));
		hashValue=(hashValue+hashQuad*(hashFunction2(horner,m)%m));
		hashQuad++;
	}
	strcpy(hashtable[hashValue][0],name);
	strcpy(hashtable[hashValue][1],type);
	sprintf(str, "%d", horner);
	strcpy(hashtable[hashValue][2],str);
	ret[1]=hashValue;
	//printf("%d--%d\n",hashValue,hashQuad);
	return ret;	

}
/*
@brief dosyadan okuma yapýp deklere edilmiþ deðiþkenleri bulur, ve gerektiðinde kullanýcýya hata mesajý döndürür
@param filename okunacak dosyanýn adý
@param vartable geçerli deðiþkenlerin saklandýðý matris
@param var_size vartable deki eleman sayýsý
@return
*/
void readFileAndProcess(char* filename, char vartable[100][2][100], int* var_size) {
    const char* keywords[] = {
        "int", "typedef", "struct", "union", "enum", "long", "short", "char", "float", "double", "const"};
    int size = sizeof(keywords) / sizeof(keywords[0]);
    char buffer[256];  // Satýr için buffer
    char* token;
    char reserved[100];

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Dosya acilamadi!\n");
        return;
    }

    // Dosyayý satýr satýr oku
    while (fgets(buffer, sizeof(buffer), fp)) {
        char* token = strtok(buffer, " ,(){}[];\n\t");  // Ayýrýcý olarak boþluk, virgül, parantez vb.
		int i;
        while (token != NULL) {
            // Anahtar kelimenin olup olmadýðýný kontrol et
            for ( i = 0; i < size; i++) {
                if (strcmp(token, keywords[i]) == 0) {
                    strcpy(reserved, token);  // Token'ý reserved dizisine kopyala
                }
            }
            // Reserved ve token karþýlaþtýrmasý
            if (token[0] == '_') {
                if (!lookup(vartable, *var_size, token)) {
                    if (reserved[0] != '\0') {
                        strcpy(vartable[*var_size][0], reserved);
                        strcpy(vartable[*var_size][1], token);
                        (*var_size)++;
                        printf("Yeni degisken bulundu: %s\n", token);
                    } else {
                        printf("Bu eleman deklere edilmedi:%s\n", token);
                    }
                } else {
                    if (reserved[0] != '\0') {
                        printf("Bu eleman daha once deklere edildi:%s\n", token);
                    }
                }
            }
            token = strtok(NULL, " ,(){}[];\n\t");  // Sonraki token'ý al
        }
        memset(reserved, 0, sizeof(reserved));
    }

    fclose(fp);
}
/*
@brief vartable ý ekrana yazdýrýr
@param vartable geçerli deðiþkenlerin saklandýðý matris
@param var_size vartable boyutu
@return
*/
void printVartable(char vartable[100][2][100], int var_size) {
	int i,j;
    for ( i = 0; i < var_size; i++) {
        for ( j = 0; j < 2; j++) {
            printf("%s ", vartable[i][j]);
        }
        printf("\n");
    }
}

/*
@brief verilen boyut kadar dinamik bellek oluþturur ve bunu null ile doldurur
@param m hashtable ýn boyutu
@return hashtable
*/
char*** createHashTable(int m) {
	int i,j;
    char*** hashtable = (char***)malloc(m * sizeof(char**));
    for ( i = 0; i < m; i++) {
        hashtable[i] = (char**)malloc(3 * sizeof(char*));
        for ( j = 0; j < 3; j++) {
            hashtable[i][j] = (char*)malloc(100 * sizeof(char));
        }
    }
    for(i=0;i<m;i++){
    	for(j=0;j<3;j++){
    		strcpy(hashtable[i][j],"null");
		}
	}
    return hashtable;
}

/*
@brief ilk ve son tablosunu oluþturur
@param var_size deðiþken sayýsý
@return
*/
char*** createIlkSonTable(int var_size) {
	int i,j;
    char*** ilk_son = (char***)malloc(var_size * sizeof(char**));
    for ( i = 0; i < var_size; i++) {
        ilk_son[i] = (char**)malloc(3 * sizeof(char*));
        for ( j = 0; j < 3; j++) {
            ilk_son[i][j] = (char*)malloc(100 * sizeof(char));
        }
    }
    return ilk_son;
}

/*
@brief ilk_son ý ekrana yazdýrýr
@param ilk_son deðiþkenlerin hashtable deki ilk ve son indisleri
@param var_size deðiþken sayýsý
@return
*/
void printIlkSonTable(char*** ilk_son, int var_size) {
	int i,j;
    for ( i = 0; i < var_size; i++) {
        printf("\n  ");
        for ( j = 0; j < 3; j++) {
            printf("%s  ", ilk_son[i][j]);
        }        
    }
}

/*
@brief hashtable ý ekrana yazdýrýr
@param hashtable sembol tablosu
@param m hashtable boyutu
@return
*/
void printHashTable(char*** hashtable, int m) {
	int i,j;
    for ( i = 0; i < m; i++) {
        printf("\n  ");
        for ( j = 0; j < 3; j++) {
            printf("%s   ", hashtable[i][j]);
        }        
    }
}


int main(){
	enum mod secim;
	int modint;
	int i,j;
    char vartable[100][2][100];
    int var_size=0;
    int* hornertable;
    int m;
	printf("Hangi modda calismak istersiniz:\n(Debug icin 0, Normal icin 1 giriniz)");
	scanf("%d",&modint);
	readFileAndProcess("dosya.txt", vartable, &var_size);
    
    printf("varsize:%d\n",var_size);
    
    printVartable(vartable,var_size);
    
    m=findTableSize(var_size);
    printf("tablesize:%d\n",m);
    
    char*** hashtable = createHashTable(m);
    char*** ilk_son = createIlkSonTable(var_size);
    
    int* ret;
	char ilk[100],son[100];
    for(i=0;i<var_size;i++){
    	ret=insert(vartable[i][1],vartable[i][0],m,hashtable);
		strcpy(ilk_son[i][0],vartable[i][1]);
		sprintf(ilk, "%d", ret[0]);
		sprintf(son, "%d", ret[1]);
		strcpy(ilk_son[i][1],ilk);
		strcpy(ilk_son[i][2],son);
	}
	
	
    
	if (modint == DEBUG || modint == NORMAL) {
        secim = (enum mod)modint;  
        if (secim == DEBUG) {
            printf("DEBUG modunda0 calisiyorsunuz.\n");
            printf("Deklere edilmis degisken sayisi:%d\n",var_size);
            printf("Sembol tablosunun uzunlugu:%d\n",m);
            printf("\nILKVESONTABLOSU\n");
			printIlkSonTable(ilk_son,var_size);
			printf("\n\nHASHTABLOSU\n");
			printHashTable(hashtable,m);
			
            
        } else {
            printf("NORMAL modda calistirdiniz.\n");
            
        }
    } else {
        printf("Geçersiz seçim!\n");
    }
	
	return 0;
}
