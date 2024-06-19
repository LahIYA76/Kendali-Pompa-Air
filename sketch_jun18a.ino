// Blynk info
#define BLYNK_TEMPLATE_ID "TMPL6Id2wNhiZ"
#define BLYNK_TEMPLATE_NAME "Sistem Tandon Otomatis"
#define BLYNK_AUTH_TOKEN "-WSPeYTqIBKzVMqRmtanOxFI-5RWySK9"
//Ini adalah informasi yang diperlukan untuk menghubungkan ke platform Blynk. Blynk adalah platform IoT yang memungkinkan Anda mengontrol perangkat Anda dari jarak jauh.

// Const
const int trigPin = D1; // GPIO2
const int echoPin = D2; // GPIO0
const int relay = D3; // GPIO5
const int buzzer = D4; // GPIO4
int switchRelay = 1;
//trigPin, echoPin, relay, buzzer: Mendefinisikan pin GPIO yang digunakan untuk sensor ultrasonik, relay, dan buzzer.
//switchRelay: Variabel yang menyimpan status relay

// WiFi info
char ssid[] = "Putra";
char pass[] = "uuuuuuuu";

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// ESP8266WiFi.h: Library untuk menghubungkan ke WiFi menggunakan ESP8266.
// Library untuk menghubungkan ke platform Blynk menggunakan ESP8266.  

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  for (int i = 0; i < 7; i++) {
    Serial.print(".");
    delay(250);
  }
  normalBuzz();

  // Connect to WiFi
  // WiFi.begin(ssid, pass);
  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.print(".");
  //   delay(250);
  // }
  // doubleBuzz();
}

void loop() {
  Blynk.run();
  Serial.println(getStatusAir());
  
  // Check water level
  if (getStatusAir() < 10) {  //fungsi untuk mendapatkan status air dari sensor ultrasonik
    // Turn off relay
    digitalWrite(relay, LOW);
  } else {
    // Check switch relay
    if (switchRelay == 1) {  //Mengontrol status relay berdasarkan level air dan status switch rela
      // Turn on relay if switch is off
      digitalWrite(relay, LOW);
    } else {
      // Turn off relay
      digitalWrite(relay, HIGH);
    }
  }
  delay(1000);
}

int getStatusAir() {
  long duration, distance_cm;
  
//Mengirim Sinyal Trigger: Mengirimkan sinyal trigger ke sensor ultrasonik.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo pulse
  duration = pulseIn(echoPin, HIGH); //Mengukur Durasi Echo: Mengukur durasi sinyal echo kembali.
  distance_cm = duration * 0.0343 / 2; //Menghitung Jarak: Menghitung jarak berdasarkan durasi sinyal echo.
  
  Blynk.virtualWrite(V0, distance_cm); //Mengirim Level Air ke Blynk: Mengirimkan data jarak ke Blynk.
  
  return int(distance_cm);
}

void doubleBuzz() { //mengaktifkan buzzer dengan pola suara berbeda.
  tone(buzzer, 1200, 150);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 1200, 150);
  delay(100);
  noTone(buzzer);
}

void normalBuzz() {
  tone(buzzer, 1200, 200);
  delay(100);
  noTone(buzzer);
}

//dijalankan ketika ada perubahan nilai pada virtual pin V1 di Blynk. Mengubah status switchRelay dan memanggil doubleBuzz()
BLYNK_WRITE(V1) {
  switchRelay = param.asInt();
  doubleBuzz();
}