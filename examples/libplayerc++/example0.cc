#include <iostream>

#include "playerc++.h"

int main(int argc, char *argv[])
{
  PlayerCc::PlayerClient  robot("localhost");
  PlayerCc::SonarProxy    sp(&robot,0);
  PlayerCc::PositionProxy pp(&robot,0);

  double newturnrate, newspeed;

  for(int i=0;i<1000;i++)
  {
    robot.Read();

    // print out sonars for fun
    std::cout << sp << std::endl;

    // do simple collision avoidance
    if((sp.Range(0) + sp.Range(1)) <
       (sp.Range(6) + sp.Range(7)))
      newturnrate = DTOR(-20); // turn 20 degrees per second
    else
      newturnrate = DTOR(20);

    if(sp.Range(3) < 0.500)
      newspeed = 0;
    else
      newspeed = 0.100;

    // command the motors
    pp.SetSpeed(newspeed, newturnrate);
  }
}
