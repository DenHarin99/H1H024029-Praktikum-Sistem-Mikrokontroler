3.5.4 Pertanyaan Praktikum
1) Jelaskan proses dari input keyboard hingga LED menyala/mati! 
2) Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika 
baris tersebut dihilangkan? 
3) Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi 
jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan 
berikan penjelasan disetiap baris kode nya dalam bentuk README.md! 
4) Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap 
sistem

Jawaban :
1. Proses dari Input Keyboard hingga LED Menyala/MatiProses ini dimulai ketika pengguna mengetikkan karakter pada Serial Monitor di komputer. Karakter tersebut dikirimkan dalam bentuk bit serial melalui kabel USB menuju Arduino menggunakan protokol UART (Universal Asynchronous Receiver-Transmitter). Setelah data sampai di Arduino, data tersebut disimpan sementara di dalam buffer serial. Program Arduino kemudian memeriksa keberadaan data tersebut menggunakan fungsi Serial.available(), lalu membacanya dengan fungsi Serial.read(). Karakter yang terbaca (misalnya '1' atau '0') dibandingkan di dalam struktur logika if-else. Jika karakter adalah '1', Arduino mengirimkan sinyal tegangan tinggi (HIGH) ke Pin 8 yang mengakibatkan LED menyala, dan jika '0', Arduino mengirimkan sinyal rendah (LOW) sehingga LED mati.
2. Fungsi Serial.available() dan Dampak Jika DihilangkanFungsi Serial.available() digunakan untuk mengecek apakah ada data yang masuk dari komputer dan tersimpan di dalam buffer serial. Hal ini penting karena komunikasi serial bersifat asinkron, sehingga Arduino tidak tahu kapan tepatnya data akan datang. Jika baris ini dihilangkan, fungsi Serial.read() akan terus dijalankan pada setiap perulangan loop(), meskipun tidak ada data yang dikirimkan. Akibatnya, Serial.read() akan menghasilkan nilai -1 secara terus-menerus, yang dapat mengganggu logika program dan menyebabkan respon sistem menjadi tidak stabil atau memberikan output kesalahan yang berulang secara tidak perlu
3. Modifikasi Program
```cpp
#include <Arduino.h>

const int PIN_LED = 8; // Sesuai tabel wiring modul 
char mode = '0';       // Variabel untuk menyimpan status mode saat ini

void setup() {
  Serial.begin(9600);
  Serial.println("Ketik '1': ON, '0': OFF, '2': Blink");
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  // Mengecek apakah ada input baru dari Serial Monitor
  if (Serial.available() > 0) {
    char dataInput = Serial.read();
    
    // Filter karakter newline/enter agar tidak mengubah mode
    if (dataInput != '\n' && dataInput != '\r') {
      mode = dataInput;
      if (mode == '1') {
        digitalWrite(PIN_LED, HIGH);
        Serial.println("Mode: LED ON");
      } else if (mode == '0') {
        digitalWrite(PIN_LED, LOW);
        Serial.println("Mode: LED OFF");
      } else if (mode == '2') {
        Serial.println("Mode: LED BLINKING");
      } else {
        Serial.println("Perintah tidak dikenal");
      }
    }
  }

  // Menjalankan perintah secara berkelanjutan berdasarkan 'mode' terakhir
  if (mode == '2') {
    digitalWrite(PIN_LED, HIGH);
    delay(500); 
    digitalWrite(PIN_LED, LOW);
    delay(500);
  }
}
```
4. Untuk sistem sederhana yang hanya menjalankan satu tugas seperti pada percobaan 3A, penggunaan delay() masih dapat diterima. Namun, dalam pengembangan sistem yang lebih kompleks, sangat disarankan menggunakan millis().

-Pengaruh delay(): Fungsi ini bersifat blocking, artinya mikrokontroler akan berhenti total dan tidak bisa melakukan tugas lain (termasuk membaca input serial baru) selama durasi jeda berlangsung. Ini membuat sistem menjadi tidak responsif saat LED sedang dalam fase tunggu blink.

