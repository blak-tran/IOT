#include <Servo.h>
#include <IRremote.h> // thư viện hỗ trợ IR remote
const int receiverPin = 5; // chân digital 8 dùng để đọc tín hiệu
IRrecv irrecv(receiverPin); // tạo đối tượng IRrecv mới
decode_results results;// lưu giữ kết quả giải mã tín hiệu
#define trigPin 7
#define echoPin 8

int in1 = 9;
int in2 = 10;
int in3 = 11;
int in4 = 12;
int distance_front;
int distance_left;
int distance_right;
Servo myservo;
int speed_right = 5;
int speed_left = 3;

int pos = 0;
void setup() {
  // put your setup code here, to run once:
  irrecv.enableIRIn(); // start the IR receiver
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(6);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);



}

void tien() {
  analogWrite(in1, LOW);
  digitalWrite(in2, HIGH );
  analogWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void lui() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

}
void dung() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

}

void re_trai() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(1000);
  dung();

}

void re_phai() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(1000);
  dung();
}

int cal_Distance() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
}

void servo_rotate_right() {
  myservo.write(45);
}


void servo_rotate_left() {
  myservo.write(135);
}

void servo_rotate_front() {
  myservo.write(90);
}

void translateIR()
{
  switch (results.value)
  {
    case 4294967295:
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      break;
    case 16748655: digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      break;
    case 16750695: digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);

      break;
    case 16769055: digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      break;
    case 16754775:
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      break;
    case 16720605:
      Auto_Drive_Mode();
      break;


  }
}

void Auto_Drive_Mode() {
  boolean stops = false;
  while (stops = true) {
    servo_rotate_front();
    distance_front = cal_Distance();
    Serial.print("Khoang Cach truoc mat: ");
    Serial.print(distance_front);
    Serial.println(" cm");
    delay(1000);
    servo_rotate_right();
    distance_right = cal_Distance();
    Serial.print("Khoang Cach phai: ");
    Serial.print(distance_right);
    Serial.println(" cm");
    delay(1000);
    servo_rotate_left();
    distance_left = cal_Distance();
    Serial.print("Khoang Cach trai: ");
    Serial.print(distance_left);
    Serial.println(" cm");
    if (distance_front <= 5 ) {
      if (distance_right <= 7) {
        if (distance_left <= 7) {
          lui();
          Serial.println("lui");
          delay(1000);
          dung();
        } else {
          Serial.println("re trai");
          re_trai();
        }
      } else {
        Serial.println("re phai");
        re_phai();
      }
    } else {
      Serial.println("Tien");
      tien();
      delay(1000);
      dung();
    }
    delay(2000);

    if (irrecv.decode(&results)) // nếu nhận được tín hiệu
    {
      if (results.value == 16754775){
      break;
      }
      Serial.println(results.value, DEC);
      delay(200);
      irrecv.resume(); // nhận giá trị tiếp theo
    }
  }

}



void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)) // nếu nhận được tín hiệu
  {
    translateIR();
    Serial.println(results.value, DEC);
    delay(200);
    irrecv.resume(); // nhận giá trị tiếp theo
  } else {
    dung();
  }


}
