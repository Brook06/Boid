// ROBOT_H
#ifndef Robot_h
#define Robot_h

#include <iostream>
#include <vector>
#include <mutex> 

using namespace std;

class Robot{

private:
    
    void separation(const std::vector<Robot>& Robots);   
    void alignment(const std::vector<Robot>& Robots);
    void cohesion(const std::vector<Robot>& Robots);
    void limitSpeed();

public:
    double pos_x_, pos_y_; //posizione del Robot
    double vel_x_, vel_y_; //velocità del Robot
    //int time_step_; 

    Robot();
    Robot(double pos_x_, double pos_y_, double vel_x_, double vel_y_);
   
    void position_update(const std::vector<Robot>& Robots);
    void borders(const int left_margin,const int right_margin, const int top_margin, const int bottom_margin );
};

#endif 
//ROBOT_H