// Aim: Accident detection alert system using ESP8266 and MPU6050

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>


// -------- WIFI --------

const char* ssid = "..";
const char* password = "..";


// -------- TELEGRAM --------

#define BOT_TOKEN ".."
#define CHAT_ID ".."

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);


// -------- HC-SR04 --------

#define TRIG_PIN 5
#define ECHO_PIN 18


// -------- RGB LED --------

#define RED_LED 25
#define GREEN_LED 26
#define BLUE_LED 27


// -------- BUZZER --------

#define BUZZER 14



// -------- MPU6050 --------

Adafruit_MPU6050 mpu;



long duration;
int distance;

bool accidentSent = false;



void connectWiFi() {

  WiFi.begin(ssid,password);

  Serial.print("Connecting WiFi");

  while(WiFi.status()!=WL_CONNECTED){

    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");

  Serial.println(WiFi.localIP());
}



void setup() {

  Serial.begin(115200);


  // WiFi
  connectWiFi();

  client.setInsecure();



  // Ultrasonic

  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);



  // RGB

  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);



  // Buzzer

  pinMode(BUZZER,OUTPUT);



  // MPU6050

  Wire.begin(21,22);


  if(!mpu.begin()){

    Serial.println("MPU6050 NOT FOUND");

    while(1);
  }


  Serial.println("System Ready");

}




void loop() {


  // ---------- Ultrasonic ----------


  digitalWrite(TRIG_PIN,LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN,HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN,LOW);


  duration = pulseIn(ECHO_PIN,HIGH,30000);


  distance = duration * 0.034 / 2;



  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");



  // Turn LEDs OFF

  digitalWrite(RED_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(BUZZER,LOW);



  if(distance > 100){

    digitalWrite(GREEN_LED,HIGH);

    Serial.println("SAFE");

  }


  else if(distance >=20 && distance<=100){

    digitalWrite(BLUE_LED,HIGH);

    digitalWrite(BUZZER,HIGH);

    Serial.println("COLLISION POSSIBLE");

  }


  else if(distance>0 && distance<20){

    digitalWrite(RED_LED,HIGH);

    digitalWrite(BUZZER,HIGH);

    Serial.println("DANGER");

  }



  // ---------- MPU6050 ----------


  sensors_event_t a,g,temp;

  mpu.getEvent(&a,&g,&temp);



  Serial.print("MPU X: ");
  Serial.print(a.acceleration.x);

  Serial.print(" Y: ");
  Serial.print(a.acceleration.y);

  Serial.print(" Z: ");
  Serial.println(a.acceleration.z);



  float movement =
  abs(a.acceleration.x) +
  abs(a.acceleration.y) +
  abs(a.acceleration.z);



  // Accident detection

  if(movement > 25 && accidentSent == false){


    Serial.println("ACCIDENT DETECTED");


    bot.sendMessage(
      CHAT_ID,
      "🚨 Accident detected! Sudden movement detected by MPU6050.",
      ""
    );


    accidentSent=true;

  }



  // Reset after stable

  if(movement < 12){

    accidentSent=false;

  }


  delay(500);

}
