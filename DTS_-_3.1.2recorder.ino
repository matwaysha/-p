#include <Servo.h>

#include <SD.h>

#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27); //using displey for checking

int n = 0;  //trims before flight
int m = 0;
int t = 0;
int k = 0;  //counter for recorder
int a = 90; //angles sending to servos
int b = 90;
int c = 90;
float x;    //received angles
float y;
bool Sleft; //flags for different conditions
bool Sright;
bool Mleft;
bool Mright;
bool Okroll;
bool Snose;
bool Stail;
bool Mnose;
bool Mtail;
bool so;
bool st;
bool cl = false;
bool cr = false;
bool cn = false;
bool ct = false;
const int POW_PIN = 9;//working with SD card routine
const int CS_PIN = 10;
const int Lail = 6; //working with servo routine
const int Rail = 3;
const int Elev = 5;
const int S1 = 8;
const int S2 = 7;
const int button = A0;
Servo LailS;
Servo RailS;
Servo ElevS;
void setup() {
  pinMode(S1, INPUT);
  pinMode(Lail, OUTPUT);
  pinMode(Rail, OUTPUT);
  pinMode(Elev, OUTPUT);
  Serial.begin(115200);
  lcd.begin(16, 2);      //working with lcd routine
  lcd.home(); lcd.clear();
  lcd.print("good day");
  lcd.setCursor(0, 1);
  lcd.print("gentlemen!");
  LailS.attach(Lail);
  RailS.attach(Rail);
  ElevS.attach(Elev);
   pinMode(CS_PIN, OUTPUT);//working with SD card routine
   pinMode(POW_PIN, OUTPUT);
   digitalWrite(POW_PIN, HIGH);
    if (!SD.begin(CS_PIN))  //connection checkup
{ 
Serial.println("Card Failure"); 
return; 
} 
Serial.println("Card Ready");
 File dataFilet = SD.open("logt.csv", FILE_WRITE); //making headlines
            if (dataFilet){
              dataFilet.println("timeStamp");
              dataFilet.close();
            }
            File dataFiler = SD.open("logRoll.csv", FILE_WRITE);
            if (dataFiler){
              dataFiler.println("roll angle");
              dataFiler.close();
            }
            File dataFilep = SD.open("logPitch.csv", FILE_WRITE);
            if (dataFilep){
              dataFilep.println("pitch angle");
              dataFilep.close();
            }
            File dataFilev = SD.open("logelev.csv", FILE_WRITE); //making headlines
            if (dataFilev){
              dataFilev.println("elevator servo angle");
              dataFilev.close();
            }
            File dataFilelail = SD.open("loglail.csv", FILE_WRITE);
            if (dataFilelail){
              dataFilelail.println("left aileron servo angle");
              dataFilelail.close();
            }
            File dataFilerail = SD.open("lograil.csv", FILE_WRITE);
            if (dataFilerail){
              dataFilerail.println("right aileron servo angle");
              dataFilerail.close();
            }
  delay(1000);
}

