
/*

================================================================
-------------------------Explanation Algorithm------------------

applying the "Left-Hand Rule", the actions will be reduced to one option each:

At a "Cross": Go to Left
At a "T": Go to Left
At a "Right Only": Go to Right
At a "Left Only": Go to Left
At a "Straight or Left": Go to Left
At a "Straight or Right": Go Straight
At a "Dead End": Go back ("U turn")
At the "End of Maze": Stop
We are almost there! "Be calm!"
*/
//===========================================================
//-------------------Variable Initialisation-----------------

// IR Sensor
#define leftCenterSensor   A2
#define rightCenterSensor  A3
#define CenterSensor       A1
#define leftFarSensor      A0
#define rightFarSensor     A5

// IR Sensor readings
int leftCenterReading;
int rightCenterReading;
int CenterReading;
int leftFarReading;
int rightFarReading;

int leftNudge;
int replaystage;
int rightNudge;

// Enable Pins
#define enl 9   // LEFT
#define enr 10  // RIGHT

// speed of motor
#define spd 150

#define leapTime 200

// Motors
#define leftMotor1  5
#define leftMotor2  2
#define rightMotor1 4
#define rightMotor2 3

// LED
#define led 13

//for storing path details
char path[30] = {};
int pathLength;
int readLength;

//============================================================

void setup() {
  
//=========================================
//----------------INPUT--------------------
  
  pinMode(leftFarSensor , INPUT);
  pinMode(leftCenterSensor , INPUT);
  pinMode(CenterSensor, INPUT);
  pinMode(rightCenterSensor , INPUT);
  pinMode(rightFarSensor , INPUT);
//==========================================
//---------------OUTPUT---------------------
  
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  
}
void loop(){
	readSensors();
  	//if only the middle two sensors can see the black line
	if(leftFarReading<200 && rightFarReading<200 && (leftCenterReading>200 || rightCenterReading>200)) 
	{ 
		straight();                                                                                    
	}
  	//otherwise goes to the leftHandWall method
	else
	{                                                                                              
		leftHandWall();                                                                                   
	}
}
//===========================================================
//--------------------Read Sensor Value----------------------

void readSensors()	//accepts input from sensors
{
	leftCenterReading  = analogRead(leftCenterSensor);
	rightCenterReading = analogRead(rightCenterSensor);
    CenterReading = analogRead(CenterSensor);
    leftFarReading     = analogRead(leftFarSensor);
	rightFarReading    = analogRead(rightFarSensor); 
}
//===========================================================
//----------------------Left Hand Wall Algorithm-------------

