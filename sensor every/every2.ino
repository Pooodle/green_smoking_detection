#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000;// dust 측정시간
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

int PIR = 2; //PIR센서 디지털2핀 인풋

void setup() {
  Serial.begin(9600);

  pinMode(A0, INPUT);
  
  pinMode(8, INPUT);
  starttime = millis();//get the current time;

  pinMode(2,INPUT); //디지털2핀(PIR센서out) 인풋 설정

  mp3_set_serial (Serial);  //set Serial for DFPlayer-mini mp3 module 
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (10);
}

void loop() {
  int gas = analogRead(A0);  // a라는 변수를 선언하고 아날로그 0번핀의 아날로그 값을 입력
  
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;

  if ((millis() - starttime) > sampletime_ms) //if the sampel time == 30s
  {
    ratio = lowpulseoccupancy / (sampletime_ms * 10.0); // Integer percentage 0=>100
    concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
    Serial.print("concentration = ");
    Serial.print(concentration);
    Serial.println(" pcs/0.01cf");
    Serial.println("\n");
    lowpulseoccupancy = 0;
    starttime = millis();

    Serial.print("gas = ");
    Serial.println(gas);               // gas의 값을 시리얼로 출력합니다.

    delay(1);
    
    if((concentration >300)&&(gas > 300)){
        if(digitalRead(PIR) == HIGH){
        Serial.println("HIGH HIGH"); // 사람 있으면 high 출력
        delay(1); 

        mp3_next ();
        delay (5000);
        mp3_next ();
        delay (5000);
        mp3_pause();
        }
        
        else{
        Serial.println("X"); //사람 없으면 블랭크 출력
        }
      }
  }  
}
