#include <Servo.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

//Piny pripojenia zariadeni
#define TEPLOMER 2
#define MOTOR1 4
#define MOTOR2 6
#define RELE 8
#define VLHKOST A0

//Vytvorenie objektov motoru
Servo motor1;
Servo motor2;  

//Vytvorenie objektov pre teplomer
OneWire oneWire(TEPLOMER); 
DallasTemperature sensors(&oneWire);
float Celsius = 0;

//SIGNALY PRE POHYB MOTORA2 - potrebne upravit podla konkretneho motora
int STOP = 88;
int FRONT = 115;
int BACK = 65;
int FRONT2 = 110;
int BACK2 = 63;
int current = FRONT;
int current2 = FRONT2;

//VLHKOST
int vlhkost = 0;
int vlhkost_0 = 0;

void setup() {
  Serial.begin(115200);
  //zapnutie kniznic pre teplomery
  sensors.begin(); 
  //Nastavenie rele pre pumpu
  pinMode(8, OUTPUT);
  //nastavenie motorov do zastavenej polohy
  motor1.attach(4);
  motor2.attach(6);
  motor1.write(STOP);
  motor2.write(STOP);
}

//Informacie o stave strechy sklenika
int poc = 0;
int poc_zat = 0;
bool otvorit = false;
bool otvorene = false;
bool zatvorit = false;

//casy otvarania a zatvarania sklenika - treba upravit podla velkosti sklennika a rychlosti motorov
int motor_otvor_t = 6;
int motor_zatvor_t = 5;
bool prva_otocka;

//Informacie o stave vlhkosti sklenika
int poc_zal = 0;
bool zalievanie = false;
bool vlhke = false;

void loop() {
  //Zisti teplotu v skleniku
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  Serial.print("TEPLOTA:");
  Serial.print(Celsius);
  Serial.print("°C\n");
   
  //Ak je teplota nad 30st celzia a sklennik je zatvoreny, otvor ho
  if(Celsius > 27 && !otvorene && !otvorit){
    Serial.print("OTVARAM\n");
    otvorit = true;
    poc = motor_otvor_t;
    prva_otocka = true;
  }
  if(otvorit){
    motor1.write(FRONT);
    motor2.write(FRONT2);
    poc -= 1;
  }
  if(otvorit && poc == 0){
    otvorit = false;
    otvorene = true;
    motor1.write(STOP);
    motor2.write(STOP);
  }

  //Ak je teplota pod 30st celzia a sklnnik je otvoreny, zatvor ho
  if(Celsius < 26.9 && otvorene && !zatvorit){
    Serial.print("ZATVARAM\n");
    zatvorit = true;
    poc_zat = motor_zatvor_t;
  }
  if(zatvorit){
    motor1.write(BACK);
    if(prva_otocka){
      delay(1000);
      prva_otocka = false;
    }
    motor2.write(BACK2);
    poc_zat -= 1;
  }
  if(zatvorit && poc_zat == 0){
    zatvorit = false;
    otvorene = false;
    motor1.write(STOP);
    motor2.write(STOP);
  }

  //Zisti vlhkost pody
  vlhkost_0 = analogRead(VLHKOST);
  //Prepocitaj na percenta
  //873 a 17 treba upravit podla vlastneho senzoru
  //873 hovori aku hodnotu vykazuje senzor pri uplne suchej pode
  //17 je hodnota pri uplne mokrej pode/ked je vo vode
  vlhkost = map(vlhkost_0,873,17,0,100);
  Serial.print("VLHKOST: ");
  Serial.print(vlhkost);
  Serial.print("\n");
  //Ak je poda malo vlhka, zalievaj ju, pokial vlhkost dostatocne nestupne
  if(vlhkost < 50){
    vlhke = false;
    //Rele posleme signal, ze sa ma zapnut
    digitalWrite(8, HIGH);
    Serial.print("Polievam, vlhkost: ");
    Serial.print(vlhkost);
    Serial.print("%\n");
  //Ak je poda uz dostatocne vlhka, rele=>pumpa sa vypne
  }else{
    vlhke = true;
    digitalWrite(8, LOW);
  }   
} 
