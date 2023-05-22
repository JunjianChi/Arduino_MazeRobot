//Side Button variables
int sideButton1 = 4; // Leftbutton
int sideButton2 = 5; // Rightbutton
int sideState1 = 0; // determing whether the leftbutton is pressed
int sideState2 = 0; // determing whether the rightbutton is pressed

// Motors controlling left and right 
int motorpin1 = 6; //Right direction
int motorpin2 = 7; //Left direction
int sideCount = 0;

// Front Sensor variables
int frontSensor = A4; // photodiode
int threshold = 200; // Need to be adjusted when testing

// Motors controlling forward motion
int DIRA = 12; // motor 1 in forward and backward
int PWMA = 3; // Adjust speed for motor 1
int DIRB = 13; // motor 2 in forward and backward
int PWMB = 11; // Adjust speed for motor 2

void setup()
{
  Serial.begin(9600); //Setup sesrial
  pinMode(sideButton1, INPUT);
  pinMode(sideButton2, INPUT);
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
}

void TurnRight(){
  digitalWrite(motorpin1, HIGH);
  digitalWrite(motorpin2, LOW);
}

void TurnLeft(){
  digitalWrite(motorpin2, HIGH);
  digitalWrite(motorpin1, LOW);
}

void TurnForward(){
  digitalWrite(DIRA, HIGH); // Turn in one motor to forward direction.
  analogWrite(PWMA, 127); 
  digitalWrite(DIRB, HIGH); // Turn in the other motor to forward direction.
  analogWrite(PWMB, 127); 
}

float DetectLight(){
  //Front Sensor
  int frontWall = analogRead(frontSensor);
  Serial.print("Front Wall: ");
  Serial.println(frontWall);
  Serial.println();
  return frontWall;
}


int CheckComb(int PhotoTimeDuration, int threshold, float frontWall){
  while (PhotoTimeDuration < 1.5){
    frontWall = DetectLight();
    if (frontWall > threshold){ // still has front wall
      return 0;
    }
    PhotoTimeDuration += 1;
  }
  return 1; // comb wall has passed
} 

void loop()
{
  int PhotoTimeDuration = 0;
  Serial.begin(9600);
  // read photodiode
  int frontWall = DetectLight();

  if ((frontWall < threshold) && (sideState1 == 0) && (sideState2 == 0)){ //no Wall ahead
    TurnForward(); 
  }

  while (frontWall > threshold){ 
    analogWrite(PWMA, 0); // Stop the forward motor1
    analogWrite(PWMB, 0); // Stop the forward motor2
    TurnLeft(); //always try left first
    // Check comb
    int combFlag = CheckComb(PhotoTimeDuration,threshold,frontWall);
    PhotoTimeDuration = 0; //Reset

    if (combFlag == 1){
      delay (200); // Needs to be tested in lab; Time to keep turning left/right after finish comb
      // stop turning left/right
      digitalWrite(motorpin2, LOW);
      digitalWrite(motorpin1, LOW);
      // moving forward
      TurnForward();
      break;
    }
    if(sideState1 == 1){ //Hit Left
      TurnRight();
    }

    if(sideState2 == 1){ //Hit Right
      TurnLeft();
    }
  }

}















