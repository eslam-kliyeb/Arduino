// Motors
int leftMotor1 = 3;
int leftMotor2 = 2;
int rightMotor1= 5;
int rightMotor2 =4;
//-----------------
int enl = 10;
int enr = 9;
//----------------------
// IR Sensor
int rightFarSensor =    12;
int rightCenterSensor = 11;
int CenterSensor     =  8;
int leftCenterSensor =  7;
int leftFarSensor    =  6;
// IR Sensor readings
int rightFarReading;
int rightCenterReading;
int CenterReading;
int leftCenterReading;
int leftFarReading;
// speed of motor
int spdL=140;
int spdR=150;
int leapTime = 200;
//for storing path details
// LED
int led = 13;
void setup()
{
  
 //==========================================
  //---------------OUTPUT---------------------
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  //=========================================
  //----------------INPUT--------------------
  pinMode(leftFarSensor , INPUT);
  pinMode(leftCenterSensor , INPUT);
  pinMode(CenterSensor, INPUT);
  pinMode(rightCenterSensor , INPUT);
  pinMode(rightFarSensor , INPUT);
  //---------------------------------------------------
  pinMode(enl, INPUT);
  pinMode(enr, INPUT);
  //------------------------------------------------------
  analogWrite(enl,spdL);
  analogWrite(enr,spdR);

  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}
void loop(){
  readSensors();
  //if only the middle two sensors can see the black line
 /*if(rightFarReading==1&&CenterReading==0&&leftFarReading==0){
      turnRight();
  } */
  
  if(leftFarReading==1)
  {
    done();
    turnLeft();
  }
  else if(leftCenterReading==1 ||CenterReading==1 || rightCenterReading==1) 
  { 
    straight();                                                                                    
  }
  else if(rightFarReading==1)
  {
     straight();  
     delay(100);       
    if(rightFarReading==1&&leftFarReading==0)
    {
      done();
      turnRight();
    }
  }
  else if (leftCenterReading==0 ||CenterReading==0 || rightCenterReading==0 || rightFarReading==0 || leftFarReading==0)
  {
    turnAround();
  }
  else 
  {
    done();
  }
   /*
    Serial.print("rightFarSensor = ");
    Serial.println(digitalRead(rightFarSensor));
    Serial.print("rightCenterSensor = ");
    Serial.println(digitalRead(rightCenterSensor));
    Serial.print("CenterSensor = ");
    Serial.println(digitalRead(CenterSensor));
    Serial.print("leftCenterSensor = ");
    Serial.println(digitalRead(leftCenterSensor));
    Serial.print("leftFarSensor = ");
    Serial.println(digitalRead(leftFarSensor));
    delay(400);*/
}
//===========================================================
//--------------------Read Sensor Value----------------------

void readSensors()  //accepts input from sensors
{
  leftCenterReading  = digitalRead(leftCenterSensor);
  rightCenterReading = digitalRead(rightCenterSensor);
    CenterReading = digitalRead(CenterSensor);
    leftFarReading     = digitalRead(leftFarSensor);
  rightFarReading    = digitalRead(rightFarSensor); 
}
//===========================================================
//----------------------done---------------------------------
void done(){
  analogWrite(enl,spdL);
  analogWrite(enr,spdR);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
 return;
}
//===========================================================
//----------------------turnLeft-----------------------------
void turnLeft(){
  analogWrite(enl,spdL);
  analogWrite(enr,spdR);
  while(digitalRead(rightFarSensor)==1||digitalRead(leftFarSensor)==1){
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
  }
  done();
  
}
//===========================================================
//----------------------turnRight----------------------------
void turnRight(){
  analogWrite(enl,spdL);
  analogWrite(enr,spdR);
 while(digitalRead(rightFarSensor)==1||digitalRead(leftFarSensor)==1){
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
 }
    done();
}
//===========================================================
//----------------------straight-----------------------------
void straight(){
  if( digitalRead(leftCenterSensor)==1){
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(1);
    //-------------------------
    analogWrite(enl,spdL-10);
    analogWrite(enr,spdR);
    //-------------------------
    digitalWrite(leftMotor1, 1);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, 1);
    digitalWrite(rightMotor2, LOW);
    delay(8);
    return;
  }
  if(digitalRead(rightCenterSensor)==1){
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(1);
    analogWrite(enl,spdL);
    analogWrite(enr,spdR-70);
    digitalWrite(leftMotor1, 1);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, 1);
    digitalWrite(rightMotor2, LOW);
    delay(3);
    return;
  }
  analogWrite(enl,spdL);
  analogWrite(enr,spdR);
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(3);
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
    delay(1);
  
}
//===========================================================
//----------------------turnAround---------------------------
void turnAround()
{
 
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
 
}
//===========================================================
//----------------------endMotion----------------------------
void endMotion(){
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(200);
      digitalWrite(led, LOW);
    delay(200);
    digitalWrite(led, HIGH);
    delay(500);
  endMotion();
}
