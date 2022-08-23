#include <SoftwareSerial.h>
#include <Servo.h>

#define SERVO 5
#define BT_RXD 8
#define BT_TXD 7

SoftwareSerial BT(BT_RXD, BT_TXD);
Servo servo;
int angle = 0;
String buffer="";
bool doProcess = false;

void forward1() {
  Serial.write("forward\n");
  digitalWrite(3, LOW); //�ㅻⅨ履� �꾨옒
  digitalWrite(9, HIGH); 
  digitalWrite(11, HIGH);
  digitalWrite(4, LOW); //�쇱そ �꾨옒
}
void backward1() {
  Serial.write("backward\n");
  digitalWrite(3, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);
  digitalWrite(4, HIGH);
}
void turnleft() {
  Serial.write("left\n");
  digitalWrite(3, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(4, LOW);
}
void turnright() {
  Serial.write("right\n");
  digitalWrite(3, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(4, HIGH);
}

void stopall() {
  Serial.write("stop\n");
  digitalWrite(3, LOW);
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);
  digitalWrite(4, LOW);
}

void setup() {
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(4, OUTPUT);
  servo.attach(SERVO);
  BT.begin(9600);
  Serial.begin(9600);
  Serial.write("setup done\n");
}

void loop() {
  bool orderDone = false;
  if (BT.available()) {
    
    char c = BT.read();
    if(c == '\n') doProcess = true;
    else buffer += c;

    if(doProcess){
      doProcess = false;
//      Serial.println(buffer);
      if (buffer[0] == 'f'){forward1(); orderDone = true;    }
      else if (buffer[0] == 'b'){backward1(); orderDone = true;}
      else if (buffer[0] == 'l'){turnleft(); orderDone = true;}
      else if (buffer[0] == 'r'){turnright(); orderDone = true;}
      else if (buffer[0] == 's'){stopall(); orderDone = true;}
      else if (buffer[0] == 'a'){
          int angle = (buffer[1]-'0')*100 + (buffer[2]-'0')*10 + (buffer[3]-'0');
          if(270<angle)angle = 0;
          else if(180<angle)angle = 180;
          angle = 180 - angle;
          Serial.print("Servo :");
          Serial.println(angle);
          servo.write(angle);
          orderDone = true;
      }
      buffer = "";
    }
  }
} 
