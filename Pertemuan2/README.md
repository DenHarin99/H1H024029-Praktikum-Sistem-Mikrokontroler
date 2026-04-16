
## 2.5.4 Pertanyaan Praktikum: 
1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
2. Apa yang terjadi jika nilai num lebih dari 15?
3. Apakah program ini menggunakan common cathode atau common anode? Jelaskan
alasanya!
4. Modifikasi program agar tampilan berjalan dari F ke 0 dan berikan penjelasan disetiap
baris kode nya dalam bentuk README.md!

Jawab:

1. Rangkaian menggunakan Arduino Uno sebagai pengendali utama yang dihubungkan ke Seven Segment melalui resistor 220 ohm pada setiap jalur segmen (opsional). Pada konfigurasi ini tidak menggunakan resistor dan pin Common pada Seven Segment dihubungkan langsung ke 5V (VCC).
2. Jika variabel num memiliki nilai lebih dari 15, maka program akan mengalami error out of bounds atau akses memori di luar batas array. Hal ini terjadi karena array digitPattern[16][8] hanya didefinisikan untuk 16 indeks (0 sampai 15). Mengakses indeks ke-16 atau lebih akan menyebabkan mikrokontroler membaca data sampah (garbage data) dari memori, sehingga tampilan pada Seven Segment menjadi acak, tidak terdefinisi, atau program dapat mengalami crash.
3. Program ini menggunakan common Anode karena Pin common pada komponen dihubungkan ke jalur VCC/5V dan code yang digunakan menggunakan operator NOT (!) pada fungsi digitalWrite(segmentPins[i], !digitPattern[num][i]). Ini menunjukkan logika active-low, di mana segmen akan menyala hanya jika pin Arduino berlogika LOW (0) untuk menciptakan perbedaan potensial dengan VCC..
4.
```cpp
#include <Arduino.h>

// Pin mapping berdasarkan Tabel 1.4: a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Pola digit untuk Common Anode (Pola dasar active-high yang akan di-invers)
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3 
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}, // 9
  {1,1,1,0,1,1,1,0}, // A
  {0,0,1,1,1,1,1,0}, // b
  {1,0,0,1,1,1,0,0}, // C
  {0,1,1,1,1,0,1,0}, // d
  {1,0,0,1,1,1,1,0}, // E
  {1,0,0,0,1,1,1,0}  // F
};

 // Fungsi untuk mengirimkan sinyal ke Seven Segment
 //Menggunakan logika NOT (!) karena hardware terhubung ke VCC (Active Low)
 
void displayDigit(int num) {
  for(int i = 0; i < 8; i++) {
    // Memberikan sinyal LOW (0V) untuk menyalakan segmen
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

void setup() {
  // Inisialisasi semua pin segment sebagai OUTPUT 
  for(int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
}

void loop() {
  // Modifikasi: Perulangan mundur (Decrement) dari indeks 15 (F) ke 0 (0)
  for(int i = 15; i >= 0; i--) {
    displayDigit(i);  // Tampilkan karakter heksadesimal ke-i
    delay(1000);      // Jeda 1 detik untuk stabilitas visual 
  }
}
```

## 2.6.4 Pertanyaan Praktikum:
1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
2. Mengapa pada push button digunakan mode INPUT_PULLUP pada Arduino Uno?
Apa keuntungannya dibandingkan rangkaian biasa?
3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari
sisi hardware maupun software?
4. Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai
penambahan (increment) dan pengurangan (decrement) pada sistem counter dan
berikan penjelasan disetiap baris kode nya dalam bentuk README.md!


Jawab:

1. Rangkaian terdiri dari Arduino Uno sebagai unit pemroses, satu unit Seven Segment Common Anode, dan  push button. Pin Common pada display dihubungkan ke 5V. Push button dihubungkan ke Pin 2 dengan kaki lainnya menuju GND untuk mendukung mode Internal Pull-up.

2. kode INPUT_PULLUP digunakan untuk mengaktifkan resistor internal di dalam mikrokontroler yang menghubungkan pin input ke tegangan 5V (HIGH) secara default, Tanpa resistor pull-up, pin input akan berada dalam kondisi floating (mengambang), di mana gangguan elektromagnetik dapat menyebabkan nilai terbaca acak antara HIGH dan LOW.
Keuntungannya adalah Tidak memerlukan resistor eksternal pada breadboard, sehingga rangkaian lebih ringkas dan pin tetap berada pada logika HIGH saat tombol dilepas, dan hanya menjadi LOW saat tombol ditekan (menghubungkan pin ke GND).

