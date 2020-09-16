#include "SPI.h"
#include "Phpoc.h"
#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>

PhpocEmail email;
SoftwareSerial mySerial(10, 11); //mp3를 10,11에 연결

int ledPower = 2; //Connect 3 led driver pins of dust sensor to Arduino D2
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

float Vrl = 0; // 먼지센서
float Rs = 0;
float Ro = 10000;
int hsensor = 4;


void setup()
{
  Serial.begin(9600);
  Phpoc.begin(PF_LOG_SPI | PF_LOG_NET | PF_LOG_APP);
  pinMode(A0, INPUT);
  pinMode(A5, INPUT); // CO sensor
  pinMode(ledPower, OUTPUT);
  pinMode(hsensor, INPUT); // Human sensor

  //mp3
  mp3_set_serial(Serial);
 // delay(1);                     // delay 1ms to set volume
  mp3_set_volume(30);
  //mp3 part
  mp3_play(1);
  delay(1);
  mp3_pause();
}

void loop() {




  //CO sensor part

  float Hsensor = digitalRead(hsensor);
  Serial.println(Hsensor);
  //float Hsensor = analogRead(hsensor);
  float Vrl = 5.0 * analogRead(A5) / 1023; // 전압을 읽고
  float Rs = 20000 * (5.0 - Vrl) / Vrl;     // RL/(Rs+RL) = Vrl / 5.0 <-- 전압 분배 법칙의 비례식
  int ratio = 20 * Rs / Ro;
  //ratio = constrain(ratio, 0, 30);                    // min=0, max=30 범위로 제한
  Serial.print("  CO (unit ppm) :");
  Serial.println(ratio); // Rs/R0 비율을 출력

  Serial.print("  - CO Resistance value :");
  Serial.println(analogRead(A5));





  //dust sensor part

  digitalWrite(ledPower, LOW); // power on the LED
  delayMicroseconds(samplingTime);

  int voMeasured = analogRead(A0); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = 1000*0.17 * calcVoltage - 0.1;


  Serial.print("  Dust Density(unit µg/m3) :");
  Serial.println(dustDensity); // unit: µg/m3
  Serial.print("  - Raw Signal Value :");
  Serial.println(voMeasured);
 // Serial.print("\n");
  delay(1000);

  //email part
  Serial.print("HSensor : ");
  Serial.print(Hsensor);
  Serial.print("\n");

  if (Hsensor) {//voMeasured > 100
    email.setOutgoingServer("smtp.gmail.com", 587);
    email.setOutgoingLogin("jae4690@gmail.com", "비번");

    // setup From/To/Subject
    email.setFrom("jae4690@gmail.com", "jae4690");
    email.setTo("jae4690@gmail.com", "jae4690");
    email.setSubject("There is a smoker!!!!");//제목
    //문이 열렸다!
    // write email message
    email.beginMessage();
    email.println("O");
    email.println("P");
    email.println("E");
    email.println("N");
    email.endMessage();//내용


    // send email
    if (email.send() > 0)
      Serial.println("Email send ok");
    else
      Serial.println("Email send failed");


mp3_next();
delay(10000);
mp3_pause();
 
  }
  delay(1500);
