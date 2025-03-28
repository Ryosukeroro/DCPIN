//ROSのライブラリー//
#include <ros.h>
#include <geometry_msgs/Twist.h>

//Arduinoのライブラリー//
#include <MsTimer2.h>

//ピン設定//
#define ENCO_A 2
#define ENCO_B 3
#define MOTO_A 5
#define MOTO_D 6

//Encorder_count変数
volatile long int count = 0;
int Speed_count;
int rpm;
const float alpha=45;
const float pi=3.14159;
ros::NodeHandle nh;//登録

void Set_MOTOR(float value){
  if(value > 0){
    digitalWrite(MOTO_D,HIGH);
    analogWrite(MOTO_A, min(max((int)(value * 256), 0), 255));
    }
   else if(value<0){
    digitalWrite(MOTO_D,LOW);
    analogWrite(MOTO_A, min(max((int)(-value*256),0),255));
   }
  else{
    digitalWrite(MOTO_D,LOW);
    analogWrite(MOTO_A,0);
  }
}

void Read_ENCOA(){
  bool af=digitalRead(ENCO_A);
  bool b=digitalRead(ENCO_B);
  if(af==b)count++;
  else count--;
}

void Read_ENCOB(){
  bool ab=digitalRead(ENCO_A);
  bool bb=digitalRead(ENCO_B);
  if(ab != bb)count++;
  else count--;
}

void Count_Speed(){
  static int last_count= 0 ;
  Speed_count= count - last_count;
  last_count = count;
}
void setspeed(const float linear_x , const float linear_y , const float angular_z){
  static float target =0;
  static int rpm;
  rpm = (60 * (Speed_count / 0.04)) / 1024;
  target = -(cos((alpha*pi)/180)*(linear_x *60)/(2*3.14*0.0635))-(sin((alpha*pi)/180)*(linear_y *60)/(2*3.14*0.0635))+(angular_z*0.23*60)/(2*pi*0.0635);//(linear_x *60)/(2*3.14*0.0635);
 /* if(linear_x> 0) {
    target = (cos((alpha*pi)/180)*(linear_x *60)/(2*3.14*0.0635))-(sin((alpha*pi)/180)*(linear_y *60)/(2*3.14*0.0635));
  } else if (linear_x <0){
    target = (linear_x *60)/(2*3.14*0.0635);
  } else{
    target = 0;
  }*/
  float diff = (rpm - target);
  static float iterm = 0;
  float dt = 0.040;
  iterm += diff * dt;
float Kf = 0.0008;
float Kp = 0.0011;
float Ki = 0.0008;
  float value = Kf * target - Kp * diff - Ki * iterm;
  Set_MOTOR(value);
  if (target == 0 && rpm == 0)iterm *= 0.5;
}


void messageCb(const geometry_msgs::Twist & twist){
  const float linear_x = twist.linear.x;
  const float linear_y = twist.linear.y;
  const float angular_z = twist.angular.z;
  setspeed(linear_x , linear_y , angular_z);
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb);
void setup() {
  MsTimer2::set(40, Count_Speed);
  MsTimer2::start();
  bool a = digitalRead(ENCO_A);
  bool b = digitalRead(ENCO_B);
  pinMode(ENCO_A, INPUT_PULLUP);
  pinMode(ENCO_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCO_A), Read_ENCOA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCO_B), Read_ENCOB, CHANGE);
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);
  // put your setup code here, to run once:

}

void loop() {
  nh.spinOnce();
  delay(20);
  // put your main code here, to run repeatedly:

}
