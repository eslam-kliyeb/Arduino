
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

//for storing path details
char path[30] = {};
int pathLength;
int readLength;
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
//--------------------Shortest Path--------------------------

void shortPath()
{
	int shortDone = 0;
	if(path[pathLength-3] == 'L' && path[pathLength - 1] == 'R')
	{
		pathLength -= 3;
		path[pathLength] = 'B';
		shortDone = 1;
	}
	if(path[pathLength-3] == 'L' && path[pathLength - 1] == 'S' && shortDone == 0)
	{
		pathLength -= 3;
		path[pathLength] = 'R';
		shortDone = 1;
	}
	if(path[pathLength-3] == 'R' && path[pathLength - 1] == 'L' && shortDone == 0)
	{
		pathLength-=3;
		path[pathLength] = 'B';
		shortDone=1;
	}
	if(path[pathLength-3] == 'S' && path[pathLength - 1] == 'L' && shortDone == 0)
	{
		pathLength -= 3;
		path[pathLength] = 'R';
		shortDone = 1;
	}
	if(path[pathLength-3] == 'S' && path[pathLength - 1] =='S' && shortDone == 0)
	{
		pathLength-=3;
		path[pathLength] = 'B';
		shortDone=1;
	}
	if(path[pathLength-3] == 'L' && path[pathLength - 1] =='L' && shortDone == 0)
	{
		pathLength -= 3;
		path[pathLength] = 'S';
		shortDone = 1;
	}
	path[pathLength+1] = 'D';
	path[pathLength+2] = 'D';
	pathLength++;
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
  	&& rightFarReading>200 && CenterSensorReading>200)
	{
		turnAround();
	}
}
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
