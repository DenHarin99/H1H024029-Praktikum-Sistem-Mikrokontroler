const int ledPins[] = {2, 3, 4, 5, 6, 7};
int timeDelay = 1000; 

int timer = 100;           
// delay. Semakin tinggi angkanya, semakin lambat waktunya. 
void setup() { 
  // gunakan loop for untuk menginisialisasi setiap pin sebagai output: 
  for (int ledPin = 2; ledPin < 8; ledPin++) { 
    pinMode(ledPin, OUTPUT); 
  } 
} 
void loop() { 
  // looping dari pin rendah ke tinggi 
  for (int ledPin = 2; ledPin < 8; ledPin++) { 
    // hidupkan LED pin nya: 
    digitalWrite(ledPin, HIGH); 
    delay(timer); 
    // matikan pin LED nya: 
    digitalWrite(ledPin, LOW); 
  } 
  // looping dari pin yang tinggi ke yang rendah 
  for (int ledPin = 7; ledPin >= 2; ledPin--) { 
    // menghidupkan pin: 
    digitalWrite(ledPin, HIGH); 
    delay(timer); 
    // mematikan pin: 
    digitalWrite(ledPin, LOW); 
  } 
} 