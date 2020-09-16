/*===================================================================
 * 핀연결 
 * vin - vin
 * out - 디지털핀2
 * ground - ground
 =====================================================================*/

int PIR = 2; //PIR센서 디지털2핀 인풋

void setup(){
  Serial.begin(9600); //시리얼통신 보드레이트9600
  pinMode(PIR,INPUT); //디지털2핀(PIR센서out) 인풋 설정
  }

void loop(){
  if(digitalRead(PIR) == HIGH){
    Serial.println("HIGH"); // 사람 있으면 high 출력
    }
    else{
      Serial.println(""); //사람 없으면 블랭크 출력
      }

    delay(100); //1초 지연
  }
