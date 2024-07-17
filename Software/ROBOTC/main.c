void angleAdjust();

void checkFinish();

void configureAllSensors();

void goRobot(int motorPower);

int grabBlock(int colorInteger);

bool releaseBlock();

void rotateRobot(float angle, int motorPower);

void turnError();

void turnLeft();

task main(){
    configureAllSensors();

    displayString(2, "Maze Runner");
    displayString(3, "Made by Jason Chen, Zachary Barandino, Eric Mak, and Eric Shaw");
    displayString(5, "Press Enter to start");
    while (!(getButtonPress(buttonEnter)))
    {}
    while (getButtonPress(buttonEnter))
    {}

    //constants to set drive and turning speeds
    const int MOTPOWER = 15;
    const int MOTSPINPOWER = 10;

    //constants to set number of blocks in maze
    const int NUMBLOCKS = 1;

    float timetofinish = 0;
    time1[T1] = 0;

    displayString(7, "Time elapsed %f", time1[T1]/1000.0);
    displayString(9, "Current gyro angle %f", getGyroDegrees(S4));

    int colorInteger = 0;

    //version 1.1 of maze solving: (turn left algorithm which includes intersection lights)
    //version 1.1 of block acquiring - set blockobtained to false to enable blocks. includes multiblocking
    int numblocksobtained = 0;
    bool blockobtained = true; //will be replaced with the return of bool function grabBlock
    while (numblocksobtained < NUMBLOCKS)
    {
        goRobot(MOTPOWER);
        while (SensorValue[S3] == (int)colorBlack)
        {
            if(SensorValue[S3] == 1)
            {
                goRobot(0);
                grabBlock(colorInteger);
                goRobot(MOTPOWER);
            }
            else if(getButtonPressed(buttonEnter))
            {
                while (getButtonPress(buttonEnter))
                {}
                goRobot(0);
                angleAdjust(); //incomplete

                while (!(getButtonPress(buttonEnter)))
                {}
                while (getButtonPress(buttonEnter))
                {}
                goRobot(MOTPOWER);
            }
        }
        goRobot(0);

        if (SensorValue[S3] == (int)colorGreen)
        {
            checkFinish();
        }
        else if (SensorValue[S3] == (int)colorRed)
        {
            turnLeft((int)colorRed);
        }
        else
      	{
            turnError();
    	}
  	}
    displayString(11, "Maze Solved!");
    displayString(13, "Time: %f s", timetofinish);
    wait1Msec(10000);
}

/*
objectives:
1. To map maze and keep track of everything
2. To include block sorting at the finish line - void blockSort with two color sensors
3. To include multiple blocks in a maze
4. To include maze solving efficiency - A* or D ...
*/

void angleAdjust()
{
}

void checkFinish()
{
    if(blockobtained)
    {
        numblocksobtained++;
         //include block sorting here. reset the time1[T1] after
        timetofinish += time1[T1];
    }
    else
        turnLeft((int)colorGreen);
}

void configureAllSensors(){
    SensorType[S1] = sensorEV3_Touch;
    SensorType[S2] = sensorEV3_Ultrasonic;
    SensorType[S3] = sensorEV3_Color;
    SensorType[S4] = sensorEV3_Gyro;
    wait1Msec(50);

    SensorMode[S3] = modeEV3Color_Color;
    SensorMode[S4] = modeEV3Gyro_Calibration;
    wait1Msec(50);

    SensorMode[S4] = modeEV3Gyro_RateAndAngle;
    wait1Msec(50);
}

void goRobot(int motorPower){
    motor[motorA] = motor[motorD] = motorPower;
}

void grabBlock(int & colorInteger)
{
    colorInteger = SensorValue[S3];

    nMotorEncoder[motorB] = 0;

    motor[motorC] = 20;     //*dont know if + or - power closes/opens jaws or raise/lowers hinge, subject to change
    wait1Msec(5000); //*jaws will keep power on to continiously close to hold onto object for entire duration

    motor[motorB] = 20;
    while(nMotorEncoder[motorB] <= 60)
    {}
    motor[motorB] = 0;
}

bool releaseBlock()
{
    motor[motorC] = -20;
    wait1Msec(5000);
    motor[motorC] = 0;

    motor[motorB] = -20;
    while(nMotorEncoder[motorB] >= 0)
    {}
    motor[motorB] = 0;

    return true;
}

void rotateRobot(float angle, int motorPower){
    motorPower = abs(motorPower);
    resetGyro(S4);

    if(angle > 0)
    {
        motor[motorD] = motorPower;
    }
    else
    {
        motor[motorA] = motorPower;
    }

    while(abs(getGyroDegrees(S4)) < abs(angle))
    {}
    goRobot(0);
}

//turn left algorithm at turns or ends of paths
//TO DO: have it move a certain mimimum distance
void turnLeft(int currColor)
{
    do
    {
        rotateRobot(90, MOTSPINPOWER);
        nMotorEncoder[motorA] = 0;
        goRobot(10); // go until new colour - check if path is available
        while(SensorValue[S3] == currColor)
        {}
        goRobot(0);

        if (SensorValue[S3] == (int)colorGreen) //is this correct?
        {
            checkFinish();
        }
        else if (SensorValue[S3] != (int)colorBlack) //go back
        {
            goRobot(-MOTPOWER);
            while(nMotorEncoder[motorA] > 0)
            {}
            goRobot(0);
        }
        else
            turnError();
    } while (SensorValue[S3] == currColor);
    //SensorValue[S3] != (int)colorBlack || SensorValue[S3] != (int)colorGreen
    //does the turn left algorithm until it detects new colours
    //but if we see a colour other than red, green, or black, what happens?
}

//accounts for turning error in both directions
void turnError()
{
    for(int i = 1; i < 6; i++)
    {
        if (i % 2 == 0)
        {
            rotateRobot(5 * i, MOTSPINPOWER);
            if (SensorValue[S3] == (int)colorBlack)
                i += 10;
        }
        else
        {
            rotateRobot(-(5 * i), MOTSPINPOWER);
            if (SensorValue[S3] == (int)colorBlack)
                i += 10;
        }
    }
}
