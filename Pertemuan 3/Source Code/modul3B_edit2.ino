#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <IRremote.h> // Library untuk Remote

// Inisialisasi LCD dan IR Receiver
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int PIN_IR = 2; 
const int pinPot = A0;

int nilaiAktif = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  // Memulai receiver IR
  IrReceiver.begin(PIN_IR, ENABLE_LED_FEEDBACK);
  
  Serial.println("Sistem Siap! Kontrol via Potensio, Serial, atau Remote.");
}

void loop() {
  // 1. Cek Input Remote IR
  if (IrReceiver.decode()) {
    // Contoh logika: tombol '+' menambah nilai, tombol '-' mengurangi nilai
    // Kamu perlu cek kode HEX remote kamu di Serial Monitor terlebih dahulu
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("Kode Remote: ");
    Serial.println(code, HEX);

    // Ganti kode di bawah ini dengan kode tombol remote milikmu
    if (code == 0xFFA857) { // Contoh kode tombol VOLUME UP
      nilaiAktif += 50;
    } else if (code == 0xFFE01F) { // Contoh kode tombol VOLUME DOWN
      nilaiAktif -= 50;
    }
    
    nilaiAktif = constrain(nilaiAktif, 0, 1023);
    IrReceiver.resume(); // Terima nilai berikutnya
  }

  // 2. Cek Input UART (Serial)
  if (Serial.available() > 0) {
    nilaiAktif = Serial.parseInt();
    nilaiAktif = constrain(nilaiAktif, 0, 1023);
  }

  // 3. Cek Potensiometer (Hanya update jika diputar)
  int bacaPot = analogRead(pinPot);
  if (abs(bacaPot - nilaiAktif) > 10) {
    nilaiAktif = bacaPot;
  }

  // --- Tampilan LCD & Serial (Sama seperti sebelumnya) ---
  float volt = (nilaiAktif / 1023.0) * 5.0;
  int persen = map(nilaiAktif, 0, 1023, 0, 100);
  int panjangBar = map(nilaiAktif, 0, 1023, 0, 16);

  lcd.setCursor(0, 0);
  lcd.print("ADC:"); lcd.print(nilaiAktif);
  lcd.print(" "); lcd.print(volt, 1); lcd.print("V ");
  lcd.print(persen); lcd.print("%   ");

  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) lcd.print((char)255);
    else lcd.print(" ");
  }

  delay(100);
}