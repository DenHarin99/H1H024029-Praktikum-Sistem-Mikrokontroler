## Pertanyaan Percobaan 1
1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya! 
2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya! 
3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu 
gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan 
program pada file README.md. 

### Jawaban 
1. Ketiga task pada percobaan tersebut secara teknis berjalan secara bergantian, namun memberikan efek visual seolah-olah berjalan secara bersamaan (concurrent). Hal ini dikarenakan mikrokontroler Arduino Uno hanya memiliki satu inti prosesor (single core), sehingga Kernel Scheduler pada FreeRTOS harus membagi waktu eksekusi prosesor ke dalam potongan-potongan waktu yang sangat kecil melalui teknik time-slicing. Ketika sebuah task memanggil fungsi vTaskDelay(), scheduler secara otomatis memindahkan task tersebut ke kondisi Blocked dan segera mengalihkan sumber daya CPU untuk menjalankan task lain yang sudah siap dalam antrean eksekusi.
2. Untuk menambahkan task keempat ke dalam sistem, langkah pertama yang harus dilakukan adalah mendefinisikan prototipe fungsi task baru di bagian atas program agar dapat dikenali oleh kompiler. Selanjutnya, buatlah implementasi fungsi task tersebut yang berisi looping tak terbatas while(1) untuk menjalankan logika program secara terus-menerus. Langkah terakhir adalah mendaftarkan task tersebut di dalam fungsi void setup() menggunakan fungsi xTaskCreate(), dengan menyertakan parameter nama fungsi, nama teks task, ukuran memori stack, prioritas eksekusi, serta parameter tambahan jika diperlukan.
3. 
``` cpp
#include <Arduino_FreeRTOS.h>     // Import FreeRTOS untuk multitasking di Arduino
#include <semphr.h>               // Import semaphore (buat ngatur rebutan data)

void TaskBlink1(void *pvParameters);   // Deklarasi task kedip LED 1
void TaskBlink2(void *pvParameters);   // Deklarasi task kedip LED 2
void TaskReadPot(void *pvParameters);  // Deklarasi task pembaca potensiometer

SemaphoreHandle_t xMutex;          // Mutex dipakai agar variabel delayValue tidak dipakai barengan oleh banyak task
volatile int delayValue = 200;     // Nilai delay awal untuk kedip LED (boleh berubah)

void setup() {
  Serial.begin(9600);              // Nyalain serial monitor buat debugging
  xMutex = xSemaphoreCreateMutex(); // Bikin mutex

  // Membuat task pembaca potensiometer (prioritas 2)
  xTaskCreate(TaskReadPot, "ReadPot", 128, NULL, 2, NULL);

  // Membuat task LED pin 8 (prioritas 1)
  xTaskCreate(TaskBlink1, "Blink1", 128, NULL, 1, NULL);

  // Membuat task LED pin 7 (prioritas 1)
  xTaskCreate(TaskBlink2, "Blink2", 128, NULL, 1, NULL);

  vTaskStartScheduler();           // Start RTOS, program tidak lagi menggunakan loop()
}

void loop() {}                     // Tidak dipakai karena semua dijalankan oleh FreeRTOS

// Task 1: Membaca potensiometer lalu mengubah delay LED
void TaskReadPot(void *pvParameters) {
  while(1) {
    int potValue = analogRead(A0);        // Baca nilai pot 0-1023
    int mappedDelay = map(potValue, 0, 1023, 50, 1000); // Konversi ke delay 50-1000 ms

    // Mengunci mutex agar hanya task ini yang bisa mengubah delayValue
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
      delayValue = mappedDelay;           // Simpan delay baru
      xSemaphoreGive(xMutex);             // Lepaskan mutex
    }

    // Tampilkan nilai pot dan delay hasil mapping
    Serial.print("Pot: ");
    Serial.print(potValue);
    Serial.print(" | Delay: ");
    Serial.println(mappedDelay);

    vTaskDelay(100 / portTICK_PERIOD_MS); // Baca pot setiap 100 ms
  }
}

// Task 2: Kedip LED di pin 8 dengan delay berdasarkan potensiometer
void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);            // LED pertama
  while(1) {
    int d;
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
      d = delayValue;            // Ambil nilai delay yang terbaru
      xSemaphoreGive(xMutex);
    }

    digitalWrite(8, HIGH);       // LED ON
    vTaskDelay(d / portTICK_PERIOD_MS);
    digitalWrite(8, LOW);        // LED OFF
    vTaskDelay(d / portTICK_PERIOD_MS);
  }
}

// Task 3: Kedip LED di pin 7 dengan delay lebih lambat (1.5x dari LED 1)
void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);            // LED kedua
  while(1) {
    int d;
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
      d = delayValue * 1.5;      // Buat delay LED 2 lebih lambat
      xSemaphoreGive(xMutex);
    }

    digitalWrite(7, HIGH);
    vTaskDelay(d / portTICK_PERIOD_MS);
    digitalWrite(7, LOW);
    vTaskDelay(d / portTICK_PERIOD_MS);
  }
}
```


