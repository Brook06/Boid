#include "Robot.h"
#include <fstream>
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

/// Inizializzazione statica
std::vector<Robot> Robots;

int main() {
    cerr << "inizio" << endl;
    
    Robot newRobot1(50.0, 50.0, 1.0, 0.5);
    Robots.push_back(newRobot1);
    Robot newRobot2(100.0, 100.0, -0.5, 1.0);
    Robots.push_back(newRobot2);
    Robot newRobot3(80.0, 50.0, -1.0, -0.5);
    Robots.push_back(newRobot3);

    //cerr << "fine creazione robot di prova" << endl;
    cerr << "dimensione vettore"<<Robots.size()<< endl;

    //salvataggio delle posizioni 
    ofstream outputFile("coordinates.txt",ios::app);

    if (outputFile.is_open()) {
        for(int step = 0; step < 100; step++){
            std::cerr << "ciclo numero"<< step << endl;

            for(size_t i = 0; i< Robots.size(); ++i){
            Robots[i].position_update(Robots, step);
            outputFile << Robots[i].pos_x_ <<" "<< Robots[i].pos_y_<< " " << Robots[i].vel_x_ <<" "<< Robots[i].vel_y_ << " " ;
            }
            outputFile << std::endl;
        }  
        //outputFile << endl;
        outputFile.close(); 
    }else {
            cerr << "Errore nell'apertura del file di output." << endl;
            return 1; // Termina il programma con un codice di errore
        }

    return 0;
}