//all robot code

#include <iostream>
#include "default.hpp"

using namespace std;

int main(){
    configureAllSensors();

    displayString(2, "Maze Runner")
    displayString(3, "Made by Jason Chen, Zachary Barandino, Eric Mak, and Eric Shaw")
    displayString(5, "Press Enter to start");
    while (!(getButtonPress(ButtonEnter)))
    {}
    while (getButtonPress(ButtonEnter))
    {}

    float timetofinish = 0;
    time1[T1] = 0;

    /*
    version 1.1 of maze solving: (turn left algorithm which includes intersection lights)

    need to introduce a secondary objective: pick up block and sort it
    could introduce A* algorithm to solve maze
    */
    bool unsolved = true;
    while (unsolved)
    {
        goRobot(50);
        while (SensorValue[S3] == (int)colorBlack)
        {}
        goRobot(0);

        //check for finish
        if (SensorValue[S3] == (int)colorGreen)
        {
            unsolved = false;
            timetofinish = time1[T1] / 1000.0;
        }
        //turn left algorithm at turns or ends of paths
        else if (SensorValue[S3] == (int)colorRed)
        {
            do
            {
                rotateRobot(90, 25);
                nmotorEncoder[motorA] = 0;
                goRobot(10); // go until new colour - check if path is available
                while(SensorValue[S3] == (int)colorRed)
                {}
                goRobot(0);

                if (SensorValue[S3] == (int)colorGreen)
                {
                    unsolved = false;
                    timetofinish = time1[T1] / 1000.0;
                }
                else if (SensorValue[S3] != (int)colorBlack) //go back
                {
                    goRobot(-10);
                    while(nMotorEncoder[motorA] > 0)
                    {}
                    goRobot(0);
                }
            } while (SensorValue[S3] != (int)colorBlack || SensorValue[S3] != (int)colorGreen); //does the turn left algorithm until it detects black or green
        }
        else //accounts for turning error in both directions
            for(int i = 1; i < 9; i++)
            {
                if (i % 2 == 0)
                {
                    rotateRobot(5 * i, 25);
                    if (SensorValue[S3] != (int)colorBlack)
                        i += 10;
                }
                else 
                {
                    rotateRobot(-(5 * i), 25); 
                    if (SensorValue[S3] != (int)colorBlack)
                        i += 10;
                }
            }
    }

    displayString(5, "Maze Solved!");
    displayString(6, "Time: %f s", timetofinish);
    wait1Msec(10000);

    return 0;
}

//code to detect, locate, pick up, and sort block
    //2 objects

//code to grab block

//code to map maze
    //object

//code to solve maze