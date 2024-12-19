#include "RoboticArmBuilder.h"
#include <unistd.h> // For sleep()

int main()
{
    RoboticArmBuilder robot;


    robot.setOrientation(0, 270, 0);

    robot.setEndPosition(100, -50, 200);

    sleep(5);

    robot.setEndPosition(100, 0, 200);

    sleep(5);

    robot.setEndPosition(100, 50, 200);


    return 0;
}
