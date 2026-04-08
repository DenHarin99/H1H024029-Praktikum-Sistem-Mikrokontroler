
1.5.4 Pertanyaan Praktikum: 
1. Pada kondisi apa program masuk ke blok if? 
2. Pada kondisi apa program masuk ke blok else? 
3. Apa fungsi dari perintah delay(timeDelay)? 
4. Jika program yang dibuat memiliki alur mati → lambat → cepat → reset (mati), 
ubah menjadi LED tidak langsung reset → tetapi berubah dari cepat → sedang → 
mati dan berikan penjelasan disetiap baris kode

Jawab:

1. Program akan masuk ke dalam blok if ketika nilai variabel timeDelay sudah mencapai angka 100 atau kurang dari 100 (timeDelay <= 100). Kondisi ini terpenuhi setelah LED berkedip sebanyak 10 kali (karena pengurangan 100ms setiap siklus dari nilai awal 1000ms).
2. Program akan masuk ke dalam blok else selama nilai variabel timeDelay masih berada di atas 100. Dalam kondisi ini, sistem akan terus mengurangi nilai timeDelay sebanyak 100ms setiap kali satu siklus kedip selesai dilakukan.
3. Perintah ini berfungsi untuk memberikan jeda waktu (interupsi sementara) pada eksekusi program dalam satuan milidetik. Dalam konteks rangkaian LED, perintah ini menentukan durasi berapa lama LED bertahan dalam kondisi menyala (HIGH) atau mati (LOW) sebelum berpindah ke instruksi selanjutnya.
4. Agar LED tidak langsung reset melainkan melambat terlebih dahulu, kita perlu menambahkan logika penambah nilai delay (kebalikan dari pengurangan). Berikut adalah modifikasi kodenya:
void loop() {
  digitalWrite(ledPin, HIGH); // Nyalakan LED
  delay(timeDelay);            // Jeda sesuai nilai timeDelay
  digitalWrite(ledPin, LOW);  // Matikan LED
  delay(timeDelay);            // Jeda sesuai nilai timeDelay

  static bool melambat = false; // Penanda apakah sedang fase memperlambat

  if (melambat == false) {
    timeDelay -= 100;           // Kurangi delay agar kedipan semakin cepat
    if (timeDelay <= 100) {     // Jika sudah paling cepat (100ms)
      melambat = true;          // Aktifkan mode melambat
    }
  } 
  else {
    timeDelay += 100;           // Tambah delay agar kedipan melambat (sedang)
    if (timeDelay >= 1000) {    // Jika sudah kembali ke 1000ms (paling lambat)
      delay(3000);              // Jeda 3 detik sebelum mati/ulang
      timeDelay = 1000;         // Reset nilai awal
      melambat = false;         // Kembali ke mode mempercepat
    }
  }
}
Penjelasan Logika:

static bool melambat: Digunakan sebagai "saklar" logika agar program tahu kapan harus mengurangi delay (cepat) dan kapan harus menambahnya (sedang/lambat).

timeDelay += 100:  kode ini secara bertahap menaikkan durasi jeda sehingga transisi dari cepat ke sedang terlihat halus oleh mata.

if (timeDelay >= 1000): Menjadi titik henti untuk memastikan siklus kembali ke kondisi paling lambat sebelum memulai urutan dari awal lagi.


1.6.4 Pertanyaan Praktikum:
1. Gambarkan rangkaian schematic 5 LED running yang digunakan pada percobaan! 
2. Jelaskan bagaimana program membuat efek LED berjalan dari kiri ke kanan! 
3. Jelaskan bagaimana program membuat LED kembali dari kanan ke kiri! 
4. Buatkan program agar LED menyala tiga LED kanan dan tiga LED kiri secara bergantian 
dan berikan penjelasan disetiap baris kode nya

Jawab:

1. Rangkaian terdiri dari Arduino Uno sebagai pusat kendali, di mana Pin Digital 2, 3, 4, 5, 6, dan 7 masing-masing dihubungkan ke kaki Anoda (+) dari 6 buah LED. Setiap kaki Katoda (-) LED dihubungkan ke ground (GND) melalui resistor pembatas arus sebesar 220 Ohm untuk mencegah kerusakan pada komponen LED tersebut.

2. Efek LED berjalan dari kiri ke kanan (pin rendah ke tinggi) dihasilkan oleh struktur perulangan for dengan operator increment (++). Program memulai iterasi dari variabel ledPin = 2 dan terus bertambah hingga < 8. Di dalam setiap tahapan perulangan, pin yang aktif akan diberikan perintah digitalWrite(HIGH) selama durasi variabel timer, kemudian dimatikan kembali (LOW) sebelum berpindah ke pin berikutnya.

3. Efek balik dari kanan ke kiri (pin tinggi ke rendah) dihasilkan menggunakan struktur perulangan for dengan operator decrement (--). Program memulai iterasi dari nilai ledPin = 7 dan terus berkurang selama nilai tersebut lebih besar atau sama dengan 2 (>= 2). Hal ini memaksa mikrokontroler untuk mengaktifkan pin secara terbalik, sehingga menciptakan persepsi visual lampu yang bergerak mundur ke posisi awal.

4. kode program untuk menyalakan LED kelompok kiri (Pin 2, 3, 4) dan kelompok kanan (Pin 5, 6, 7) secara bergantian:
 void setup() {
  // Gunakan loop untuk setting Pin 2 sampai 7 sebagai OUTPUT
  for (int i = 2; i < 8; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // FASE 1: Menyalakan 3 LED Kiri (Pin 2, 3, 4)
  for (int i = 2; i <= 4; i++) {
    digitalWrite(i, HIGH); // Memberikan tegangan pada pin 2, 3, dan 4
  }
  for (int i = 5; i <= 7; i++) {
    digitalWrite(i, LOW);  // Memastikan pin 5, 6, dan 7 dalam kondisi mati
  }
  delay(500);              // Jeda waktu 0.5 detik agar terlihat bergantian

  // FASE 2: Menyalakan 3 LED Kanan (Pin 5, 6, 7)
  for (int i = 2; i <= 4; i++) {
    digitalWrite(i, LOW);  // Mematikan pin 2, 3, dan 4
  }
  for (int i = 5; i <= 7; i++) {
    digitalWrite(i, HIGH); // Memberikan tegangan pada pin 5, 6, dan 7
  }
  delay(500);              // Jeda waktu 0.5 detik sebelum kembali ke FASE 1
}

Penjelasan Logika Kode:

for (int i = 2; i <= 4; i++): Perulangan ini secara spesifik hanya menyasar kelompok pin "kiri" untuk diberikan instruksi secara kolektif.

digitalWrite(i, HIGH/LOW): Digunakan untuk mengubah status logika pada sekelompok pin secara bersamaan di dalam satu siklus loop.

delay(500): Memberikan waktu tunggu yang cukup bagi mata manusia untuk menangkap perubahan kondisi antara kelompok LED kiri dan kanan.