3. Sisi Hardware:

  -Kabel jumper terputus atau tidak terpasang dengan kencang pada breadboard.
  -Resistor pembatas arus pada segmen tersebut rusak atau nilainya terlalu besar.
  -Salah satu LED di dalam unit Seven Segment telah putus (kerusakan fisik komponen)

  Sisi Software:

  -Kesalahan pemetaan pin pada variabel segmentPins, sehingga data dikirim ke pin yang salah.
  -Kesalahan pada pola biner di array digitPattern (terdapat angka 0 yang seharusnya 1).
  -Pin terkait belum dikonfigurasi sebagai OUTPUT pada fungsi setup()

4. Dual Button
```cpp
#include <Arduino.h>

// Mendefinisikan pin digital Arduino yang terhubung ke segmen a, b, c, d, e, f, g, dp 
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Mendefinisikan pin untuk input tombol
const int btnUp = 2;    // Tombol untuk menambah nilai (increment)
const int btnDown = 3;  // Tombol untuk mengurangi nilai (decrement)

// Variabel untuk menyimpan angka yang sedang ditampilkan (0-15) 
int currentNumber = 0;

// Matriks pola biner untuk membentuk karakter 0-F 
// Index 0-9 untuk angka, Index 10-15 untuk huruf A-F
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // Pola angka 0
  {0,1,1,0,0,0,0,0}, // Pola angka 1
  {1,1,0,1,1,0,1,0}, // Pola angka 2
  {1,1,1,1,0,0,1,0}, // Pola angka 3 
  {0,1,1,0,0,1,1,0}, // Pola angka 4
  {1,0,1,1,0,1,1,0}, // Pola angka 5
  {1,0,1,1,1,1,1,0}, // Pola angka 6
  {1,1,1,0,0,0,0,0}, // Pola angka 7
  {1,1,1,1,1,1,1,0}, // Pola angka 8
  {1,1,1,1,0,1,1,0}, // Pola angka 9
  {1,1,1,0,1,1,1,0}, // Pola huruf A
  {0,0,1,1,1,1,1,0}, // Pola huruf b
  {1,0,0,1,1,1,0,0}, // Pola huruf C
  {0,1,1,1,1,0,1,0}, // Pola huruf d
  {1,0,0,1,1,1,1,0}, // Pola huruf E
  {1,0,0,0,1,1,1,0}  // Pola huruf F
};

  //Fungsi untuk menampilkan karakter pada Seven Segment
 //Menggunakan operator NOT (!) karena tipe Common Anode memerlukan logika Active Low (0 untuk ON) 
 
void displayDigit(int num)
{
  for(int i=0; i<8; i++)
  {
    // Mengirim sinyal LOW ke pin untuk menyalakan LED segmen 
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

void setup()
{
  // Mengatur semua pin segment sebagai OUTPUT 
  for(int i=0; i<8; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Mengatur pin tombol sebagai INPUT_PULLUP 
  // Internal pull-up memastikan pin bernilai HIGH saat dilepas dan LOW saat ditekan 
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  
  // Menampilkan angka awal (0) saat sistem pertama kali menyala 
  displayDigit(currentNumber);
}

void loop()
{
  // Membaca kondisi tombol penambah nilai 
  if(digitalRead(btnUp) == LOW) // Jika tombol terhubung ke GND (ditekan) 
  {
    currentNumber++; // Menambah nilai variabel counter
    
    // Jika nilai melebihi 15 (F), maka reset kembali ke 0 
    if(currentNumber > 15) currentNumber = 0; 
    
    displayDigit(currentNumber); // Memperbarui tampilan segment
    delay(200); // Debounce sederhana untuk mencegah pembacaan ganda akibat getaran mekanis 
  }

  // Membaca kondisi tombol pengurang nilai
  if(digitalRead(btnDown) == LOW) // Jika tombol ditekan
  {
    currentNumber--; // Mengurangi nilai variabel counter
    
    // Jika nilai kurang dari 0, maka melompat ke nilai tertinggi yaitu 15 (F)
    if(currentNumber < 0) currentNumber = 15;
    
    displayDigit(currentNumber); // Memperbarui tampilan segment
    delay(200); // Debounce sederhana
  }
}
```