-Pengaruh millis(): Fungsi ini bersifat non-blocking. Mikrokontroler tetap dapat menjalankan baris kode lainnya dan mendeteksi input serial kapan saja sambil memantau waktu untuk mengedipkan LED. Penggunaan millis() membuat sistem jauh lebih efisien dan responsif terhadap perintah pengguna.

3.6.4 
Pertanyaan Praktikum 
1) Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian 
tersebut! 
2) Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan 
pin kanan tertukar! 
3) Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai 
output) sehingga: - - - - 
Data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor 
Volt: 0.00 V 
Adapun data yang ditampilkan pada Serial Monitor sesuai dengan table berikut: 
ADC: 0 
Tampilan jika potensiometer dalam kondisi diputar paling kiri 
Persen: 0% 
ADC: 0  0% | setCursor(0, 0) dan Bar (level) | setCursor(0, 1) 
Berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
4. Lengkapi table berikut berdasarkan pengamatan pada Serial Monitor
| ADC | Volt (V) | Persen (%) |
| :---: | :---: | :---: |
| 1 | | |
| 21 | | |
| 49 | | |
| 74 | | |
| 96 | | |

Jawab:
1. Komunikasi I2C pada rangkaian ini bekerja menggunakan prinsip Master-Slave, di mana Arduino Uno bertindak sebagai Master dan modul LCD I2C bertindak sebagai Slave. Master mengontrol seluruh pertukaran data melalui dua jalur kabel utama: SDA (Serial Data) untuk mengirim data dan SCL (Serial Clock) untuk menyinkronkan waktu pengiriman bit. Agar data sampai ke perangkat yang tepat, Arduino mengirimkan I2C Address unik (seperti 0x27) ke jalur bus; hanya LCD dengan alamat tersebut yang akan merespons instruksi untuk menampilkan teks atau grafik.

2. Secara fungsional, pin potensiometer memang harus terhubung ke Ground, VCC, dan Pin Analog (A0) agar dapat membentuk pembagi tegangan (voltage divider). Jika pin kiri dan pin kanan tertukar, rangkaian tetap akan berfungsi dengan aman, namun arah pembacaan nilai akan menjadi terbalik. Jika sebelumnya putaran ke kanan meningkatkan nilai ADC, setelah ditukar, putaran ke kanan justru akan menurunkan nilai ADC menuju 0 karena urutan tegangan referensi yang diterima pin tengah menjadi terbalik terhadap posisi fisik tuas.

3. Modifikasi
```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// Inisialisasi LCD pada alamat 0x27 dengan ukuran 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinPot = A0; // Pin analog untuk potensiometer

void setup() {
  Serial.begin(9600); // Memulai UART pada baud rate 9600
  lcd.init();         // Inisialisasi LCD
  lcd.backlight();    // Menyalakan lampu latar LCD
}

void loop() {
  // Membaca nilai ADC (0-1023)
  int nilaiADC = analogRead(pinPot);
  
  // Kalkulasi nilai Voltase dan Persentase
  float volt = (nilaiADC * 5.0) / 1023.0;
  int persen = map(nilaiADC, 0, 1023, 0, 100);
  int panjangBar = map(nilaiADC, 0, 1023, 0, 16);

  // Output ke Serial Monitor (UART)
  Serial.print("ADC: "); Serial.println(nilaiADC);
  Serial.print("Persen: "); Serial.print(persen); Serial.println("%");
  Serial.print("Volt: "); Serial.print(volt); Serial.println(" V");
  Serial.println("-----------------------");

  // Output ke LCD (I2C)
  // Baris 1: Informasi teks
  lcd.setCursor(0, 0);
  lcd.print("ADC:"); lcd.print(nilaiADC);
  lcd.print(" "); lcd.print(persen); lcd.print("%");
  lcd.print("      "); // Membersihkan sisa karakter

  // Baris 2: Visualisasi Bar (Level)
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255); // Karakter blok penuh
    } else {
      lcd.print(" "); // Karakter kosong
    }
  }

  delay(500); // Jeda pembaruan data
}
```
4. 
| ADC | Volt (V) | Persen (%) |
| :---: | :---: | :---: |
| 1 | 0 | 0 |
| 21 | 0.1 | 2 |
| 49 | 0.2 | 4 |
| 74 | 0.4 | 7 |
| 96 | 0.5 | 9 |
