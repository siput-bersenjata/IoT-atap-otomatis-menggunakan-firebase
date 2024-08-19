/* 2022-12-16
   versi 1.0
   error FirebaseArduino.h karena masih json 5
   ganti ke #include <FirebaseESP8266.h> udah json 6

   versi 1.1
   ganti lib firebase = done
   tes baca tiap field = done
   tes tulis field tertentu = done

*/

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <ArduinoJson.h>
//#include <FirebaseArduino.h>
//#define fireid "fire-base-coba-default-rtdb.firebaseio.com"
//#define fireauth "SD6zBsBHk6omTBPDWgBR0aQpsbHMdThzYtv6Ohhg"

#include <FirebaseESP8266.h>
#define FIREBASE_HOST "fire-base-coba-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "fYtnPUOuR2vHpNIohgcieUeQaf5fb04qFgrnvRAq"
FirebaseData firebase_data;

#define ssid "Salshya1"
#define pas "daundaun"

//#define ssid "TOTOLINK_N200RE"
//#define pas "1234lima"

#define APIPIN 12
#define buzpin 13

int hujan = 16;
int servo = 5;
int ldr = 4;
int rely1 = 0;
int rely2 = 2;
int rely3 = 14;
int hString;
int lString;
int apiI = 0;
int apiH = 0;
int apiL = 0;
int apiS = 0;

