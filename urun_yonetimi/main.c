#include <stdio.h>  //printf scanf gibi fonksiyonları kullanmak için kullanlır
#include <stdlib.h> //genel yardımcı fonksiyonlar gelir
#include <string.h> //string işlemleri için kullanılır (strcpy , strcmp , strlen gibi)
// Ürün yapısı
struct Urun {
  int id;
  char *ad;       // Dinamik string    // ad ve kategorinin belli uzunlugu yok
  char *kategori; // Dinamik string
  float fiyat;
};

struct Urun *urunler = NULL; // Dinamik dizi pointer'ı
int urun_sayisi = 0;         // Mevcut ürün sayısı
int kapasite_urun = 5;      // Başlangıç kapasitesi (ihtiyaca göre artacak)

// Fonksiyon Prototipleri
void menu_goster(void);
void urun_ekle(void);
void urun_listele(void);
void urun_ara(void);
void urun_guncelle(void);
void urun_sil(void);
void dosyaya_kaydet(void);
void dosyadan_oku(void);
int id_var_mi(int id); // girilen id daha önce var mı yok mu ona bakar
void urunleri_sirala(void);
void yer_degistir(struct Urun *a, struct Urun *b);
void raporlama(void);

int main(void) {
  int secim;

  printf("Veriler yukleniyor...\n");

  // Başlangıç için bellek ayırıyoruz
  urunler = (struct Urun *)malloc(kapasite_urun * sizeof(struct Urun));
  if (urunler == NULL) {
    printf("Kritik Hata: Bellek ayrilamadi!\n");
    return 1; // bellek ayrılamadığı için veriler kaydedilemez ve program sonlanır
  }           // return 0 olsaydı programın hafıza ayıramadığı için çöktüğünü işletim sistemi anlamazdı bilgisayar programın başarıyla tamamlandığını sanırdı

  dosyadan_oku(); // Program baslarken dosyadaki verileri okur

  do {
    menu_goster(); // menüyü gösterir
    printf("Seciminizi yapiniz (0-7): ");
    scanf("%d", &secim);

    switch (secim) {
    case 1:
      urun_ekle();
      break;
    case 2:
      urun_listele();
      break;
    case 3:
      urun_ara();
      break;
    case 4:
      urun_guncelle();
      break;
    case 5:
      urun_sil();
      break;
    case 6:
      dosyaya_kaydet();
      break;
    case 7:
      raporlama();
      break;
    case 0:
      printf("Cikis yapiliyor ve kaydediliyor...\n");
      dosyaya_kaydet();
      printf("Gule gule!\n");
      break;
    default:
      printf("Hatali secim! Lutfen tekrar deneyin.\n");
    }
  } while (secim != 0);

  // Program kapanırken belleği serbest bırak (Memory Management)
  for (int i = 0; i < urun_sayisi; i++) {
    free(urunler[i].ad);
    free(urunler[i].kategori);
  }
  if (urunler != NULL) {
    free(urunler);
  }

  return 0;
}

void menu_goster(void) {
  printf("----------------------------\n");
  printf("URUN YONETIM SISTEMI\n");
  printf("1. Yeni Urun Ekle\n");
  printf("2. Urunleri Listele\n");
  printf("3. Urun Ara\n");
  printf("4. Urun Guncelle\n");
  printf("5. Urun Sil\n");
  printf("6. Dosyaya Kaydet\n");
  printf("7. Raporlama\n");
  printf("0. Cikis\n");
  printf("----------------------------\n");
}

void urun_ekle(void) {
  printf("\n--- Yeni Urun Ekle ---\n");

  // Bellek doluysa kapasiteyi artırıyoruz (Realloc)
  if (urun_sayisi >= kapasite_urun) {
    kapasite_urun *= 2;
    struct Urun *yeni_liste =
        (struct Urun *)realloc(urunler, kapasite_urun * sizeof(struct Urun));
    if (yeni_liste == NULL) {
      printf("Hata: Hafiza genisletilemedi!\n");
      return;
    }
    urunler = yeni_liste;
  }

  struct Urun yeniUrun;
  int girilen_id;
  while (1) {
    printf("Urun ID: ");
    scanf("%d", &girilen_id);
    if (id_var_mi(girilen_id)) {
      printf("Hata: Bu ID (%d) zaten kullaniliyor. Baska bir ID giriniz.\n",
             girilen_id);
    } else {
      yeniUrun.id = girilen_id;
      break;
    }
  }

  char temp_ad[100], temp_kat[100];

  printf("Urun Adi: ");
  scanf(" %[^\n]s", temp_ad);
  yeniUrun.ad = strdup(temp_ad); // Dinamik bellek ayırır ve kopyalar

  printf("Kategori: ");
  scanf(" %[^\n]s", temp_kat);
  yeniUrun.kategori = strdup(temp_kat);

  printf("Fiyat: ");
  scanf("%f", &yeniUrun.fiyat);

  urunler[urun_sayisi] = yeniUrun;
  urun_sayisi++;

  printf("Urun basariyla eklendi!\n");
}

