#include "Robot.h"
#include <fstream>
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <cstdlib>  // Per la funzione rand()
#include <ctime>    // Per inizializzare il seed di rand()

/// Inizializzazione statica
std::vector<Robot> Robots;

int main() {
    std::srand(std::time(0));
    cerr << "inizio" << endl;
    // Crea 100 oggetti Robot con posizioni e velocità casuali
    for (int i = 0; i < 100; ++i) {
        float rand_x = static_cast<float>(std::rand() % 500); // Posizione x casuale
        float rand_y = static_cast<float>(std::rand() % 500); // Posizione y casuale
        float rand_vx = static_cast<float>(std::rand()) / RAND_MAX - 0.5; // Velocità x casuale tra -0.5 e 0.5
        float rand_vy = static_cast<float>(std::rand()) / RAND_MAX - 0.5; // Velocità y casuale tra -0.5 e 0.5

        Robots.push_back(Robot(rand_x, rand_y, rand_vx, rand_vy));
    }
    
   // Robot newRobot1(50.0, 50.0, 1.0, 0.5);
   // Robots.push_back(newRobot1);
   // Robot newRobot2(60.0, 100.0, -0.5, 1.0);
   // Robots.push_back(newRobot2);
   // Robot newRobot3(80.0, 50.0, -1.0, -0.5);
   // Robots.push_back(newRobot3);
   // Robot newRobot4(10.0, 10.0, 1.0, 0.5);
   // Robots.push_back(newRobot4);
   // Robot newRobot5(80.0, 90.0, -0.5, 1.0);
   // Robots.push_back(newRobot5);
   // Robot newRobot6(1.0, 5.0, -1.0, -1.5);
   // Robots.push_back(newRobot6);

    //cerr << "fine creazione robot di prova" << endl;
    cerr << "dimensione vettore"<<Robots.size()<< endl;
    double left_margin, right_margin, top_margin, bottom_margin;
    cerr << "inserisci i confini: x_min, x_max , y_min, y_max";
    cin >> left_margin >> right_margin >> top_margin >> bottom_margin ;

    //salvataggio delle posizioni 
    ofstream outputFile("coordinates.txt",ios::app);

    if (outputFile.is_open()) {
        outputFile << left_margin <<" " << right_margin <<" "<< top_margin <<" "<< bottom_margin << " ";
        outputFile << std::endl;

        for(int step = 0; step < 10000; step++){
            std::cerr << "ciclo numero"<< step << endl;

            for(size_t i = 0; i< Robots.size(); ++i){
            Robots[i].position_update(Robots, step);
            Robots[i].borders(Robots[i],left_margin, right_margin, top_margin, bottom_margin);
            outputFile << Robots[i].pos_x_ <<" "<< Robots[i].pos_y_<< " ";
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