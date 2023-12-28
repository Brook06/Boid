// ROBOT_H
#ifndef Robot_h
#define Robot_h

#include <iostream>
#include <vector>

using namespace std;

class Robot{

private:

    void separation(const std::vector<Robot>& Robots);   
    void alignment(const std::vector<Robot>& Robots);
    void cohesion(const std::vector<Robot>& Robots);
    void limitSpeed();
    

public:
  // Posizione del robot
    double pos_x_, pos_y_;
    // Velocità del _robot
    double vel_x_, vel_y_;
    Robot();
    //costruttore personalizzato
    Robot(double pos_x_, double pos_y_, double vel_x_, double vel_y_);
   
    void position_update(const std::vector<Robot>& Robots,const int step);
    void borders(Robot& Robot,double left_margin, double right_margin, double top_margin, double bottom_margin );
};

#endif 
//ROBOT_H