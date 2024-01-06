#include "Robot.h"
#include <vector>
#include <cmath>


//Adjustable parameters
const double d_sep = 10.0;
const double d_ca = 50.0;
const double avoidfactor = 0.3;
const double alignfactor = 0.4;
const double centeringfactor = 0.1;
const double min_speed = 1.0;
const double max_speed = 5.0;
const double turn_factor = 0.2;


Robot::Robot(){}
// Constructor definition (initialization)
Robot::Robot(double pos_x, double pos_y, double vel_x, double vel_y) 
     : pos_x_{pos_x}, pos_y_{pos_y}, vel_x_{vel_x}, vel_y_{vel_y}{}

void Robot::separation(const std::vector<Robot>& Robots) {
        
        double close_diff_pos_x = 0.0;
        double close_diff_pos_y = 0.0; 

        for (size_t i = 0; i< Robots.size(); ++i) {
            const Robot& other_Robot = Robots[i];
            double distance = hypot(pos_x_ - other_Robot.pos_x_, pos_y_ - other_Robot.pos_y_);
            if (distance > 0 && distance < d_sep) {
                close_diff_pos_x += pos_x_ - other_Robot.pos_x_;
                close_diff_pos_y += pos_y_ - other_Robot.pos_y_;
            }
        }
        // Update speed based on separation
        vel_x_ += close_diff_pos_x * avoidfactor;
        vel_y_ += close_diff_pos_y * avoidfactor;
}
void Robot:: alignment(const std::vector<Robot>& Robots){
        double avg_vel_x = 0.0;
        double avg_vel_y = 0.0;
        int neighboring_Robots = 0;

        for (size_t i = 0; i< Robots.size(); ++i) {
            const Robot& other_Robot = Robots[i];
            double distance = hypot(pos_x_ - other_Robot.pos_x_, pos_y_ - other_Robot.pos_y_);

            if(distance > 0 && distance < d_ca){
                avg_vel_x += other_Robot.vel_x_;
                avg_vel_y += other_Robot.vel_y_;
                neighboring_Robots ++ ;
            } 
        }
        if (neighboring_Robots > 0){
            avg_vel_x = avg_vel_x / neighboring_Robots;
            avg_vel_y = avg_vel_y / neighboring_Robots;
            // Update speed based on alignment
            vel_x_ += (avg_vel_x - vel_x_) * alignfactor;
            vel_y_ += (avg_vel_y - vel_y_) * alignfactor;
        }
}
void Robot:: cohesion(const std::vector<Robot>& Robots){
        double avg_pos_x = 0.0;
        double avg_pos_y = 0.0;
        int neighboring_Robots = 0;

        for (size_t i = 0; i< Robots.size(); ++i) {
            const Robot& other_Robot = Robots[i];
            double distance = hypot(pos_x_ - other_Robot.pos_x_, pos_y_ - other_Robot.pos_y_);

            if (distance > 0 && distance < d_ca){
                avg_pos_x += other_Robot.pos_x_;
                avg_pos_y += other_Robot.pos_y_;
                neighboring_Robots++;
            }
        }

        if (neighboring_Robots > 0){
            avg_pos_x = avg_pos_x / neighboring_Robots;
            avg_pos_y = avg_pos_y / neighboring_Robots; 
            //Update speed based on cohesion
            vel_x_ += (avg_pos_x - pos_x_) * centeringfactor;
            vel_y_ += (avg_pos_y - pos_y_) * centeringfactor;
        }

}
void Robot:: limitSpeed(){
        //Update speed based on speed limits
        double speed = hypot(vel_x_, vel_y_);

        if (speed > max_speed){
            vel_x_ = (vel_x_ / speed) * max_speed;
            vel_y_ = (vel_y_ / speed) * max_speed;
        }
        if (speed < min_speed){
            vel_x_ = (vel_x_ / speed) * min_speed;
            vel_y_ = (vel_y_ / speed) * min_speed;
        }

}

void Robot:: borders(const int left_margin,const int right_margin, const int top_margin, const int bottom_margin ){

    if (pos_x_ < left_margin + left_margin*0.25 ) {
        pos_x_ = vel_x_ - turn_factor;
    }
    if (pos_x_ > right_margin - right_margin*0.05) {
        pos_x_ = vel_x_ - turn_factor;
    }
    if (pos_y_ > bottom_margin + bottom_margin*0.15 ) {
        vel_y_ = vel_y_ - turn_factor;
    }
    if (pos_y_ < top_margin - top_margin*0.05 ) {
        vel_y_ = vel_y_ + turn_factor;
    }
}

void Robot:: position_update(const std::vector<Robot>& Robots){

        separation(Robots);
        alignment(Robots);
        cohesion(Robots);
        limitSpeed();
    
        pos_x_ = pos_x_ + vel_x_;
        pos_y_ = pos_y_ + vel_y_;
}