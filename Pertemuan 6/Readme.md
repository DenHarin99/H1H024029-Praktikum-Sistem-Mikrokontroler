## Pertanyaan 6.5.4
1. Jelaskan proses bagaimana tombol dapat mengubah kondisi LED menggunakan
interrupt!
2. Apa fungsi attachInterrupt() pada program tersebut?
3. Mengapa pada ISR tidak disarankan menggunakan delay() dan Serial.print()?
4. Apa fungsi keyword volatile pada variabel ledState?
5. Pada percobaan digunakan mode interrupt FALLING. Modifikasikan program
menggunakan mode interrupt lain (RISING, CHANGE, atau LOW) kemudian:
• Jelaskan perbedaan cara kerja masing-masing mode interrupt tersebut
• Analisis perubahan perilaku LED yang terjadi pada setiap mode
• Sertakan source code dan penjelasan program dalam bentuk README.md
### Jawab
1. Proses Tombol Mengubah Kondisi LED Menggunakan InterruptSecara sistem, proses ini terjadi melalui langkah-langkah berikut:  
<strong>Kondisi Awal:</strong> Mikrokontroler (Arduino Uno) menjalankan program utama di dalam fungsi loop() secara berurutan, yaitu terus-menerus menuliskan nilai ledState ke Pin 13.  
<strong>Pemicuan (Trigger):</strong> Saat tombol pada Pin 2 ditekan, terjadi perubahan sinyal dari bertegangan tinggi (High) ke rendah (Low) karena menggunakan konfigurasi INPUT_PULLUP. Perubahan sinyal turun ini disebut FALLING edge.  
<strong>Interupsi Program:</strong> Sinyal FALLING tersebut dideteksi oleh perangkat keras Arduino pada Pin 2 (Interrupt 0). Seketika itu juga, CPU menghentikan sementara jalannya program utama di loop().  
<strong>Eksekusi ISR:</strong> CPU beralih mengeksekusi fungsi khusus bernama Interrupt Service Routine (ISR), yaitu fungsi tombolInterrupt(). Di dalam fungsi ini, nilai variabel ledState dibalik (jika awalnya false/mati menjadi true/menyala, dan sebaliknya).  
<strong>Kembali ke Program Utama:</strong> Setelah kode di dalam ISR selesai dieksekusi, CPU kembali melanjutkan program utama yang sempat tertunda. Pada perulangan loop() berikutnya, fungsi digitalWrite(13, ledState) akan memperbarui kondisi fisik LED sesuai nilai ledState yang baru

2. Fungsi attachInterrupt() berfungsi untuk mengonfigurasi, mengaktifkan, dan menghubungkan penanda interupsi eksternal fisik dengan fungsi penanganan interupsi (ISR) tertentu di dalam kode program.Fungsi ini memberi tahu mikrokontroler agar:
    -Memantau pin digital 2 sebagai jalur interupsi eksternal.  
    -Mengaitkannya dengan fungsi ISR bernama tombolInterrupt.  
    -Menentukan bahwa interupsi hanya akan dipicu saat sinyal pada pin tersebut mengalami transisi turun atau dari tinggi ke rendah (FALLING).

3. delay() dan Serial.print() tidak disarankan dalam ISR karena fungsi ISR harus dirancang sesingkat dan secepat mungkin. Fungsi delay() pada Arduino bekerja dengan memanfaatkan perhitungan timer internal berbasis interupsi hardware. Ketika mikrokontroler sedang berada di dalam sebuah ISR, interupsi lain yang selevel atau di bawahnya akan ditangguhkan secara otomatis. Akibatnya, nilai waktu tidak akan bertambah dan program bisa mengalami freeze (macet total). Kemudian kenapa Serial.print() tidak digunakan? Karena Komunikasi serial mengandalkan pengiriman data bit-demi-bit yang lambat dan juga memanfaatkan mekanisme interupsi hardware. Menggunakannya di dalam ISR dapat menyebabkan penumpukan antrean data, memakan waktu CPU terlalu lama, serta merusak sifat real-time sistem karena menahan program utama berhenti terlalu lama.

4. Keyword volatile berfungsi untuk memberitahu compiler agar tidak melakukan optimasi memori pada variabel tersebut. Tanpa keyword volatile, compiler berasumsi bahwa variabel di dalam loop() hanya berubah melalui alur program yang normal/sekuensial. Karena perubahan variabel ledState terjadi secara mendadak di dalam fungsi ISR tombolInterrupt() (di luar kendali alur loop()), compiler yang terlalu agresif melakukan optimasi bisa saja menyimpan nilai variabel tersebut di register CPU alih-alih membacanya langsung dari RAM. Hal ini bisa menyebabkan program utama di loop() tidak menyadari bahwa nilainya sudah berubah.  Dengan menambahkan volatile, Arduino dipaksa untuk selalu membaca nilai terbaru dari RAM setiap kali variabel tersebut dipanggil. 