void urun_listele(void) {
  printf("\n--- Kayitli Urunler ---\n");
  if (urun_sayisi == 0) {
    printf("Listelenecek urun yok.\n");
    return;
  }

  // Listelemeden once siralayalim (Bubble Sort)
  urunleri_sirala(); // id göre sıralama yapar

  printf("%-5s %-20s %-15s %-10s\n", "ID", "Urun Adi", "Kategori",
         "Fiyat"); // tablonun sutun başlıklarını yazar
                   //-5s 5 karakterlik yer ayırır ve - sola yaslar
  printf("-------------------------------------------------------\n");

  for (int i = 0; i < urun_sayisi; i++) { // listedki tüm ürünleri tek tek gezer
    printf("%-5d %-20s %-15s %-10.2f TL\n", urunler[i].id, urunler[i].ad,
           urunler[i].kategori, urunler[i].fiyat);
  }
}

void urun_ara(void) {
  printf("\n--- Urun Arama ---\n");
  if (urun_sayisi == 0) {
    printf("Aranacak veri yok.\n");
    return;
  }

  int aranan_id;
  int bulundu = 0; // 0: Hayir, 1: Evet

  printf("Aranacak Urun ID'si: ");
  scanf("%d", &aranan_id);

  for (int i = 0; i < urun_sayisi; i++) {
    if (urunler[i].id ==
        aranan_id) { // aranan id var olan id lerle karşılaştırılır
      printf("\nUrun Bulundu:\n");
      printf("Ad: %s\n", urunler[i].ad);
      printf("Kategori: %s\n", urunler[i].kategori);
      printf("Fiyat: %.2f TL\n", urunler[i].fiyat);
      bulundu = 1;
      break; // Bulduk, donguden cikabiliriz
    }
  }

  if (bulundu == 0) {
    printf("Bu ID'ye sahip urun bulunamadi.\n");
  }
}

void urun_guncelle(void) {
  printf("\n--- Urun Guncelle ---\n");
  if (urun_sayisi == 0) {
    printf("Guncellenecek urun yok.\n");
    return;
  }

  int guncellenecek_id;
  printf("Guncellenecek Urun ID: ");
  scanf("%d", &guncellenecek_id);

  for (int i = 0; i < urun_sayisi; i++) {
    if (urunler[i].id == guncellenecek_id) {
      char temp_ad[100], temp_kat[100];
      printf("Urun bulundu: %s (%.2f TL)\n", urunler[i].ad, urunler[i].fiyat);

      printf("Yeni Urun Adi: ");
      scanf(" %[^\n]s", temp_ad);
      free(urunler[i].ad);             // Eski ismi sil
      urunler[i].ad = strdup(temp_ad); // Yeni ismi ata

      printf("Yeni Kategori: ");
      scanf(" %[^\n]s", temp_kat);
      free(urunler[i].kategori); // Eski kategoriyi sil
      urunler[i].kategori = strdup(temp_kat);

      printf("Yeni Fiyat: ");
      scanf("%f", &urunler[i].fiyat);

      printf("Urun basariyla guncellendi.\n");
      return;
    }
  }
  printf("Bu ID ile bir urun bulunamadi.\n");
}

void urun_sil(void) {
  printf("\n--- Urun Sil ---\n");
  if (urun_sayisi == 0) {
    printf("Silinecek urun yok.\n");
    return;
  }

  int silinecek_id;
  printf("Silinecek Urun ID: ");
  scanf("%d", &silinecek_id);

  for (int i = 0; i < urun_sayisi; i++) {
    if (urunler[i].id == silinecek_id) {
      // Önce bu ürünün içindeki dinamik belleği temizliyoruz
      free(urunler[i].ad);
      free(urunler[i].kategori);

      // Kaydırma işlemi
      for (int j = i; j < urun_sayisi - 1; j++) {
        urunler[j] = urunler[j + 1];
      }
      urun_sayisi--;
      printf("Urun silindi.\n");
      return;
    }
  }
  printf("Silinecek urun bulunamadi.\n");
}

