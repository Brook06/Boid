// Robot.h
#include ROBOT_H
#define ROBOT_H

#include <iostream>

using namespace std;

class Robot{

public:
    // Posizione del robot
    double pos_x_, pos_y_;
    // Velocità del _robot
    double vel_x_, vel_y_;
    // Costruttore di default
    Robot();
    //costruttore personalizzato
    Robot(double pos_x_, double pos_y_, double vel_x_, double vel_y_)

    void position_update();

private:

    void separation();    
    void alignment();
    void cohesion();
    void limitSpeed();

    //vettore che conterra tutti i robot creati  
    static vector<Robot> Robots;
};

#endif 
//ROBOT_H