## Pertanyaan Percobaan 2
1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya! 
2. Apakah program ini berpotensi mengalami race condition? Jelaskan! 
3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga 
informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file 
README.md

### Jawaban
1. Kedua task pada percobaan tersebut secara teknis berjalan secara bergantian, namun terlihat seolah-olah berjalan secara bersamaan (concurrent). Hal ini dikarenakan mikrokontroler Arduino Uno hanya memiliki satu inti prosesor (single core), sehingga Kernel Scheduler pada FreeRTOS harus membagi waktu eksekusi prosesor ke dalam potongan-potongan waktu yang sangat kecil melalui teknik time-slicing. Mekanisme ini bekerja dengan cara memberikan jatah waktu singkat kepada setiap task secara bergantian dengan kecepatan tinggi, sehingga memberikan ilusi eksekusi simultan. Ketika satu task sedang menunggu data atau memanggil fungsi delay, scheduler akan segera mengalihkan sumber daya CPU untuk menjalankan task lain yang sudah siap dalam antrean.
2. Program ini memiliki potensi minimal untuk mengalami race condition karena sudah menerapkan mekanisme Queue untuk pertukaran data antar-task. Race condition sendiri adalah kondisi berbahaya di mana dua atau lebih task mencoba mengakses dan memodifikasi sumber daya atau variabel global yang sama secara bersamaan, yang dapat mengakibatkan ketidakkonsistenan data. Namun, dalam kode percobaan ini, komunikasi data dilakukan melalui fungsi xQueueSend() dan xQueueReceive(), yang merupakan fitur bawaan FreeRTOS untuk menjamin bahwa akses data dilakukan secara teratur dan aman oleh sistem. Dengan menggunakan antrean, task penerima akan menunggu hingga data tersedia secara utuh sebelum memprosesnya, sehingga integritas data tetap terjaga meskipun sistem menjalankan banyak task sekaligus.
3. 
```cpp #include <Arduino_FreeRTOS.h>   // Library FreeRTOS untuk multitasking di Arduino
#include <queue.h>              // Library untuk fitur antrean (queue)
#include <DHT22.h>              // Library sensor DHT22 (suhu & kelembapan)

#define DHT22_PIN 2             // Pin data sensor DHT22 terhubung ke pin 2

// Membuat objek DHT22 yang terhubung ke pin 2
DHT22 dht22(DHT22_PIN);

// Struktur untuk menyimpan satu paket data: suhu & kelembapan
struct readings {
  float temp;
  float hum;
};

QueueHandle_t my_queue;         // Queue untuk mengirim data antar task

void read_data(void *pvParameters);    // Task membaca sensor
void display_data(void *pvParameters); // Task menampilkan data

void setup() {

  Serial.begin(9600);           // Memulai serial monitor untuk debugging

  // Membuat queue dengan kapasitas 5 elemen,
  // setiap elemen bertipe struct readings
  my_queue = xQueueCreate(5, sizeof(struct readings));

  // Membuat task untuk membaca sensor DHT22
  xTaskCreate(
    read_data,        // fungsi task
    "Read Sensor",    // nama task
    128,              // ukuran stack
    NULL,
    1,                // prioritas (1 = normal)
    NULL
  );

  // Membuat task untuk menampilkan data
  xTaskCreate(
    display_data,
    "Display",
    128,
    NULL,
    1,
    NULL
  );

  // Menjalankan FreeRTOS scheduler
  vTaskStartScheduler();
}

void loop() {
  // Tidak dipakai, semua sudah dijalankan oleh FreeRTOS
}

// =================================================================
// TASK MEMBACA SENSOR DHT22
// =================================================================
void read_data(void *pvParameters) {

  struct readings data;   // Variabel untuk menyimpan hasil sebelum dikirim ke queue

  while (1) {

    // Membaca suhu & kelembapan dari sensor
    float temperature = dht22.getTemperature();
    float humidity    = dht22.getHumidity();

    // Mengecek apakah pembacaan berhasil
    if (dht22.getLastError() != dht22.OK) {

      Serial.print("DHT22 error: ");
      Serial.println(dht22.getLastError()); // tampilkan kode error

    } else {

      // Jika valid, masukkan ke struct
      data.temp = temperature;
      data.hum  = humidity;

      // Kirim struct ke queue (tunggu selamanya jika queue penuh)
      xQueueSend(my_queue, &data, portMAX_DELAY);
    }

    // Delay 2 detik sebelum baca lagi
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// =================================================================
// TASK MENAMPILKAN DATA DARI QUEUE
// =================================================================
void display_data(void *pvParameters) {

  struct readings data;   // Variabel penampung data hasil queue

  while (1) {

    // Menerima data dari queue (tunggu selamanya sampai ada data masuk)
    if (xQueueReceive(my_queue, &data, portMAX_DELAY) == pdPASS) {

      // Menampilkan data suhu
      Serial.print("Temperature = ");
      Serial.print(data.temp);
      Serial.println(" C");

      // Menampilkan data kelembapan
      Serial.print("Humidity = ");
      Serial.print(data.hum);
      Serial.println(" %");

      Serial.println("-------------------");
    }
  }
}```