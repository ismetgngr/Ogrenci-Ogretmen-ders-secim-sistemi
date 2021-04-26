#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#include<time.h>
#include<conio.h>
#define SIZE 50
#if defined(WIN32) || defined(_WIN32)  || defined(__WIN32__) || defined(__NT__)
    #define CLS "cls"
#endif

typedef struct{
	int std_id;
	char std_isim[SIZE];
	char std_sisim[SIZE];
	int std_dersAdet;
	int std_kredi;
}STD;
// öðrenci için gerekli bilgilerin tutulduðu bölüm 

typedef struct{
	int ders_id;
	char ders_isim[SIZE];
	int ders_kredi;
	int ders_totalKont;
	int ders_kalanKont;
	int ders_sorumluId;
}DERS;
// dersler için gerekli bilgilerin tutulduðu bölüm

typedef struct{
	int ak_id; // ak -> akademisyen
	char ak_isim[SIZE];
	char ak_sisim[SIZE];
	char ak_unvan[SIZE];
}AK;
// akademisyenler için gerekli bilgilerin tutulduðu bölüm

typedef struct{
	int kayit_dersId;
	int kayit_ogrId;
	int kayit_kontrol;
	char kayit_tarih[SIZE]; // gün.ay.yýl formatýnda tutulacaktýr
}KAYIT;
// derslere kayýt durumunun takip edilebilmesi için

void ogrenciDosyaYazdirma(int, STD*, char*);
void dersDosyaYazdirma(int n_ders, DERS*, char*);
void akademisyenDosyaYazdirma(int, AK*, char*);
void kayitDosyaYazdirma(int, KAYIT*);

int ogrenciIlkKayit(int, char*, STD*);
void ogrenciEkle(char*, char*, int, int, STD**, char*);
void ogrenciSil(int, int, STD**, int, DERS*, char*, char*, int, KAYIT*);
void ogrenciGuncelle(int, char*, char*, int, STD*, char*);

int dersIlkKayit(int n, char *dosyaIsim, DERS *arr);
void dersEkle(char*, int, int, int, int, DERS**, char*);
void dersSil(int, int, DERS**, int, STD*, char*, char*, int, KAYIT*);
void dersGuncelle(int, char*, int, int, int, DERS*, char*);

int akademisyenIlkKayit(int n, char *dosyaIsim, AK *arr);
void akademisyenEkle(char*, char*, int, char*, int, AK**, char*);
void akademisyenDersKayit(int, int, int, DERS*, char*);
void akademisyenDersSil(int, int, int, DERS*, char*);
void akademisyenSil(int, int, AK**, int, DERS*, char*, char*);
void akademisyenGuncelle(int, char*, char*, char*, int, AK*, char*);

void ogrenciDersKayitDosyaOlusturma(int n_ogr, int n_ders, STD *ogr, DERS *ders, int n_kayit, KAYIT *kayit);
void ogrenciDersKayit(int, int, char*, int, KAYIT*, int, STD*, int, DERS*, int, int, char*, char*);
void ogrenciDersSil(int, int, int, KAYIT*, int, STD*, int, DERS*);//++

int akademisyenDerslerListe(int, int, DERS*);
void dersKayitliOgrenciListe(int, int, KAYIT*, int, STD*);
void ogrenciKayitliDersListe(int, int, KAYIT*, int, DERS*);
void akademisyenDerseKayitliOgrenciListe(int, int, KAYIT*, int, STD*, int, DERS*);

int dosyaElementSayma(char *dosyaIsim);
void tarihHesapla(char*);
void terminalBekletme();
int binaryStdSearch(STD*, int, int, int);
int binaryDersSearch(DERS*, int, int, int);
int binaryAkSearch(AK*, int, int, int);
int binaryDersSorumluSearch(DERS*, int, int, int);



