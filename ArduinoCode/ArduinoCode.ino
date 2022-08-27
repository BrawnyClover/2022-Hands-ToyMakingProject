#include <SoftwareSerial.h>
#include <Servo.h>

#define WAIST_SERVO 5
#define SHOOT_UP_SERVO 12
#define SHOOT_DOWN_SERVO 2

#define BT_RXD 7
#define BT_TXD 8

bool isShoot = false;
const int shootUpDef = 55;
const int shootDownDef = 125;
const int shootUpAngle = 90;
const int shootDownAngle = 90;

SoftwareSerial BT(BT_RXD, BT_TXD);
Servo waistServo;
Servo shootUpServo;
Servo shootDownServo;
int angle = 0;
String buffer="";
bool doProcess = false;

void forward1() {
  Serial.write("forward\n");
  digitalWrite(3, LOW);
  digitalWrite(9, HIGH); 
  digitalWrite(11, HIGH);
  digitalWrite(4, LOW);
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

void shoot(){
  Serial.write("shoot");
  
  shootUpServo.attach(SHOOT_UP_SERVO);
  shootDownServo.attach(SHOOT_DOWN_SERVO);
  
  if(isShoot == false){

    shootUpServo.write(shootUpAngle);
    shootDownServo.write(shootDownAngle);
  }
  else{
    shootUpServo.write(shootUpDef);
    shootDownServo.write(shootDownDef);
  }
  isShoot = !isShoot;
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
  waistServo.attach(WAIST_SERVO);
  shootUpServo.attach(SHOOT_UP_SERVO);
  shootDownServo.attach(SHOOT_DOWN_SERVO);
  BT.begin(9600);
  Serial.begin(9600);
  Serial.write("setup done\n");
  shootUpServo.write(shootUpDef);
  shootDownServo.write(shootDownDef);
  waistServo.write(90);

  shootUpServo.detach();
  shootDownServo.detach();
  
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
      else if (buffer[0] == 'H'){
        shoot();
              
//        shootUpServo.detach();
//        shootDownServo.detach(); 
        orderDone = true;
      }
      else if (buffer[0] == 'b'){backward1(); orderDone = true;}
      else if (buffer[0] == 'l'){turnleft(); orderDone = true;}
      else if (buffer[0] == 'r'){turnright(); orderDone = true;}
      else if (buffer[0] == 's'){stopall(); orderDone = true;}
      else if (buffer[0] == 'a'){
          int angle = (buffer[1]-'0')*100 + (buffer[2]-'0')*10 + (buffer[3]-'0');
          int strength = (buffer[4]-'0')*100 + (buffer[5]-'0')*10 + (buffer[6]-'0');
          if(strength == 0){
            angle = 90;
          }
          else{
            if(270<angle)angle = 0;
            else if(180<angle)angle = 180;
            angle = 180 - angle;
          }
          Serial.print("Servo :");
          Serial.println(angle);
          waistServo.write(angle);
          orderDone = true;
      }
      buffer = "";
    }
  }
} 
