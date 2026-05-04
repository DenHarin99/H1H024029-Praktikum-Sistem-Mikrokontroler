### Percobaan Praktikum 2
1. Apa fungsi perintah analogRead() pada rangkaian praktikum ini?
2. Mengapa diperlukan fungsi map() dalam program tersebut?
3. Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023. Jelaskan program pada file README.md

## Jawaban

1. Fungsi perintah analogRead() dalam rangkaian praktikum ini adalah untuk membaca tegangan analog yang masuk dari pin wiper potensiometer dan mengubahnya menjadi nilai digital diskrit dengan rentang antara 0 hingga 1023. Secara teknis, perintah ini mengaktifkan fitur Analog to Digital Converter (ADC) 10-bit pada mikrokontroler untuk menerjemahkan variasi tegangan fisik menjadi data yang dapat diproses secara matematis oleh program guna menentukan posisi servo atau tingkat kecerahan LED.
2. fungsi map() sangat diperlukan karena terdapat perbedaan rentang skala antara data input yang dibaca oleh sensor dengan rentang operasional yang dibutuhkan oleh perangkat output. Sebagai contoh, nilai ADC dari potensiometer berada pada rentang 0–1023, sedangkan motor servo hanya dapat menerima instruksi sudut antara 0–180 derajat dan LED hanya menerima nilai PWM antara 0–255. Dengan menggunakan fungsi map(), program dapat melakukan pemetaan nilai tersebut secara proporsional sehingga setiap perubahan kecil pada putaran potensiometer menghasilkan perubahan yang selaras dan linear pada gerakan servo maupun intensitas cahaya LED.

3. Modifikasi cukup pada bagian map
```cpp
// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan LED PWM
const int potPin = A0;   
const int ledPin = 9;   

// ===================== VARIABEL =====================
// Variabel untuk menyimpan hasil pembacaan dan konversi PWM
int nilaiADC = 0;  
int pwm = 0;   

void setup() {

  // ===================== OUTPUT SETUP =====================
  // Atur pin LED sebagai output
  pinMode(ledPin,OUTPUT);

  // ===================== SERIAL MONITOR =====================
  // Aktifkan komunikasi serial untuk melihat data pembacaan
  Serial.begin(9600); 
}

void loop() {

  // ===================== PEMBACAAN SENSOR =====================
  // Baca nilai analog dari potensiometer (rentang 0–1023)
  nilaiADC = analogRead(potPin); 

  // ===================== PEMROSESAN DATA (SCALING) =====================
  // Ubah nilai ADC (0–1023) menjadi nilai PWM (0–255)
  pwm = map(nilaiADC,
            0,   
            1023,   
            50,   // isi PWM minimum
            200);  // isi PWM maksimum

  // ===================== OUTPUT PWM =====================
  // Kirim sinyal PWM ke LED (mengatur kecerahan)
  analogWrite(ledPin, pwm); 

  // ===================== MONITORING DATA =====================
  // Tampilkan data ADC dan PWM ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC); 
  Serial.print(" | PWM: ");
  Serial.println(pwm); 

  // ===================== STABILISASI SISTEM =====================
  // Delay untuk menstabilkan pembacaan dan tampilan data
  delay(50);
  ```



### Pertanyaan Praktikum Percobaan 2
1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!
2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?
3. Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200. Jelaskan program pada file README.md.

## Jawaban
1. Kecerahan LED dapat diatur menggunakan fungsi analogWrite() karena fungsi tersebut menerapkan teknik Pulse Width Modulation (PWM), di mana pin digital Arduino tidak benar-benar mengeluarkan tegangan analog murni, melainkan sinyal digital yang berpindah antara kondisi HIGH (5V) dan LOW (0V) secara sangat cepat. Pengaturan intensitas cahaya terjadi melalui manipulasi duty cycle, yaitu rasio waktu sinyal berada dalam kondisi HIGH dibandingkan dengan total waktu satu periode sinyal tersebut. Karena perpindahan kondisi ON dan OFF ini terjadi dalam frekuensi yang sangat tinggi, mata manusia tidak dapat melihat kedipan tersebut dan justru menangkapnya sebagai tingkat kecerahan yang berbeda berdasarkan rata-rata energi yang diterima oleh LED.
2. Hubungan antara nilai ADC yang berkisar antara 0–1023 dan nilai PWM antara 0–255 merupakan bentuk penskalaan linear yang didasarkan pada perbedaan resolusi bit masing-masing fitur pada Arduino Uno. Fitur ADC memiliki resolusi 10-bit yang mampu membagi input tegangan menjadi 1024 tingkatan, sedangkan fitur output PWM menggunakan resolusi 8-bit yang hanya memiliki 256 tingkatan. Secara teknis, terdapat rasio 4:1 di antara keduanya, sehingga setiap perubahan 4 satuan pada nilai ADC akan dikonversi menjadi 1 satuan pada nilai PWM agar rentang input dari potensiometer dapat diselaraskan dengan kapasitas output LED melalui fungsi pemetaan data.
3. Hanya perlu diedit pada bagian map
```cpp
#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan servo
const int potensioPin = A0;   
const int servoPin = 9;     

// ===================== VARIABEL =====================
// Variabel untuk menyimpan data ADC dan sudut servo
int pos = 0; 
int val = 0; 

void setup() {

  // Hubungkan servo ke pin yang sudah ditentukan
  myservo.attach(servoPin); 

  // Aktifkan komunikasi serial untuk monitoring
  Serial.begin(9600); 
}

void loop() {

  // ===================== PEMBACAAN ADC =====================
  // Baca nilai dari potensiometer (rentang 0–1023)
  val = analogRead(potensioPin); 

  // ===================== KONVERSI DATA =====================
  // Ubah nilai ADC menjadi sudut servo (0–180 derajat)
  pos = map(val,
             0,   
             1023,   
             30,   // isi sudut minimum servo
             150);  // isi sudut maksimum servo

  // ===================== OUTPUT SERVO =====================
  // Gerakkan servo sesuai hasil mapping
  myservo.write(pos); 

  // ===================== MONITORING DATA =====================
  // Tampilkan data ADC dan sudut servo ke Serial Monitor
  Serial.print("ADC Potensio: ");
  Serial.print(val); 

  Serial.print(" | Sudut Servo: ");
  Serial.println(pos); 

  // ===================== STABILISASI =====================
  // Delay untuk memberi waktu servo bergerak stabil
  delay(20); 
}```

