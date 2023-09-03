//The DIY Life
//Michael Klements
//29 June 2020
//Obstacle Avoiding Robot

// #include <AFMotor.h>                              //Import library to control motor shield
#include <Servo.h>                                //Import library to control the servo

#define enA 0
#define in1 14  
#define in2 2
#define enB 5
#define in3 16
#define in4 4
#define servoPin 12

Servo servoLook;                                  //Create an object to control the servo

int trig = 13;                                    //Assign the ultrasonic sensor pins
int echo = 15;

int distance ;
int maxDist = 150;                               //Maximum sensing distance (Objects further than this distance are ignored)
int stopDist = 50;                               //Minimum distance from an object to stop in cm

int motorSpeed = 110;                             //The maximum motor speed
int motorSpeedL= 110;

int turnSpeed = 110;                               //Amount to add to motor speed when turning


void setup() 
{

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);


  servoLook.attach(servoPin);                           //Assign the servo pin
  pinMode(trig,OUTPUT);                           //Assign ultrasonic sensor pin modes
  pinMode(echo,INPUT);

  Serial.begin(115200);                           //For Debuging
}

void loop() 
{
  servoLook.write(90);                            //Set the servo to look straight ahead
  delay(750);
  distance = getDistance();                       //Check that there are no objects ahead
  if(distance >= stopDist)                        //If there are no objects within the stopping distance, move forward
  {
    moveForward();
    Serial.println("moveForward()");              //For Debuging
  }
  while(distance >= stopDist)                     //Keep checking the object distance until it is within the minimum stopping distance
  {
    distance = getDistance();
    delay(250);
  }
  stopMove();                                     //Stop the motors
  Serial.println("stopMove()");                   //For Debuging
  int turnDir = checkDirection();                 //Check the left and right object distances and get the turning instruction
  Serial.println(turnDir);                        //For Debuging
  switch (turnDir)                                //Turn left, turn around or turn right depending on the instruction
  {
    case 0:                                       //Turn left
      turnLeft (950);
      Serial.println("turnLeft (850)");           //For Debuging  
      break;
    case 1:                                       //Turn around
      turnLeft (1650);
        Serial.println("return (1550)");          //For Debuging
      break;
    case 2:                                       //Turn right
      turnRight (800);
      Serial.println("turnRight (700)");          //For Debuging
      break;
  }
}

void moveForward()                                //Set all motors to run forward
{
  analogWrite(enA,motorSpeed);
  analogWrite(enB,motorSpeedL);
  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
    
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stopMove()                                   //Set all motors to stop
{
  analogWrite(enA,0);
  analogWrite(enB,0);
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
    
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void turnLeft(int duration)                                 //Set motors to turn left for the specified duration then stop
{
  analogWrite(enA,turnSpeed);
  analogWrite(enB,turnSpeed);
  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  delay(duration);
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
    
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
}

void turnRight(int duration)                                //Set motors to turn right for the specified duration then stop
{
  analogWrite(enA,turnSpeed);
  analogWrite(enB,turnSpeed);
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  delay(duration);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

int getDistance()                                   //Measure the distance to an object
{
  unsigned long pulseTime;                         //Create a variable to store the pulse travel time
  int distance;                                    //Create a variable to store the calculated distance
  digitalWrite(trig, LOW);                      
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);                         //Generate a 10 microsecond pulse
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  pulseTime = pulseIn(echo, HIGH);  
  distance = (float)pulseTime * 340 / 2 / 10000;    //Calculate the object distance based on the pulse time
  Serial.print("Distance: ");                       //For Debuging
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

int checkDirection()                                            //Check the left and right directions and decide which way to turn
{
  int distances [2] = {0,0};                                    //Left and right distances
  int turnDir = 1;                                              //Direction to turn, 0 left, 1 reverse, 2 right
  servoLook.write(180);                                         //Turn servo to look left
  delay(500);
  distances [0] = getDistance();                                //Get the left object distance
  servoLook.write(0);                                           //Turn servo to look right
  delay(1000);
  distances [1] = getDistance();                                //Get the right object distance
  if (distances[0]>=200 && distances[1]>=200)                   //If both directions are clear, turn left
    turnDir = 0;
  else if (distances[0]<=stopDist && distances[1]<=stopDist)    //If both directions are blocked, turn around
    turnDir = 1;
  else if (distances[0]>=distances[1])                          //If left has more space, turn left
    turnDir = 0;
  else if (distances[0]<distances[1])                           //If right has more space, turn right
    turnDir = 2;
  return turnDir;
}
