// Include Library Required
#include <Servo.h>
#include <LiquidCrystal_PCF8574.h>
#include <SoftwareSerial.h>
#include "Wire.h"
// DESCRIPTION VARIABLE Required
#define LED_RED_ORGANIK 2
#define LED_GREEN_ORGANIK 3
#define LED_RED_UNORGANIK 4
#define LED_GREEN_UNORGANIK 5
#define TRIG_PIN_ORGANIK 8
#define ECHO_PIN_ORGANIK 9
#define TRIG_PIN_UNORGANIK 10
#define ECHO_PIN_UNORGANIK 11
const int BUZZER_ORGANIK = 6;
const int BUZZER_UNORGANIK = 7;
const int Servo_Open = 180;
const int Servo_Close = 0;
unsigned int Pro_IND = 2;
unsigned int Pro_KAP = 3;
unsigned int INFRA = 4;
unsigned long duration_organik;
unsigned long cm_organik;
unsigned long duration_unorganik;
unsigned long cm_unorganik;
String data;

// Initialize VARIABLE Required
Servo servoOrganik;
Servo servoUnorganik;
LiquidCrystal_PCF8574 lcd(0x27, 16, 2, 4, 5, 6, 7, 8, 9); 
SoftwareSerial nodemcu('D2','D1'); //RX TX

// Variable untuk menyimpan waktu saat iram terakhir
unsigned long previousMillis = 0;

//setup
void setup() {
  // Setup Serial
  Serial.begin(115200);
 
  // PIN MODE
  pinMode(BUZZER_ORGANIK, OUTPUT);
  pinMode(BUZZER_UNORGANIK, OUTPUT);
  pinMode(LED_RED_ORGANIK, OUTPUT);
  pinMode(LED_GREEN_ORGANIK, OUTPUT);
  pinMode(LED_RED_UNORGANIK, OUTPUT);
  pinMode(LED_GREEN_UNORGANIK, OUTPUT);
  pinMode(TRIG_PIN_ORGANIK, OUTPUT);
  pinMode(ECHO_PIN_ORGANIK, INPUT);
  pinMode(TRIG_PIN_UNORGANIK, OUTPUT);
  pinMode(ECHO_PIN_UNORGANIK, INPUT);
  pinMode(Pro_IND, INPUT);
  pinMode(Pro_KAP, INPUT);
  pinMode(INFRA, INPUT);
  
  // RESET
  digitalWrite(LED_RED_ORGANIK, LOW);
  digitalWrite(LED_GREEN_ORGANIK, LOW);
  digitalWrite(LED_RED_UNORGANIK, LOW);
  digitalWrite(LED_GREEN_UNORGANIK, LOW);
  servoOrganik.attach(12);
  servoOrganik.write(Servo_Close);
  servoUnorganik.attach(13);
  servoUnorganik.write(Servo_Close);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.write(254);
  lcd.write(12); // turn on backlight
  lcd.write(254);
  lcd.write(12); // turn off backlight
  
  // Delay
  delay(10);
}

// START SYSTEMS (LCD)
void welcome() {
  Serial.print("Sistem Pemilah Sampah Aktif");
  Serial.println();
  lcd.setCursor(0, 0);
  lcd.print(" Sistem Pemilah");
  lcd.setCursor(0, 1);
  lcd.print("     Sampah Otomatis    ");
}
 
  // Fungsi untuk mengirim notifikasi WhatsApp dan mengubah teks pada LCD
void sendNotificationAndUpdateLCD(unsigned long cm, const char* sampahType) {
  // Konversi cm ke centimeter
  String cmString = String(cm) + " cm";

  // Tampilkan teks pada LCD
  lcd.setCursor(0, 0);
  lcd.print("Tinggi Sampah");
  lcd.setCursor(0, 1);
  lcd.print(sampahType);
  lcd.print(" ");
  lcd.print(cmString);

  // Jika sampah penuh atau kosong, kirim notifikasi WhatsApp
  if (cm == 0 || cm >= 26) {
  }
}

// Fungsi cek_tinggi_sampah_organik()
void cek_tinggi_sampah_organik() {
  // ...

  // Hitung tinggi sampah
  unsigned long cm_organik = (duration_organik / 2) / 29.1;

  // Panggil fungsi sendNotificationAndUpdateLCD()
  sendNotificationAndUpdateLCD(cm_organik, "Organik");
}

// Fungsi cek_tinggi_sampah_unorganik()
void cek_tinggi_sampah_unorganik() {
  // ...

  // Hitung tinggi sampah
  unsigned long cm_unorganik = (duration_unorganik / 2) / 29.1;

  // Panggil fungsi sendNotificationAndUpdateLCD()
  sendNotificationAndUpdateLCD(cm_unorganik, "Unorganik");
}
void loop() {
  int sensor_Inductive = digitalRead(Pro_IND);
  int sensor_Capacitive = digitalRead(Pro_KAP);
  int sensor_Infrared = digitalRead(INFRA);

  if (sensor_Capacitive == 0 && sensor_Inductive == 1 && sensor_Infrared == 1) {
    // Welcome and check heights
    welcome();
    delay(1500);
    lcd.clear();
    cek_tinggi_sampah_organik();
    delay(1500);
    lcd.clear();
    cek_tinggi_sampah_unorganik();
    delay(1500);
    lcd.clear();
  } else if (sensor_Inductive == 0 && sensor_Capacitive == 1 && sensor_Infrared == 0) {
    // Organic waste detected
    lcd.setCursor(0, 0);
    lcd.print("Mendeteksi");
    lcd.setCursor(0, 1);
    lcd.print("Sampah Organik");
    delay(1000);
    Serial.println("Sampah Organik Terdeteksi");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sampah");
    lcd.setCursor(0, 1);
    lcd.print("Organik");
    servoOrganik.write(Servo_Open);
    delay(3000);
    servoOrganik.write(Servo_Close);
    delay(500);
    lcd.clear();
  } else if (sensor_Inductive == 1 && sensor_Capacitive == 0 && sensor_Infrared == 1) {
    // Inorganic waste detected
    lcd.setCursor(0, 0);
    lcd.print("Mendeteksi");
    lcd.setCursor(0, 1);
    lcd.print("Sampah UnOrganik");
    delay(1000);
    Serial.println("Sampah UnOrganik Terdeteksi");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sampah");
    lcd.setCursor(0, 1);
    lcd.print("UnOrganik");
    servoUnorganik.write(Servo_Open);
    delay(3000);
    servoUnorganik.write(Servo_Close);
    delay(500);
    lcd.clear();
  } else if (sensor_Infrared == 0) {
    // Infrared sensor alone condition (general detection fallback)
    lcd.setCursor(0, 0);
    lcd.print("Mendeteksi");
    lcd.setCursor(0, 1);
    lcd.print("Sampah");
    delay(1000);
    Serial.println("Sampah UnOrganik Terdeteksi");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sampah");
    lcd.setCursor(0, 1);
    lcd.print("UnOrganik");
    servoUnorganik.write(Servo_Open);
    delay(3000);
    servoUnorganik.write(Servo_Close);
    delay(500);
    lcd.clear();
  } else {
    // Default case, check waste heights
    welcome();
    delay(3000);
    lcd.clear();
    cek_tinggi_sampah_organik();
    delay(3000);
    lcd.clear();
    cek_tinggi_sampah_unorganik();
    delay(3000);
    lcd.clear();
  }
}