void leftHandWall(){
	if( leftFarReading<200 && rightFarReading<200)	//indicates either 3way intersection or end of maze
	{
		digitalWrite(leftMotor1, HIGH);
		digitalWrite(leftMotor2, LOW);
		digitalWrite(rightMotor1, HIGH);
		digitalWrite(rightMotor2, LOW);
		delay(leapTime);
		readSensors();

		if(leftFarReading<200 || rightFarReading<200)	//if it moves forward and still sees all black, maze is done
		{
	  		done();
		}
		if(leftFarReading>200 && rightFarReading>200)	//otherwise it turns left
		{ 
	  		turnLeft();
		}
	}
  
	if(leftFarReading<200)	//indicates that a left turn is possible
	{ 
		digitalWrite(leftMotor1, HIGH);
		digitalWrite(leftMotor2, LOW);
		digitalWrite(rightMotor1, HIGH);
		digitalWrite(rightMotor2, LOW);
		delay(leapTime);
		readSensors();
		if(leftFarReading>200 && rightFarReading>200)	//checks to make sure maze isn't done
		{
			turnLeft();
		}
		else
		{
			done();
		}
	}
   
	if(rightFarReading<200)	//indicates a right turn is possible
	{
		digitalWrite(leftMotor1, HIGH);
		digitalWrite(leftMotor2, LOW);
		digitalWrite(rightMotor1, HIGH);
		digitalWrite(rightMotor2, LOW);
		delay(10);	//CHANGED FROM 30 to 10
		readSensors();

		if(leftFarReading<200)	//left turn is also possible
		{
			delay(leapTime-30);
			readSensors();

			if(rightFarReading<200 && leftFarReading<200)//end of maze
			{
				done();
			}
			else	//make the left turn
			{
			turnLeft();
			return;
			}
		}
		delay(leapTime-30);
		readSensors();
		if(leftFarReading>200 && leftCenterReading>200 && rightCenterReading>200 && rightFarReading>200)
		{
			turnRight();
			return;
		}
		path[pathLength] = 'S';
		pathLength++;
		if(path[pathLength-2] == 'B')
		{
			shortPath();
		}
		straight();
	}
	readSensors();
	if(leftFarReading>200 && leftCenterReading>200 && rightCenterReading>200 
  	&& rightFarReading>200 && CenterReading>200)
	{
		turnAround();
	}
}
//===========================================================
//----------------------done---------------------------------
void done(){
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
  replaystage=1;
  path[pathLength]='D';
  pathLength++;
 while(analogRead(leftFarSensor)>200){
   digitalWrite(led, LOW);
   delay(150);
   digitalWrite(led, HIGH);
   delay(150);
 }
 delay(500);
  replay();
}
//===========================================================
//----------------------turnLeft-----------------------------
void turnLeft(){
  
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(1075);
 
  if(replaystage==0){
    path[pathLength]='L';
    pathLength++;
      if(path[pathLength-2]=='B'){
        shortPath();
      }
  }
}
//===========================================================
//----------------------turnRight----------------------------
void turnRight(){

 
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
    delay(1075);
  
  if(replaystage==0){
  path[pathLength]='R';
  Serial.println("r");
  pathLength++;
  Serial.print("Path length: ");
  Serial.println(pathLength);
    if(path[pathLength-2]=='B'){
      Serial.println("shortening path");
      shortPath();
    }
  }
 
}
//===========================================================
//----------------------straight-----------------------------
void straight(){
  if( analogRead(leftCenterSensor)<200){
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(1);
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
    delay(5);
    return;
  }
  if(analogRead(rightCenterSensor)<200){
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(1);
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(5);
    return;
  }
  
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(4);
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
 
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(900);
  
  path[pathLength]='B';
  pathLength++;
  straight();
 
}
//===========================================================
//----------------------shortPath----------------------------
void shortPath(){
 int shortDone=0;
  if(path[pathLength-3]=='L' && path[pathLength-1]=='R'){
    pathLength-=3;
    path[pathLength]='B';
    shortDone=1;
  }
   
  if(path[pathLength-3]=='L' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    shortDone=1;
  }
   
  if(path[pathLength-3]=='R' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='B';
    shortDone=1;
  }
  
   
  if(path[pathLength-3]=='S' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    shortDone=1;
  }
     
  if(path[pathLength-3]=='S' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='B';
    shortDone=1;
  }
    if(path[pathLength-3]=='L' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='S';
    shortDone=1;
  }
  
  path[pathLength+1]='D';
  path[pathLength+2]='D';
  pathLength++;
  
}
//===========================================================
//----------------------printPath----------------------------
void printPath(){
  Serial.println("+++++++++++++++++");
  int x;
  while(x<=pathLength){
  Serial.println(path[x]);
  x++;
  }
  Serial.println("+++++++++++++++++");
}
//===========================================================
//----------------------replay-------------------------------
void replay(){
   readSensors();
  if(leftFarReading<200 && rightFarReading<200){
    straight();
  }
  else{
    if(path[readLength]=='D'){
       digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(100);
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
      endMotion();
    }
    if(path[readLength]=='L'){
     digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(leapTime);
      turnLeft();
    }
    if(path[readLength]=='R'){
       digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(leapTime);
    turnRight();
    }
    if(path[readLength]=='S'){
       digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    delay(leapTime);
    straight();
    }
    
    readLength++;
  }
    
  replay();
  
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