Servo servoku;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pas);
  Serial.println("Menghubungkan...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("sedang mengkoneksikan");
    apiH = digitalRead(hujan);
    apiI = digitalRead(APIPIN);
    apiL = digitalRead(ldr);
    of_hujan();
    of_api();
    of_ldr();
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Tersambung");
  Serial.println(WiFi.localIP());

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(hujan, INPUT);
  pinMode(ldr, INPUT);
  pinMode(servo, OUTPUT);
  pinMode(rely1, OUTPUT);
  pinMode(rely2, OUTPUT);
  pinMode(rely3, OUTPUT);
  pinMode(APIPIN, INPUT);
  pinMode(buzpin, OUTPUT);
  servoku.attach(servo);

  //  Firebase.begin(fireid, fireauth);
  //2. Firebase begin
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

//int apiI = 0;
//int apiH = 0;
//int apiL = 0;
//int apiS = 0;

float oto = Firebase.getFloat(firebase_data, "otomatis");
float ser = Firebase.getFloat(firebase_data, "atap");
float ld = Firebase.getFloat(firebase_data, "awan");
float lp = Firebase.getFloat(firebase_data, "lampu");
float ck = Firebase.getFloat(firebase_data, "cek");
float sinyal = Firebase.getFloat(firebase_data, "sinyal");


void of_hujan() {
  if (apiH == 0) {
    servoku.write(180);
    delay(100);
    digitalWrite(rely1, LOW);
  }
  else {
    servoku.write(0);
    delay(100);
    digitalWrite(rely1, HIGH);
  }
}

void of_ldr() {
  if (apiL == 0) {
    digitalWrite(rely3, HIGH);
  }
  else {
    digitalWrite(rely3, LOW);
  }
}

void of_api() {
  if (apiI == 0) {
    digitalWrite(rely2, LOW);
    digitalWrite(buzpin, HIGH);
    delay(30);
    digitalWrite(rely2, HIGH);
    delay(30);
    digitalWrite(rely2, LOW);
    delay(30);
    digitalWrite(rely2, HIGH);
    delay(30);
    digitalWrite(rely2, LOW);
    delay(30);
    digitalWrite(rely2, HIGH);
    delay(100);
  }
  else {
    digitalWrite(rely2, HIGH);
    digitalWrite(buzpin, LOW);
  }
}


void loop() {

//  Serial.println("Baca nama alat:");
//  Firebase.getFloat(firebase_data, "/Nama");
//  String Nama = firebase_data.stringData();
//  Serial.print("Nama alat = "); Serial.println(Nama);
//  delay(50);


  if (!Firebase.readStream(firebase_data)) {
    //Serial.println(firebaseData.errorReason()); //If read stream was failed, print the error detail.
    Serial.println("Error Reason: " + firebase_data.errorReason());
    Serial.print("setting /number failed:");
  }
  else {
    ;
  }

  Firebase.getFloat(firebase_data, "/sinyal");
  float sinyal = firebase_data.floatData();
  Serial.print("Sinyal = "); Serial.println(sinyal);
  
 

  if (sinyal == 1 )
  { 
    Serial.println("Baca data input Sensor:");
    Firebase.getFloat(firebase_data, "/Hujan");
    float Hujan = firebase_data.floatData();
    Serial.print("Hujan = "); Serial.println(Hujan);
    delay(50);
  
    Firebase.getFloat(firebase_data, "/api");
    float api = firebase_data.floatData();
    Serial.print("api = "); Serial.println(api);
    delay(50);
  
    Firebase.getFloat(firebase_data, "/awan");
    float ld = firebase_data.floatData();
    Serial.print("awan = "); Serial.println(ld);
    delay(50);
  
    Serial.println("");
    Serial.println("Baca data status");
    Firebase.getFloat(firebase_data, "/otomatis");
    oto = firebase_data.floatData();
    Serial.print("otomatis = "); Serial.println(oto);
    delay(50);
  
    Firebase.getFloat(firebase_data, "/cek");
    float ck = firebase_data.floatData();
    Serial.print("cek = "); Serial.println(ck);
    delay(50);
  
    Firebase.getFloat(firebase_data, "/terima");
    float terima = firebase_data.floatData();
    Serial.print("terima = "); Serial.println(terima);
    delay(50);
  
    Serial.println("");  

    if (ck == 0) {
      Firebase.setInt(firebase_data, "terima", 0);
    }
    if (ck == 1) {
      Firebase.setInt(firebase_data, "terima", 1);
    }
    else {
      ;
    }
  }

  else
  {
    apiH = digitalRead(hujan);
    apiI = digitalRead(APIPIN);
    apiL = digitalRead(ldr);
    Serial.println("koneksi firebase gagal ");
    of_hujan();
    of_api();
    of_ldr();
    Serial.print("Kondisi hujan ");
    Serial.println(apiH);
    Serial.print("Kondisi cahaya ");
    Serial.println(apiL);
    Serial.print("sensor api ");
    Serial.println(apiI);
    delay(200);
    return;
  }


  apiH = digitalRead(hujan);
  apiI = digitalRead(APIPIN);
  apiL = digitalRead(ldr);
  hString = int(apiH);
  lString = int(apiL);

  atap();
  kebakaran();
  LDR();

  Serial.print("Kondisi hujan ");
  Serial.println(apiH);
  Serial.print("Kondisi cahaya ");
  Serial.println(apiL);
  Serial.print("sensor api ");
  Serial.println(apiI);
  Serial.println("");

}

void atap() {
  Serial.println("Set Hujan");
  if (oto == 0) {
    if (hString == 0) {
      digitalWrite(rely3, LOW);
      servoku.write(180);
      Firebase.setInt(firebase_data, "Hujan", 0);
    }
    else {
      digitalWrite(rely3, HIGH);
      servoku.write(0);
      Firebase.setInt(firebase_data, "Hujan", 1);
    }
  }
  else {
    servoku.write(180);
  }
}

void kebakaran() {
  Serial.println("Set api");
  if (apiI == 0) {
    digitalWrite(buzpin, HIGH);
    digitalWrite(rely2, LOW);
    delay(30);
    digitalWrite(rely2, HIGH);
    delay(30);
    digitalWrite(rely2, LOW);
    delay(30);
    digitalWrite(rely2, HIGH);
    delay(30);
    digitalWrite(rely2, LOW);
    delay(30);
    digitalWrite(rely2, HIGH);
    delay(100);
    Firebase.setInt(firebase_data, "api", 0);
  }
  else {    
    digitalWrite(buzpin, LOW);
    Firebase.setInt(firebase_data, "api", 1);
  }
}

void LDR() {
  Serial.println("Set Awan");
  if (lString == 0) {    
    Firebase.setInt(firebase_data, "awan", 1);
    digitalWrite(rely1, HIGH);
  }
  else {
    Firebase.setInt(firebase_data, "awan", 0);
    digitalWrite(rely1, LOW);
  }
}