5. 
```cpp
# Dokumentasi Percobaan 6A: Modifikasi Mode Interrupt (CHANGE)

#include <Arduino.h>
// Variabel volatile agar dapat diubah dalam ISR
volatile bool ledState = false;
// ISR: dijalankan saat tombol ditekan (FALLING edge)
void tombolInterrupt() {
  ledState = !ledState; // Toggle status LED
}
void setup() {
 // Konfigurasi pin 13 sebagai output (LED)
  pinMode(13, OUTPUT);
 // Konfigurasi pin 2 sebagai input dengan pull-up internal
  pinMode(2, INPUT_PULLUP);
 // Daftarkan ISR pada pin 2, dipicu FALLING (tombol ditekan)
  attachInterrupt(
  digitalPinToInterrupt(2),
  tombolInterrupt,
  CHANGE
  );
}
void loop() {
 // Tulis status LED sesuai variabel ledState
  digitalWrite(13, ledState);
}
```

## Pertanyaan 6.6.4
1. Jelaskan bagaimana fungsi millis() bekerja pada program tersebut!
2. Apa perbedaan utama antara delay() dan millis()?
3. Mengapa metode millis() disebut non-blocking?
4. Modifikasi program agar:
• LED pertama berkedip setiap 1 detik
• LED kedua berkedip setiap 500 ms
• Tanpa menggunakan delay()

### Jawab
1. fungsi millis() bekerja sebagai pewaktu (timer) pasif yang melacak total waktu operasional perangkat sejak Arduino mulai dinyalakan. Proses pengecekan interval waktu di dalam fungsi loop() terjadi melalui logika berikut:
    a. Variabel currentMillis terus-menerus mengambil nilai waktu terbaru (dalam satuan milidetik) dari fungsi millis() pada setiap siklus perulangan loop().
    b. Sistem melakukan operasi matematika sederhana (currentMillis - previousMillis). Ini digunakan untuk menghitung berapa banyak waktu yang telah berlalu sejak kondisi LED terakhir kali diubah (toggle)
    c. Hasil pengurangan tersebut kemudian dibandingkan dengan variabel interval (1000 ms)
    d. Jika selisih waktu belum mencapai atau melewati 1000 ms, blok kode di dalam if akan diabaikan dan program langsung melompat ke baris berikutnya. Namun, begitu selisihnya >=1000$ ms, program akan masuk ke dalam blok if, memperbarui nilai previousMillis dengan nilai currentMillis saat itu sebagai titik acuan baru , lalu membalik status logika LED.

2. | Karakteristik     | Fungsi delay()                                           | Fungsi millis()                                                             |
|-------------------|-----------------------------------------------------------|------------------------------------------------------------------------------|
| Sifat Operasi     | Blocking (menghentikan alur program utama)               | Non-blocking (program utama tetap berjalan paralel)                          |
| Mekanisme Kerja   | Memaksa CPU idle dan menunggu sampai jeda selesai        | Mengembalikan total waktu sejak board menyala tanpa menghentikan instruksi   |
| Multitasking      | Tidak mendukung multitasking                             | Mendukung multitasking (bisa menjalankan beberapa task sekaligus)            |
| Efisiensi CPU     | Tidak efisien untuk sistem real-time                     | Sangat efisien karena CPU tetap bisa menjalankan logika lain                  |

3. Metode millis() disebut sebagai non-blocking karena tidak menyita atau mengunci waktu eksekusi CPU mikrokontroler untuk sekadar menunggu suatu interval waktu selesai.
Saat kita menggunakan millis(), kita hanya melakukan pengecekan kondisi berbasis perbandingan matematika nilai timestamp di dalam struktur pencabangan if. Jika kondisi waktu belum terpenuhi, CPU tidak dipaksa berhenti di tempat, melainkan langsung melewati blok tersebut dan melanjutkan eksekusi baris kode lain yang ada di bawahnya atau di luar struktur if tersebut. Dengan demikian, mikrokontroler tetap responsif terhadap input-input eksternal lain (seperti pembacaan sensor atau penekanan tombol) secara simultan.

4. 
```cpp
#include <Arduino.h>
unsigned long previousMillis1 = 0; // waktu terakhir LED berubah
const long interval1 = 1000; // interval kedip: 1000 ms
bool ledState1 = false; // status LED saat ini

unsigned long previousMillis2 = 0; // waktu terakhir LED berubah
const long interval2 = 500; // interval kedip: 500 ms
bool ledState2 = false; // status LED saat ini

void setup() {
  pinMode(13, OUTPUT); // Pin 13 sebagai output
  pinMode(12, OUTPUT); // Pin 12 sebagai output
}

void loop() {
 // Ambil waktu saat ini
  unsigned long currentMillis = millis();
 // Cek apakah sudah melewati interval
  if(currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis; // simpan waktu terakhir
    ledState1 = !ledState1; // toggle status LED
    digitalWrite(13, ledState1); // tulis ke pin LED
  }

  if(currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis; // simpan waktu terakhir
    ledState2 = !ledState2; // toggle status LED
    digitalWrite(12, ledState2); // tulis ke pin LED
  }
}
```