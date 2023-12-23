#include <Robot.h>
using namespace std;

'Parametri regolabili'

const double d_sep = 10.0;
const double voidfactor = 0.01;

'costruttore'
// inizializzazione statica del vettore 
vector<Robot>Robot::Robots
// definizione (inizializzazione) del costruttore 
Robot::Robot() : pos_x(0.0), pos_y(0.0), vel_x(0.0), vel_y(0.0){}

'creazione delle funzioni'

// Funzione di separazione
    void separation() {
        // Accumulare le differenze di posizione 
        double close_diff_pos_x = 0.0;
        double close_diff_pos_y = 0.0; 

        // Ciclo su tutti gli altri boid
        for (const Robot& other_Robot : Robots) {
            //calcolo la distanza Eucl
            double distance = std::hypot(Robot.pos_x - other_Robot.pos_x, Robot.pos_y - other_Robot.pos_y);
            
            if (distance > 0 && distance < d_sep) {
                close_diff_pos_x += Robot.pos_x - other_Robot.pos_x;
                close_diff_pos_y += Robot.pos_y - other_Robot.pos_y;
            }
        }

        // Aggiorna la velocità in base alla separazione
        Robot.vel_x += close_diff_pos_x * avoidfactor;
        Robot.vel_y += close_diff_pos_y * avoidfactor;
    }
