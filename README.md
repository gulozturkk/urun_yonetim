# C Tabanlı Ürün Yönetim ve Veri Tabanı Sistemi

Atatürk Üniversitesi Mühendislik Fakültesi Yazılım Mühendisliği Bölümü kapsamında geliştirilen bu proje; C programlama dilinin temel kavramlarını (`struct`, dinamik bellek yönetimi, pointerlar, dosya I/O ve sıralama algoritmaları) kullanarak konsol üzerinden çalışan modüler bir ürün yönetim sistemidir.

## Projenin Amacı ve Kapsamı
Bu proje, statik diziler yerine dinamik bellek yapıları kullanarak performanslı ve güvenli veri yönetimi yapmayı amaçlar:
- Dinamik Bellek Yönetimi (`malloc`, `realloc`, `free`): Ürün sayısı arttıkça belleğin dinamik olarak genişletilmesi ve bellek sızıntılarının önlenmesi.
- Dinamik String Yönetimi: Bellek tasarrufu sağlamak için `strdup` ile dinamik bellek tahsisli ürün adı ve kategori yönetimi.
- Dosya Kalıcılığı (File I/O): Tüm ekleme, güncelleme ve silme işlemlerinin `veri_tabani.txt` dosyasına güvenli şekilde senkronize edilmesi.
- Algoritmik Sıralama: Kayıtlı ürünlerin ID numaralarına göre `Bubble Sort` algoritmasıyla küçükten büyüğe otomatik sıralanması.

## Kullanılan Teknolojiler ve Kütüphaneler
Projede kullanılan ana teknolojiler ve alt yapı bileşenleri:
- C Programlama Dili: Sistem mantığı ve algoritmik işlemler.
- Standart Kütüphaneler: Giriş/çıkış, bellek ve string operasyonları için `<stdio.h>`, `<stdlib.h>`, `<string.h>`.
- Veri Yapıları: `struct` (Yapılar) ve Dinamik Diziler (`Pointer Arrays`).
- Algoritmalar: Bubble Sort (Sıralama) ve Lineer Arama.

## Kurulum ve Çalıştırma
Projeyi bilgisayarınızda çalıştırmak için terminale sırasıyla şu komutları yazabilirsiniz:

1. git clone https://github.com/gulozturkk/urun_yonetim_sistemi.git
2. cd urun_yonetim_sistemi
3. gcc main.c -o urun_sistemi
4. urun_sistemi.exe (Windows için) veya ./urun_sistemi (Mac/Linux için)
