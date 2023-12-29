#include "Robot.h"
#include <fstream>
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <cstdlib>  // Per la funzione rand()
#include <ctime>    
#include <thread>
#include <mutex>

/// Inizializzazione statica
std::vector<Robot> Robots;

int main() {
    std::srand(std::time(0));
    cerr << "inizio" << endl;
    int no_of_robot;
    cerr << "inserisci il numero di Robot: ";
    cin >> no_of_robot ;
    // Crea 100 oggetti Robot con posizioni e velocità casuali
    for (int i = 0; i < no_of_robot; ++i) {
        float rand_x = static_cast<float>(std::rand() % no_of_robot); // Posizione x casuale
        float rand_y = static_cast<float>(std::rand() % no_of_robot); // Posizione y casuale
        float rand_vx = static_cast<float>(std::rand()) / RAND_MAX - 0.5; // Velocità x casuale tra -0.5 e 0.5
        float rand_vy = static_cast<float>(std::rand()) / RAND_MAX - 0.5; // Velocità y casuale tra -0.5 e 0.5

        Robots.push_back(Robot(rand_x, rand_y, rand_vx, rand_vy));
    }
    

    //cerr << "fine creazione robot di prova" << endl;
    cerr << "dimensione vettore"<<Robots.size()<< endl;
    double left_margin, right_margin, top_margin, bottom_margin;
    cerr << "inserisci i confini: x_min, x_max , y_min, y_max";
    cin >> left_margin >> right_margin >> top_margin >> bottom_margin ;

    vector<std::thread> threads;

    //salvataggio delle posizioni 
    ofstream outputFile("coordinates.txt",ios::app);

    if (outputFile.is_open()) {
        outputFile << left_margin <<" " << right_margin <<" "<< top_margin <<" "<< bottom_margin << " ";
        outputFile << std::endl;

        for(int step = 0; step < 1000; step++){
            std::cerr << "ciclo numero"<< step << endl;
            // scrivere il ciclo for per la creazione delle threads, ci saranno Robots.size() threads, prima aggiorno tutte le posizioni 
            // salvandole in un vettore e poi stampo il vettore con le posizioni aggionate. Ho bisogno di un vettore bool che mi dica che il thread nell posizione i 
            //del vettore Robots ha aggiornato la sua posizione e che una volta che il vettore è tutto vero allora il vettore Robots è tutto aggiornato,
            // allora posso aspettare che tutte le thread abbiano finito, che poi stampo sul file esterno le coordinate aggiornate e ricomincio con il nuovo ciclo for step++.

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