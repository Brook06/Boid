#include "Robot.h"
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>


// Parametri regolabili

const double d_sep = 10.0;
const double d_ca = 50.0;
const double avoidfactor = 0.01;
const double alignfactor = 0.01;
const double centeringfactor = 0.01;
const double min_speed = 1.0;
const double max_speed = 5.0;
const double turn_factor = 1.0;


Robot::Robot(){}
// definizione (inizializzazione) del costruttore 
Robot::Robot(double pos_x, double pos_y, double vel_x, double vel_y) 
     : pos_x_{pos_x}, pos_y_{pos_y}, vel_x_{vel_x}, vel_y_{vel_y}{}

// creazione delle funzioni

// inizializzazione statica del vettore 
//std::vector<Robot> Robots;

// Funzione di separazione: 
void Robot::separation(const std::vector<Robot>& Robots) {
        // Accumulare le differenze di posizione 
        double close_diff_pos_x = 0.0;
        double close_diff_pos_y = 0.0; 

        // Ciclo su tutti gli altri Robot
        for (size_t i = 0; i< Robots.size(); ++i) {
            const Robot& other_Robot = Robots[i];
            //calcolo la distanza Euclidea
            double distance = hypot(pos_x_ - other_Robot.pos_x_, pos_y_ - other_Robot.pos_y_);
            
            if (distance > 0 && distance < d_sep) {
                close_diff_pos_x += pos_x_ - other_Robot.pos_x_;
                close_diff_pos_y += pos_y_ - other_Robot.pos_y_;
            }
        }

        // Aggiorna la velocità in base alla separazione
        vel_x_ += close_diff_pos_x * avoidfactor;
        vel_y_ += close_diff_pos_y * avoidfactor;
}
void Robot:: alignment(const std::vector<Robot>& Robots){
        double avg_vel_x = 0.0;
        double avg_vel_y = 0.0;
        int neighboring_Robots = 0;

        //calcolo della distanza 
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
            // Aggiorna la velocità in base all'allineamento
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

            vel_x_ += (avg_pos_x - pos_x_) * centeringfactor;
            vel_y_ += (avg_pos_y - pos_y_) * centeringfactor;
        }

}
void Robot:: limitSpeed(){
        double speed = hypot(vel_x_, vel_y_);

        if (speed > max_speed){
            vel_x_ = (vel_x_ / speed) * max_speed;
            vel_y_ = (vel_y_ / speed) * max_speed;
            //cerr << "troppo veloce"<< endl;
        }
        if (speed < min_speed){
            vel_x_ = (vel_x_ / speed) * min_speed;
            vel_y_ = (vel_y_ / speed) * min_speed;
            //cerr << "troppo lento"<< endl;
        }

}
void Robot:: borders(Robot& Robot,double left_margin, double right_margin, double top_margin, double bottom_margin ){
    if (Robot.pos_x_ < left_margin) {
        Robot.vel_x_ = Robot.vel_x_ + turn_factor;
    }
    if (Robot.pos_x_ > right_margin) {
        Robot.vel_x_ = Robot.vel_x_ - turn_factor;
    }
    if (Robot.pos_y_ > bottom_margin) {
        // Applica la rotazione organica quando il boid si avvicina al bordo inferiore
        Robot.vel_y_ = Robot.vel_y_ - turn_factor;
    }
    if (Robot.pos_y_ < top_margin) {
        // Applica la rotazione organica quando il boid si avvicina al bordo superiore
        Robot.vel_y_ = Robot.vel_y_ + turn_factor;
    }
}

    
void Robot:: position_update(const std::vector<Robot>& Robots, const int step, const int id){
        //cerr << "inizio separazione "<< step << endl;
        separation(Robots);
        //cerr << "inizio alineamento"<< step << endl;
        alignment(Robots);
        //cerr << "inizio coesione"<< step << endl;
        cohesion(Robots);
        //cerr << "inizio limite di velocità"<< step << endl;
        limitSpeed();
        //cerr << "fine aggiornamento posizione:  "<< step << " della thread numero: "<< id << endl;

        // aggiornamento posizione considero delta T = 1, velcoità di 1px/frame 
        pos_x_ = pos_x_ + vel_x_;
        pos_y_ = pos_y_ + vel_y_;
}