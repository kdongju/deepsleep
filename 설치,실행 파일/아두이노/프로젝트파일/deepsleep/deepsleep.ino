#include <Stepper.h>
#include <SoftwareSerial.h>
#include <DHT.h>

SoftwareSerial btSerial(6,7); //RX,TX
DHT dht(2, DHT11); //온도센서 핀 설정
int in1Pin = 11; 
int in2Pin = 10;
int in3Pin = 9;
int in4Pin = 8; //스텝모터 핀번호 설정
const int stepsPerRevolution = 3100; //2048:한바퀴(360도), 1024:반바퀴(180도)
Stepper myStepper(stepsPerRevolution, in1Pin, in3Pin, in2Pin, in4Pin); //모터 드라이브에 연결된 핀 IN4, IN2, IN3, IN1
boolean window = true; //초기값 창문 열림
float ondo1 = 26.00; //열리는 온도값
float ondo2 = 23.00; //닫히는 온도값
int noise = 900; //기준 소음값
boolean mode = true; //자동모드선택

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(11); //모터 속도
  btSerial.begin(9600);
  dht.begin();
}
void loop(){
  Sdisplay();
  BTdisplay();
  modeselect();
  if(mode==true)
    automatic();
  if(mode==false)
    manual();
  delay(2000);
}
void modeselect(){ //모드선택
  if(btSerial.available()){
    char c = btSerial.read();
    
    switch(c){
      case'a':
        mode = true;
        break;
       case'b':
        mode = false;
        break;
}
  }
}
void automatic() { //자동모드
  float temp = dht.readTemperature();//온도센서
  int sound = analogRead(A0);//소음센서
  int rain = digitalRead(4);//빗물감지센서

  if (window == true && rain == 0) { //창문이 열려있고 비가 오는 상태
    if (temp <= ondo2 && sound > noise)
    {
      window_close();
    }
    else if (temp > ondo2 &&  sound > noise)
    {
      window_close();
    }
    else if (temp <= ondo2 &&  sound <= noise)
    {
      window_close();
    }
    else if (temp > ondo2 &&  sound <= noise)
    {
      window_close();
    }
  }
  else if (window == true && rain == 1) { //창문이 열려있고 비가 안오는 상태
    if (temp <= ondo2 && sound > noise)
    {
      window_close();
    }
    else if (temp > ondo2 &&  sound > noise)
    {
      window_close();
    }
    else if (temp <= ondo2 &&  sound <= noise)
    {
      window_close();
    }
    else if (temp > ondo2 &&  sound <= noise)
    {
      //동작안함
    }
  }
  else if (window == false && rain == 1) { //창문이 닫혀있고 비가 안오는 상태
    if (temp <= ondo1 && sound > noise)
    {
      //동작안함
    }
    else if (temp > ondo1 &&  sound > noise)
    {
      //동작안함
    }
    else if (temp <= ondo1 &&  sound <= noise)
    {
      //동작안함
    }
    else if (temp > ondo1 &&  sound <= noise)
    {
      window_open();
    }
  }
  else if (window == false && rain == 0) { //창문이 닫혀있고 비가 오는 상태
    if (temp <= ondo1 && sound > noise)
    {
      //동작안함
    }
    else if (temp > ondo1 &&  sound > noise)
    {
      //동작안함
    }
    else if (temp <= ondo1 &&  sound <= noise)
    {
      //동작안함
    }
    else if (temp > ondo1 &&  sound <= noise)
    {
      //동작안함
    }
   
}  
}

void manual() { //수동모드
  if(btSerial.available()){
    char c = btSerial.read();

    switch(c){
      case'c':
      if(window == false)
        window_open();
        break;
       case'd':
       if(window == true)
        window_close();
        break;
    } 
  }
}
void window_open() { //창문열기
 myStepper.step(-stepsPerRevolution); //모터 반시계방향 회전
 window = true;
}
void window_close() //창문닫기
{
  myStepper.step(stepsPerRevolution); //모터 시계방향 회전
  window = false;
}

void Sdisplay(){//시리얼 모니터에 센서값 표시
  float temp = dht.readTemperature();//온도센서
  int sound = analogRead(A0);//소음센서
  int rain = digitalRead(4);//빗물감지센서
  
  Serial.print("빗물감지: ");
  if(rain == 0)
     Serial.print("o");
  else
     Serial.print("x");
  Serial.print("\n");
  Serial.print("온도: ");
  Serial.print(temp);
  Serial.print("도");
  Serial.print("\n");
  Serial.print("소음: ");
  Serial.print(sound);
  Serial.print("\n");
}
void BTdisplay(){ //블루투스 모니터에 센서값 표시
  float temp = dht.readTemperature();//온도센서
  int sound = analogRead(A0);//소음센서
  int rain = digitalRead(4);//빗물감지센서
  
  btSerial.print("  rain: ");
  if(rain == 0)
     btSerial.print("o");
  else
     btSerial.print("x");
  btSerial.print("\n");
  btSerial.print("  temp: ");
  btSerial.print(temp);
  btSerial.print(" °C");
  btSerial.print("\n");
  btSerial.print("  noise: ");
  btSerial.print(sound);
  btSerial.print("\n");
}