// Dosyasi islemlerinde basit text formati kullaniyoruz: ID;Ad;Kategori;Fiyat
void dosyaya_kaydet(void) {
  FILE *dosya = fopen("veri_tabani.txt", "w");
  if (dosya == NULL) {
    printf("Hata: Dosya acilamadi!\n");
    return;
  }

  for (int i = 0; i < urun_sayisi; i++) {
    // Verileri dosyaya yaziyoruz. Isimlerde bosluk olabilecegi icin
    // virgul veya noktali virgul ile ayirmak daha güvenli olurdu ama
    // bu seviyede basit tab (\t) veya ozel format kullandik.
    fprintf(dosya, "%d;%s;%s;%.2f\n", urunler[i].id, urunler[i].ad,
            urunler[i].kategori, urunler[i].fiyat);
  }

  fclose(dosya);
  printf("Tum veriler 'veri_tabani.txt' dosyasina kaydedildi.\n");
}

void dosyadan_oku(void) {
  FILE *dosya = fopen("veri_tabani.txt", "r");
  if (dosya == NULL)
    return;

  char temp_ad[100], temp_kat[100];
  int id;
  float fiyat;

  while (fscanf(dosya, "%d;%[^;];%[^;];%f\n", &id, temp_ad, temp_kat, &fiyat) !=
         EOF) {
    // Kapasite kontrolü
    if (urun_sayisi >= kapasite_urun) {
      kapasite_urun *= 2;
      urunler = (struct Urun *)realloc(urunler, kapasite_urun * sizeof(struct Urun));
    }

    urunler[urun_sayisi].id = id;
    urunler[urun_sayisi].ad = strdup(temp_ad);
    urunler[urun_sayisi].kategori = strdup(temp_kat);
    urunler[urun_sayisi].fiyat = fiyat;
    urun_sayisi++;
  }

  fclose(dosya);
  printf("%d urun dosyadan yuklendi.\n", urun_sayisi);
}

// Girilen ID'nin listede olup olmadigini kontrol eder
// Varsa 1, yoksa 0 dondurur.
int id_var_mi(int id) {
  for (int i = 0; i < urun_sayisi; i++) {
    if (urunler[i].id == id) {
      return 1;
    }
  }
  return 0;
}

// Urunleri ID numarasina gore kucukten buyuge siralar
// Bubble Sort algoritmasi kullanilmistir.
void urunleri_sirala(void) {
  for (int i = 0; i < urun_sayisi - 1; i++) {
    for (int j = 0; j < urun_sayisi - 1 - i; j++) {
      // Eger simdiki elemanin ID'si bir sonrakinden buyukse yer degistir
      if (urunler[j].id > urunler[j + 1].id) {
        // Pointer ile adreslerini gonderiyoruz -> Call by Reference
        yer_degistir(&urunler[j], &urunler[j + 1]);
      }
    }
  }
}

// Pointer kullanarak yapilan yer degistirme fonksiyonu
// Bu fonksiyon "Call by Reference" mantigiyla calisir.
void yer_degistir(struct Urun *a, struct Urun *b) {
  struct Urun gecici;

  // a'nin gosterdigi degeri geciciye al
  gecici = *a;

  // b'nin gosterdigi degeri a'nin gosterdigi yere koy
  *a = *b;

  // gecicideki degeri b'nin gosterdigi yere koy
  *b = gecici;
}

void raporlama(void) {
  printf("\n--- Raporlama ---\n");
  printf("Toplam Kayit Sayisi: %d\n", urun_sayisi);

  char aranan_kategori[100];
  int kategori_sayaci = 0;

  printf("Sayim yapilacak kategori adi: ");
  scanf(" %[^\n]s", aranan_kategori);

  for (int i = 0; i < urun_sayisi; i++) {
    // strcmp iki string ayniysa 0 dondurur
    if (strcmp(urunler[i].kategori, aranan_kategori) == 0) {
      kategori_sayaci++;
    }
  }

  printf("'%s' kategorisindeki urun sayisi: %d\n", aranan_kategori,
         kategori_sayaci);
}