void loop() {
  so = false;
  st = false;
  bool Sleft = false;
  bool Sright = false;
  bool Mleft = false;
  bool Mright = false;
  bool Okroll = true;
  bool Snose = false;
  bool Stail = false;
  bool Mnose = false;
  bool Mtail = false;
  bool Okpitch = true;
  lcd.setBacklight(255);        //working witn lcd
  lcd.home(); lcd.clear();
  x = pulseIn(S1, HIGH);        //receiving angles
  x = 0.0978 * x - 53.0663 - 90;
  x = x * 10;
  x = x / 10;
  //delay(1);
  y = pulseIn(S2, HIGH);
  y = 0.0977 * y - 52.8977 - 90;
  y = y * 10;
  y = y / 10;
  /*lcd.setCursor(0, 1);      //printing angles
  lcd.print(x);
  lcd.setCursor(8, 1);
  lcd.print(y);
  lcd.setCursor(0, 0);*/
  if ((y < -6) || (y > 6)) { //roll stabilazation
    Okroll = false;
    if ((y < -40) && (!so)) {
      Sleft = true;
      a += 6;
      b -= 6;
      cl = false;
      cr = false;
      so = true;
    }
    if ((y < -6) && (!so)) {
      Mleft = true;
      if (!cl) {
        a = 93;
        b = 87;
        cl = true;
      }
      a += 1;
      b -= 1;
      so = true;
    }
    if ((y > 40) && (!so)) {
      Sright = true;
      a -= 6;
      b += 6;
      cl = false;
      cr = false;
      so = true;
    }
    if ((y > 6) && (!so)) {
      Mright = true;
      if (!cr) {
        a = 87;
        b = 93;
        cr = true;
      }
      a -= 1;
      b += 1;
      so = true;
    }
  }
  if ((x < -6) || (x > 6)) {  //pitch stabilazation
    Okpitch = false;
    if ((x < -40) && (!st) && (!Sleft) && (!Sright)) {
      Snose = true;
      c += 6;
      cn = false;
      ct = false;
      st = true;
    }
    if ((x < -6) && (!st) && (!Sleft) && (!Sright)) {
      Mnose = true;
      if (!cn) {
        c = 93;
        cn = true;
      }
      c += 1;
      st = true;
    }
    if ((x > 40) && (!st) && (!Sleft) && (!Sright)) {
      Stail = true;
      c -= 6;
      cn = false;
      ct = false;
      st = true;
    }
    if ((x > 6) && (!st) && (!Sleft) && (!Sright)) {
      Mtail = true;
      if (!ct) {
        c = 87;
        ct = true;
      }
      c -= 1;
      st = true;
    }
  }
  if (a + n < 40) {  //checkup for the incorrect output
    a = 40 - n;
  }
  if (b + m < 40) {
    b = 40 - m;
  }
  if (c + t < 40) {
    c = 40 - t;
  }
  if (a + n > 140) {
    a = 140 - n;
  }
  if (b + m > 140) {
    b = 140 - m;
  }
  if (c + t > 140) {
    c = 140 - t;
  }
  LailS.write(a + n); //sending commands to servos
  RailS.write(a + m); //this servo somewhy has reverse in real life
  ElevS.write(c + t);
  /*Serial.println(a);  //sending angles to the COM port
  Serial.print(" ");
  Serial.print(d);
  Serial.print(" ");
  Serial.print(c);
  Serial.print(" ");*/
  k++;
  if(k == 20){
    k = 0;
    long timeStamp = millis(); //getting timestamp
  String str3 = String(a + n); //converting int to string, cause only string can be written to the file
  String str4 = String(b + m);
  String str5 = String(c + t);
  String str1 = String(x);
            String str2 = String(y);
      File dataFilet = SD.open("logt.csv", FILE_WRITE);//writing angles and timestamp to the files
            if (dataFilet){
              dataFilet.println(timeStamp);
              dataFilet.close();
            }
            File dataFiler = SD.open("logRoll.csv", FILE_WRITE);
            if (dataFiler){
              dataFiler.println(str1);
              dataFiler.close();
            }
            File dataFilep = SD.open("logPitch.csv", FILE_WRITE);
            if (dataFilep){
              dataFilep.println(str2);
              dataFilep.close();
            }
            File dataFilev = SD.open("logelev.csv", FILE_WRITE); //writing servo's angles
            if (dataFilev){
              dataFilev.println(str5);
              dataFilev.close();
            }
            File dataFilelail = SD.open("loglail.csv", FILE_WRITE);
            if (dataFilelail){
              dataFilelail.println(str3);
              dataFilelail.close();
            }
            File dataFilerail = SD.open("lograil.csv", FILE_WRITE);
            if (dataFilerail){
              dataFilerail.println(str4);
              dataFilerail.close();
            }     
  
  }
  delay(7);
}