int main(){
	setlocale(LC_ALL, "Turkish");
	int sonOgrId, sonAkId, sonDersId;
	int maxKredi, maxDers;
	int i, exit=1, input;
	int n_ogr, n_ak, n_ders, n_kayit;
	char dosyaOgrenci[SIZE], dosyaDers[SIZE], dosyaAkademisyen[SIZE];
	char dosyaKayit[SIZE] = "ogrenciderskayit.txt";
	char ad[SIZE], sad[SIZE], unvan[SIZE];
	int tmpId, tmpId2;
	
	printf("Ogrenci dosyasinin adini giriniz: ");
	scanf("%s", dosyaOgrenci);
	printf("Ders dosyasinin adini giriniz: ");
	scanf("%s", dosyaDers);
	printf("Akademisyen dosyasinin adini giriniz: ");
	scanf("%s", dosyaAkademisyen);
	printf("Öðrencinin alabileceði kredi üst sýnýrýný giriniz: ");
	scanf("%d", &maxKredi);
	printf("Öðrencinin alabileceði ders adet üst sýnýrýný giriniz ");
	scanf("%d", &maxDers);
	DERS *ders;
	STD *ogr;
	AK *ak;
	KAYIT *kayit;
	
	n_ogr = dosyaElementSayma(dosyaOgrenci);
	ogr = (STD*)malloc(n_ogr * sizeof(STD));
	n_ak =  dosyaElementSayma(dosyaAkademisyen);
	ak = (AK*)malloc(n_ak * sizeof(AK));
	n_ders =  dosyaElementSayma(dosyaDers);
	ders = (DERS*)malloc(n_ders * sizeof(DERS));
	n_kayit = n_ogr * n_ders;
	kayit = (KAYIT*)malloc(n_kayit * sizeof(KAYIT));
	
	i = ogrenciIlkKayit(n_ogr, dosyaOgrenci, ogr);
	if(i==-1){
		return -1;
	}
	i = dersIlkKayit(n_ders, dosyaDers, ders);
	if(i==-1){
		return -1;
	}
	i = akademisyenIlkKayit(n_ak, dosyaAkademisyen, ak);
	if(i==-1){
		return -1;
	}
	
	sonOgrId = ogr[n_ogr-1].std_id;
	sonAkId = ak[n_ak-1].ak_id;
	sonDersId= ders[n_ders-1].ders_id;
	
	ogrenciDersKayitDosyaOlusturma(n_ogr, n_ders, ogr, ders, n_kayit, kayit);
	char tarih[SIZE];
	
	
	printf("\nGeçmek için herhangi bir tuþa basýn");
	getch();
	while(exit == 1){
		system(CLS);
		printf("YAPMAK ÝSTEDÝÐÝNÝZ ÝÞLEMÝ SEÇÝN\n"
		"1-) Ekle/Sil/Güncelle iþlemleri\n"
		"2-) Listeleme iþlemleri\n"	 
		"3-) Çýkýþ \n");
		scanf("%d", &input);
		if(input == 1){
			system(CLS);
			printf("YAPMAK ÝSTEDÝÐÝNÝZ ÝÞLEMÝ SEÇÝN\n"
			"1-) Öðrenci iþlemleri \n"
			"2-) Akademisyen Ýþlemleri \n"
			"3-) Ders iþlemleri\n"
			"4-) Bir önceki menü\n"
			"5-) Çýkýþ \n");
			scanf("%d", &input);
			if(input == 1){
				system(CLS);
				printf("YAPMAK ÝSTEDÝÐÝNÝZ ÝÞLEMÝ SEÇÝNÝZ\n"
				"1-) Öðrenci ekle\n"
				"2-) Öðrenci sil\n"
				"3-) Öðrenci düzenle\n"
				"4-) Öðrenciyi derse ekle\n"
				"5-) Öðrenciyi dersten sil\n"
				"6-) Önceki menü\n"
				"7-) Çýkýþ\n");
				scanf("%d", &input);
				if(input == 1){
					system(CLS);
					printf("Eklenecek öðrencinin ismini giriniz: ");
					scanf("%s", ad);
					printf("Eklenecek öðrencinin soyismini giriniz: ");
					scanf("%s", sad);
					ogrenciEkle(ad, sad, sonOgrId+1, n_ogr, &ogr, dosyaOgrenci);
					sonOgrId++;
					n_ogr++;
					terminalBekletme();
				}
				else if(input == 2){
					system(CLS);
					printf("Silinecek öðrencinin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					ogrenciSil(tmpId, n_ogr, &ogr, n_ders, ders, dosyaDers, dosyaOgrenci, n_kayit, kayit);
					n_ogr--;
					n_kayit = n_ders*n_ogr;
					terminalBekletme();
				}
				else if(input == 3){
					system(CLS);
					printf("Güncellenecek öðrencinin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					printf("Güncelleme sonrasý gözükecek ismi giriniz: ");
					scanf("%s", ad);
					printf("Güncelleme sonrasý gözükecek soyismi giriniz: ");
					scanf("%s", sad);
					ogrenciGuncelle(tmpId, ad, sad, n_ogr, ogr, dosyaOgrenci);
					terminalBekletme();
				}
				else if(input == 4){
					system(CLS);
					printf("Öðrencinin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					printf("Kaydolacaðý dersin Id'sini giriniz: ");
					scanf("%d", &tmpId2);
					tarihHesapla(tarih);
					ogrenciDersKayit(tmpId, tmpId2, tarih, n_kayit, kayit, n_ogr, ogr, n_ders, ders, maxKredi, maxDers, dosyaDers, dosyaOgrenci);
					terminalBekletme();
				}
				else if(input == 5){
					system(CLS);
					printf("Dersten silinecek öðrencinin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					printf("Dersin Id'sini giriniz: ");
					scanf("%d", &tmpId2);
					ogrenciDersSil(tmpId, tmpId2, n_kayit, kayit, n_ogr, ogr, n_ders, ders);
					terminalBekletme();
				}
				else if(input == 6){
					exit=1;
				}
				else{
					exit=0;
				}
			}
			else if(input == 2){
				system(CLS);
				printf("YAPMAK ÝSTEDÝÐÝNÝZ ÝÞLEMÝ SEÇÝN\n"
				"1-) Akademisyen ekle\n"
				"2-) Akademisyen sil\n"
				"3-) Akademisyen düzenle\n"
				"4-) Akademisyene ders atama\n"
				"5-) Akademisyene atalý dersi kaldýrma\n"
				"6-) Önceki menü\n"
				"7-) Çýkýþ\n");
				scanf("%d", &input);
				if(input == 1){
					system(CLS);
					printf("Eklenecek akademisyenin ismini giriniz: ");
					scanf("%s", ad);
					printf("Eklenecek akademisyenin soyismini giriniz: ");
					scanf("%s", sad);
					printf("Eklenecek akademisyenin unvanýný giriniz: ");
					scanf("%s", unvan);
					akademisyenEkle(ad, sad, sonAkId+1, unvan, n_ak, &ak, dosyaAkademisyen);
					sonAkId++;
					n_ak++;
					terminalBekletme();
				}
				else if(input == 2){
					system(CLS);
					printf("Silinecek akademisyenin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					akademisyenSil(tmpId, n_ak, &ak, n_ders, ders, dosyaDers, dosyaAkademisyen);
					n_ak--;
					terminalBekletme();
				}
				else if(input == 3){
					system(CLS);
					printf("Güncellenecek akademisyenin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					printf("Güncelleme sonrasý gözükecek ismi giriniz: ");
					scanf("%s", ad);
					printf("Güncelleme sonrasý gözükecek soyismi giriniz: ");
					scanf("%s", sad);
					printf("Günceleme sonrsaý gözükecek unvaný giriniz: ");
					scanf("%s", unvan);
					akademisyenGuncelle(tmpId, ad, sad, unvan, n_ak, ak, dosyaAkademisyen);
					terminalBekletme();
				}
				else if(input == 4){
					system(CLS);
					printf("Ders atanacak akademisyenin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					printf("Atanacak dersin Id'sini giriniz: ");
					scanf("%d", &tmpId2);
					akademisyenDersKayit(tmpId, tmpId2, n_ders, ders, dosyaDers);
					terminalBekletme();
				}
				else if(input == 5){
					system(CLS);
					printf("Akademisyenin býrakacaðý dersin Id'sini giriniz: ");
					scanf("%d", &tmpId2);
					printf("Dersi býrakacak akademisyenin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					akademisyenDersSil(tmpId, tmpId2, n_ders, ders, dosyaDers);
					terminalBekletme();
				}
				else if(input == 6){
					exit = 1;
				}
				else{
					exit = 0;
				}
			}
			else if(input == 3){
				system(CLS);
				printf("YAPMAK ÝSTEDÝÐÝNÝZ ÝÞLEMÝ SEÇÝN\n"
				"1-) Ders ekle\n"
				"2-) Ders sil\n"
				"3-) Ders düzenle\n"
				"4-) Önceki menü\n"
				"5-) Çýkýþ\n");
				scanf("%d", &input);
				if(input == 1){
					system(CLS);
					printf("Eklenecek dersin adýný giriniz: ");
					scanf("%s", ad);
					printf("Dersin kontenjanýný giriniz: ");
					scanf("%d", &tmpId);
					printf("Dersin kredisini giriniz: ");
					scanf("%d", &tmpId2);
					dersEkle(ad, sonDersId+1, tmpId, tmpId2, n_ders, &ders, dosyaDers);
					sonDersId++;
					n_ders++;
					terminalBekletme();
				}
				else if(input == 2){
					system(CLS);
					printf("Silinecek dersin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					dersSil(tmpId, n_ders, &ders, n_ogr, ogr, dosyaDers, dosyaOgrenci, n_kayit, kayit);
					n_ders--;
					terminalBekletme();
				}
				else if(input == 3){
					system(CLS);
					printf("Güncellenecek Dersin Id'sini giriniz: ");
					scanf("%d", &tmpId);
					printf("Güncelleme sonrasý gözükecek ismi giriniz: ");
					scanf("%s", ad);
					printf("Güncelleme sonrasý gözükecek krediyi giriniz: ");
					scanf("%d", &tmpId2);
					printf("GÜncelleme sonrasý gözükecek toplam kontenjaný giriniz: ");
					scanf("%d", &i);
					dersGuncelle(tmpId, ad, tmpId2, i, n_ders, ders, dosyaDers);
					terminalBekletme();
				}
				else if(input == 4){
					exit = 1;
				}
				else{
					exit = 0;
				}
			}
			else if(input == 4){
				exit = 1;
			}
			else{
				exit = 0;
			}
		}
		else if(input == 2){
			system(CLS);
			printf("YAPMAK ÝSTEDÝÐÝNÝZ ÝÞLEMÝ SEÇÝN\n"
			"1-) Akademisyenin verdiði derler\n"
			"2-) Öðrencinin aldýðý dersler\n"
			"3-) Derse kayýtlý öðrenciler\n"
			"4-) Akademisyenin verdiði derse kayýtlý öðrenciler\n"
			"5-) Önceki menü\n"
			"6-) Çýkýþ\n");
			scanf("%d", &input);
			if(input == 1){
				system(CLS);
				printf("Dersleri listelenecek akademisyenin Id'sini giriniz: ");
				scanf("%d", &tmpId);
				akademisyenDerslerListe(tmpId, n_ders, ders);
				terminalBekletme();
			}
			else if(input == 2){
				system(CLS);
				printf("Aldýðý dersler listelecek olan öðrencinin Id'sini giriniz: ");
				scanf("%d", &tmpId);
				ogrenciKayitliDersListe(tmpId, n_kayit, kayit, n_ders, ders);
				terminalBekletme();
			}
			else if(input == 3){
				system(CLS);
				printf("Kayýtlý öðrencilerin yazdýrýlacaðý dersin Id'sini giriniz: ");
				scanf("%d", &tmpId);
				dersKayitliOgrenciListe(tmpId, n_kayit, kayit, n_ogr, ogr);
				terminalBekletme();
			}
			else if(input == 4){
				system(CLS);
				printf("Verdiði derse kayýtlý öðrencilerin yazdýrýlacaðý akademisyenin Id'sini giriniz: ");
				scanf("%d", &tmpId);
				akademisyenDerseKayitliOgrenciListe(tmpId, n_kayit, kayit, n_ogr, ogr, n_ders, ders);
				terminalBekletme();
			}
			else if(input == 5){
				exit = 1;
			}
			else{
				exit = 0;
			}
		}
		else{
			exit = 0;
		}
	}
	
	free(kayit);
	free(ders);
	free(ogr);
	free(ak);
	return 0;
}

int binaryStdSearch(STD *arr, int l, int r, int x){ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
        
        if (arr[mid].std_id == x){ 
            return mid;
		}
        if (arr[mid].std_id > x){
            return binaryStdSearch(arr, l, mid - 1, x); 
        }
        return binaryStdSearch(arr, mid + 1, r, x); 
    } 
    return -1; 
} 
int binaryDersSearch(DERS *arr, int l, int r, int x){ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
        
        if (arr[mid].ders_id == x){ 
            return mid; 
		}
        if (arr[mid].ders_id > x){
            return binaryDersSearch(arr, l, mid - 1, x); 
        }
        return binaryDersSearch(arr, mid + 1, r, x); 
    } 
    return -1; 
}
int binaryAkSearch(AK *arr, int l, int r, int x){
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
        
        if (arr[mid].ak_id == x){ 
            return mid; 
		}
        if (arr[mid].ak_id > x){
            return binaryAkSearch(arr, l, mid - 1, x); 
        }
        return binaryAkSearch(arr, mid + 1, r, x); 
    } 
    return -1; 
}  
int binaryDersSorumluSearch(DERS *arr, int l, int r, int x){ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
        
        if (arr[mid].ders_sorumluId == x){ 
            return mid; 
		}
        if (arr[mid].ders_sorumluId > x){
            return binaryDersSorumluSearch(arr, l, mid - 1, x); 
        }
        return binaryDersSorumluSearch(arr, mid + 1, r, x); 
    } 
    return -1; 
}

int dosyaElementSayma(char *dosyaIsim){
	int counter=1;
	FILE *fp = fopen(dosyaIsim, "r");
	char harf;
	harf = getc(fp);
	while(harf != EOF){
		if(harf == '\n'){
			counter++;
		}
		harf = getc(fp);
	}
	fclose(fp);
	return counter;
}

void terminalBekletme(){
	printf("\nGeçmek için herhangi bir tuþa basýn");
	getch();
}
void tarihHesapla(char* tarih){
    time_t vakit;
    time(&vakit);
    sprintf(tarih, ctime(&vakit));
}
void ogrenciDosyaYazdirma(int n_ogr, STD* ogr, char* dosyaOgrenci){
	int i;
	FILE* fw;
	fw = fopen(dosyaOgrenci, "w");
	for(i=0; i<n_ogr; i++){
		fprintf(fw, "%d %s %s %d %d", ogr[i].std_id, ogr[i].std_isim, ogr[i].std_sisim, ogr[i].std_dersAdet, ogr[i].std_kredi);
		if(i!=n_ogr-1){
			fprintf(fw,"\n");
		}	
	}
	fclose(fw);
}

void dersDosyaYazdirma(int n_ders, DERS* ders, char* dosyaDers){
	int i;
	FILE* fw;
	fw = fopen(dosyaDers, "w");
	for(i=0; i<n_ders; i++){
		fprintf(fw, "%d %s %d %d %d %d", ders[i].ders_id, ders[i].ders_isim, ders[i].ders_kredi, ders[i].ders_totalKont, ders[i].ders_kalanKont, ders[i].ders_sorumluId);
		if(i!=n_ders-1){
			fprintf(fw,"\n");
		}	
	}
	fclose(fw);
}

void kayitDosyaYazdirma(int n_kayit, KAYIT* kayit){
	int k;
	FILE* fw;
	fw = fopen("OgrenciDersKayit.txt", "w");
	for(k=0; k<n_kayit; k++){
		fprintf(fw,"%d %d %d %.24s\n", kayit[k].kayit_ogrId, kayit[k].kayit_dersId, kayit[k].kayit_kontrol, kayit[k].kayit_tarih);
	}
	fclose(fw);
}

void akademisyenDosyaYazdirma(int n_ak, AK* ak, char* dosyaAkademisyen){
	int i;
	FILE* fw;
	fw = fopen(dosyaAkademisyen, "w");
	for(i=0; i<n_ak; i++){
		fprintf(fw, "%d %s %s %s", ak[i].ak_id, ak[i].ak_isim, ak[i].ak_sisim, ak[i].ak_unvan);
		if(i!=n_ak-1){
			fprintf(fw,"\n");
		}
	}
	fclose(fw);
}

int ogrenciIlkKayit(int n, char *dosyaIsim, STD *arr){
	FILE *data=fopen(dosyaIsim, "r");
	if(!data){
		printf("Öðrenci dosyasý açýlamadý");
		return -1;
	}
	int i=0;
	while(!feof(data)){
		fscanf(data, "%d %s %s %d %d", &arr[i].std_id, arr[i].std_isim, arr[i].std_sisim, &arr[i].std_dersAdet, &arr[i].std_kredi);
		i++;
	}
	fclose(data);
	return;
}

int dersIlkKayit(int n, char *dosyaIsim, DERS *arr){
	FILE *data=fopen(dosyaIsim, "r");
	if(!data){
		printf("Ders dosyasý açýlamadý");
		return -1;
	}
	int i=0;
	while(!feof(data)){
		fscanf(data, "%d %s %d %d %d %d", &arr[i].ders_id, arr[i].ders_isim, &arr[i].ders_kredi, &arr[i].ders_totalKont, &arr[i].ders_kalanKont, &arr[i].ders_sorumluId);
		i++;
	}
	fclose(data);
	return;	
}

int akademisyenIlkKayit(int n, char *dosyaIsim, AK *arr){
	FILE *data=fopen(dosyaIsim, "r");
	if(!data){
		printf("Akademisyen dosyasý açýlamadý");
		return -1;
	}
	int i=0;
	while(!feof(data)){
		fscanf(data, "%d %s %s %s", &arr[i].ak_id, arr[i].ak_isim, arr[i].ak_sisim, arr[i].ak_unvan);
		i++;
	}
	fclose(data);
	return;
}

void ogrenciDersKayitDosyaOlusturma(int n_ogr, int n_ders, STD *ogr, DERS *ders, int n_kayit, KAYIT *kayit){
	int i, j, k=0;
	int ders_idx, ogr_idx;
	char fileName[30];
	FILE *fw;
	fw = fopen("OgrenciDersKayit.txt", "w");
	for(i=0; i<n_ogr; i++){
		for(j=0; j<n_ders; j++){
			kayit[k].kayit_ogrId = ogr[i].std_id;
			kayit[k].kayit_dersId = ders[j].ders_id;
			kayit[k].kayit_kontrol = 0;
			strcpy(kayit[k].kayit_tarih,"0") ;
			fprintf(fw,"%d %d %d %s", kayit[k].kayit_ogrId, kayit[k].kayit_dersId, kayit[k].kayit_kontrol, kayit[k].kayit_tarih);
			k++;
			if(k != n_kayit){
				fprintf(fw, "\n");
			}
		}
	}
	fclose(fw);
	return;
}

void ogrenciDersKayit(int stdId, int dersId, char *tarih, int n_kayit, KAYIT *kayit, int n_ogr, STD *ogr, int n_ders, DERS *ders, int maxKredi, int maxDers, char *dosyaDersIsim, char *dosyaOgrIsim){
	int i;
	int ogr_idx = binaryStdSearch(ogr, 0, n_ogr, stdId);
	if (ogr_idx == -1){
		printf("Ýstenilen öðrenci yok");
		return;
	}
	int ders_idx = binaryDersSearch(ders, 0, n_ders, dersId);
	if (ders_idx == -1){
		printf("Ýstenilen ders yok\n");
		return;
	}
	ogr[ogr_idx].std_dersAdet++;
	ogr[ogr_idx].std_kredi += ders[ders_idx].ders_kredi;
	ders[ders_idx].ders_kalanKont--;
	if(ders[ders_idx].ders_kalanKont>=0 && ogr[ogr_idx].std_dersAdet<=maxDers && ogr[ogr_idx].std_kredi<=maxKredi){	
		int k=0, control=0;
		while(k<n_kayit && control != 1){
			if(stdId == kayit[k].kayit_ogrId && dersId == kayit[k].kayit_dersId){
				kayit[k].kayit_kontrol=1;
				strcpy(kayit[k].kayit_tarih, tarih);
				control=1;
			}
			k++;
		}
		kayitDosyaYazdirma(n_kayit, kayit);
		ogrenciDosyaYazdirma(n_ogr, ogr, dosyaOgrIsim);
		dersDosyaYazdirma(n_ders, ders, dosyaDersIsim);
	}
	else{
		printf("Ders için kredi veya ders seçim hakký yetersiz.\n");
		return;
	}
}

void ogrenciEkle(char *isim, char *sisim, int newId, int n_ogr, STD** ogr, char *dosyaStdIsim){
	int i;
	*ogr = (STD*)realloc(*ogr, (n_ogr+1) * sizeof(STD));
	ogrenciIlkKayit(n_ogr, dosyaStdIsim, *ogr);
	if(*ogr == NULL){
		printf("Alan arttýrýlamadý.");
		return;
	}
	(*ogr)[n_ogr].std_id = newId;
	strcpy((*ogr)[n_ogr].std_isim, isim);
	strcpy((*ogr)[n_ogr].std_sisim, sisim);
	(*ogr)[n_ogr].std_dersAdet = 0;
	(*ogr)[n_ogr].std_kredi = 0;
	ogrenciDosyaYazdirma(n_ogr+1, *ogr, dosyaStdIsim);

}

void dersEkle(char* isim, int newId, int tKontenjan, int kredi, int n_ders, DERS** ders, char* dosyaDersIsim){
	int i;
	*ders = (DERS*)realloc(*ders, (n_ders+1)*sizeof(DERS));
	if(*ders == NULL){
		printf("Alan arttýrýlamadý.");
		return;
	}
	(*ders)[n_ders].ders_id = newId;
	strcpy((*ders)[n_ders].ders_isim, isim);
	(*ders)[n_ders].ders_totalKont = tKontenjan;
	(*ders)[n_ders].ders_kredi = kredi;
	(*ders)[n_ders].ders_sorumluId = 0;
	(*ders)[n_ders].ders_kalanKont = tKontenjan;
	dersDosyaYazdirma(n_ders+1, *ders, dosyaDersIsim);
}

void akademisyenEkle(char* isim, char* sisim, int newId, char* unvan, int n_ak, AK** ak, char* dosyaAkIsim){
	int i;
	*ak = (AK*)realloc(*ak, (n_ak+1)*sizeof(AK));
	if(*ak == NULL){
		printf("Alan arttýrýlamadý.");
		return;
	}
	(*ak)[n_ak].ak_id = newId;
	strcpy((*ak)[n_ak].ak_isim, isim);
	strcpy((*ak)[n_ak].ak_sisim, sisim);
	strcpy((*ak)[n_ak].ak_unvan, unvan);
	akademisyenDosyaYazdirma(n_ak+1, *ak, dosyaAkIsim);
}

void akademisyenDersKayit(int akId, int dersId, int n_ders, DERS* ders, char* dosyaDersIsim){
	int x, i;
	x = binaryDersSearch(ders, 0, n_ders, dersId);
	if(x==-1){
		printf("Böyle bir ders yok");
	}
	else{
		ders[x].ders_sorumluId = akId;
		dersDosyaYazdirma(n_ders, ders, dosyaDersIsim);
	}
}

void akademisyenDersSil(int akId, int dersId, int n_ders, DERS* ders, char* dosyaDersIsim){
	int x, i, control;
	i=0, control=0;
	while(i<n_ders && control!=1){
		if(ders[i].ders_id == dersId && ders[i].ders_sorumluId==akId){
			ders[i].ders_sorumluId = 0;
			control = 1;
		}
		i++;
	}
	if(control==0){
		printf("Akademisyen istenen dersi vermiyor.\n");
		return;
	}
	dersDosyaYazdirma(n_ders, ders, dosyaDersIsim);
}

void akademisyenSil(int akId, int n_ak, AK** ak, int n_ders, DERS* ders, char* dosyaDersIsim, char* dosyaAkIsim){
	int i;
	int ak_idx = binaryDersSorumluSearch(ders, 0, n_ders, akId);
	if(ak_idx != -1){
		ders[ak_idx].ders_sorumluId = 0;
		dersDosyaYazdirma(n_ders, ders, dosyaDersIsim);
	}
	ak_idx = binaryAkSearch(*ak, 0, n_ak, akId);
	for(i=0; i<n_ak-1; i++){
		if(i>=ak_idx){
			(*ak)[i].ak_id = (*ak)[i+1].ak_id;
			strcpy((*ak)[i].ak_isim, (*ak)[i+1].ak_isim);
			strcpy((*ak)[i].ak_sisim, (*ak)[i+1].ak_sisim);
			strcpy((*ak)[i].ak_unvan, (*ak)[i+1].ak_unvan);
		}
	}
	*ak = (AK*)realloc(*ak, (n_ak-1)*sizeof(AK));
	if(*ak == NULL){
		printf("Alan azaltýlamadý.");
		return;
	}
	akademisyenDosyaYazdirma(n_ak-1, *ak, dosyaAkIsim);
}

void dersSil(int dersId, int n_ders, DERS** ders, int n_ogr, STD* ogr, char* dosyaDersIsim, char* dosyaOgrIsim, int n_kayit, KAYIT* kayit){
	int i, j, k, ders_idx, ogr_idx, tmp;
	ders_idx = binaryDersSearch(*ders, 0, n_ders, dersId);
	if(ders_idx == -1){
		printf("Ders zaten yok");
		return;
	}
	tmp = (*ders)[ders_idx].ders_kredi;
	for(i=0; i<n_ders; i++){
		if(i>=ders_idx){
			(*ders)[i].ders_id = (*ders)[i+1].ders_id;
			strcpy((*ders)[i].ders_isim, (*ders)[i+1].ders_isim);
			(*ders)[i].ders_kalanKont = (*ders)[i+1].ders_kalanKont;
			(*ders)[i].ders_kredi = (*ders)[i+1].ders_kredi;
			(*ders)[i].ders_sorumluId = (*ders)[i+1].ders_sorumluId;
			(*ders)[i].ders_totalKont = (*ders)[i+1].ders_totalKont;
		}
	}
	*ders = (DERS*)realloc(*ders,sizeof(DERS)*(n_ders-1));
	if(*ders == NULL){
		printf("Alan azaltýlamadý.");
		return;
	}
	dersDosyaYazdirma(n_ders-1, *ders, dosyaDersIsim);
	for(i=0; i<n_kayit; i++){
		if(dersId==kayit[i].kayit_dersId && kayit[i].kayit_kontrol ==1){
			ogr_idx = binaryStdSearch(ogr, 0, n_ogr, kayit[i].kayit_ogrId);
			ogr[ogr_idx].std_dersAdet-- ;
			ogr[ogr_idx].std_kredi -= tmp;
		}
	}
	ogrenciDosyaYazdirma(n_ogr, ogr, dosyaOgrIsim);
	n_kayit = n_ders * n_ogr;
	kayitDosyaYazdirma(n_kayit, kayit);
}

void ogrenciSil(int ogrId, int n_ogr, STD** ogr, int n_ders, DERS* ders, char* dosyaDersIsim, char* dosyaOgrIsim, int n_kayit, KAYIT* kayit){
	int i, j, ogr_idx;
	ogr_idx = binaryStdSearch(*ogr, 0, n_ogr, ogrId);
	if(ogr_idx == -1){
		printf("Ogrenci zaten yok");
		return;
	}
	for(i=0; i<n_ogr; i++){
		if(i>=ogr_idx){
			(*ogr)[i].std_id=(*ogr)[i+1].std_id;
			strcpy((*ogr)[i].std_isim, (*ogr)[i+1].std_isim);
			strcpy((*ogr)[i].std_sisim, (*ogr)[i+1].std_sisim);
			(*ogr)[i].std_dersAdet = (*ogr)[i+1].std_dersAdet;
			(*ogr)[i].std_kredi = (*ogr)[i+1].std_kredi;
		}
	}
	*ogr = (STD*)realloc(*ogr, sizeof(STD)*(n_ogr-1));
	if(*ogr == NULL){
		printf("Alan azaltýlamadi");
		return;
	}
	ogrenciDosyaYazdirma(n_ogr-1, *ogr, dosyaOgrIsim);
	n_kayit = n_ogr * n_ders;
	kayitDosyaYazdirma(n_kayit, kayit);
}

void ogrenciDersSil(int ogrId, int dersId, int n_kayit, KAYIT *kayit, int n_ogr, STD *ogr, int n_ders, DERS *ders){
	int i=0, control=0;
	while(i<n_kayit && control != 1){
		if(kayit[i].kayit_dersId == dersId && kayit[i].kayit_ogrId == ogrId && kayit[i].kayit_kontrol == 1){
			kayit[i].kayit_kontrol = 0;
			control = 1;
		}
		i++;
	}
	if(control == 0){
		printf("Öðrenci derse kayýtlý deðil.\n");
		return;
	}
	kayitDosyaYazdirma(n_kayit, kayit);
	
}

void akademisyenGuncelle(int akId, char* isim, char* sisim, char* unvan, int n_ak, AK* ak, char* dosyaAkademisyen){
	int i, x;
	x = binaryAkSearch(ak, 0, n_ak, akId);
	strcpy(ak[x].ak_isim,isim);
	strcpy(ak[x].ak_sisim, sisim);
	strcpy(ak[x].ak_unvan, unvan);
	akademisyenDosyaYazdirma(n_ak, ak, dosyaAkademisyen);
}

void ogrenciGuncelle(int ogrId, char* isim, char* sisim, int n_ogr, STD* ogr, char* dosyaOgrenci){
	int i, x;
	x = binaryStdSearch(ogr, 0, n_ogr, ogrId);
	strcpy(ogr[x].std_isim, isim);
	strcpy(ogr[x].std_sisim, sisim);
	ogrenciDosyaYazdirma(n_ogr, ogr, dosyaOgrenci);
}

void dersGuncelle(int dersId, char* isim, int kredi, int tKontenjan, int n_ders, DERS* ders, char* dosyaDers){
	int i, x, tmp;
	x = binaryDersSearch(ders, 0, n_ders, dersId);
	strcpy(ders[x].ders_isim, isim);
	tmp = tKontenjan - ders[x].ders_totalKont;
	ders[x].ders_totalKont = tKontenjan;
	ders[x].ders_kalanKont += tmp;
	dersDosyaYazdirma(n_ders, ders, dosyaDers);
}

int akademisyenDerslerListe(int akId, int n_ders, DERS* ders){
	int i, count=0;
	for(i=0; i<n_ders; i++){
		if(ders[i].ders_sorumluId == akId){
			printf("%d. Id: %d - Ders Adý: %s\n",count+1, ders[i].ders_id, ders[i].ders_isim);
			count++;
		}
	}
	if(count==0){
		printf("Akademisyen ders vermiyor.\n");
		return -1;
	}
	return 0;
}

void dersKayitliOgrenciListe(int dersId, int n_kayit, KAYIT* kayit, int n_ogr, STD* ogr){
	int i, count=0, x, idx;
	printf("Kayýtlý Öðrenciler:\n");
	for(i=0; i<n_kayit; i++){
		if(kayit[i].kayit_dersId == dersId && kayit[i].kayit_kontrol==1){
			x = kayit[i].kayit_ogrId;
			idx = binaryStdSearch(ogr, 0, n_ogr, x);
			printf("%d. Id: %d - Isim: %s - Soyisim: %s\n", count+1, ogr[idx].std_id, ogr[idx].std_isim, ogr[idx].std_sisim);
			count++;
		}
	}
	if(count==0){
		printf("Derse kayýtlý öðrenci yok.\n");
	}
}

void ogrenciKayitliDersListe(int ogrId, int n_kayit, KAYIT* kayit, int n_ders, DERS* ders){
	int i, count=0, x, idx;
	printf("Öðrencinin kayýtlý olduðu dersler:\n");
	for(i=0; i<n_kayit; i++){
		if(kayit[i].kayit_ogrId == ogrId && kayit[i].kayit_kontrol==1){
			x = kayit[i].kayit_dersId;
			idx = binaryDersSearch(ders, 0, n_ders, x);
			printf("%d. Id: %d - Ders Adý: %s\n", count+1, ders[idx].ders_id, ders[idx].ders_isim);
		count++;
		}
	} 
	if(count==0){
		printf("Ogrencinin Kayitli oldugu ders yok.\n");
	}
}

void akademisyenDerseKayitliOgrenciListe(int akId, int n_kayit, KAYIT* kayit, int n_ogr, STD* ogr, int n_ders, DERS* ders){
	int dersId, y;
	y = akademisyenDerslerListe(akId, n_ders, ders);
	if(y==-1){
		return;
	}
	printf("Öðrenci listesine eriþmek istediðiniz ders Id'sini giriniz:");
	scanf("%d", &dersId);
	int i, count=0, x, idx;
	char dosyaAdi[SIZE];
	itoa(dersId, dosyaAdi, 10);
	strcat(dosyaAdi, "_SINIFLISTESI.txt"); 
	FILE* fw;
	fw = fopen(dosyaAdi, "a");
	for(i=0; i<n_kayit; i++){
		if(kayit[i].kayit_dersId == dersId && kayit[i].kayit_kontrol==1){
			x = kayit[i].kayit_ogrId;
			idx = binaryStdSearch(ogr, 0, n_ogr, x);
			fprintf(fw,"%d. Id: %d - Isim: %s - Soyisim: %s\n", count+1, ogr[idx].std_id, ogr[idx].std_isim, ogr[idx].std_sisim);
			count++;
		}
	}
	if(count==0){
		fprintf(fw,"Derse kayitli ogrenci yok.");
	}
	fclose(fw);
}

