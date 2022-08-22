#include <SoftwareSerial.h>


#define BT_RXD 8
#define BT_TXD 7
SoftwareSerial BT(BT_RXD, BT_TXD);

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
  BT.begin(9600);
  Serial.begin(9600);
  Serial.write("setup done\n");
}

void loop() {
  Serial.flush();
  // �⑥닔瑜� �ъ슜�섏뿬 �꾩쭊,�꾩쭊,醫뚰쉶��,�고쉶��,�뺤�
  if (BT.available()) {
    char c = BT.read();
//    Serial.write("%c\n",c);
    if (c == 'f')forward1();
    if (c == 'b')backward1();
    if (c == 'l')turnleft();
    if (c == 'r')turnright();
    if (c == 's')stopall();
  }
}
