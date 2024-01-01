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
#include <algorithm>


std::mutex mutexRobots;
std::mutex coutMutex;   
//std::condition_variable cv;
//std::atomic<int> turn(0);
/// Inizializzazione statica
vector<Robot> Robots;
vector<Robot> Robots_output;
vector<bool> updated_Robots;

void updatePosition(int id, const int step, const double left_margin,const double right_margin,const double top_margin,const double bottom_margin ){
    Robot robot_thread = Robots[id];
    if(!updated_Robots[id]){
        //std::unique_lock<std::mutex> lock(mutexRobots);
        robot_thread.position_update(Robots, step, id);
        robot_thread.borders(robot_thread,left_margin, right_margin, top_margin, bottom_margin);
        //std::lock_guard<std::mutex> lock(mutexRobots);
        mutexRobots.lock();
        Robots_output[id] = robot_thread;
        updated_Robots[id] = true;
        mutexRobots.unlock();
        //bul_vecto = true;
        coutMutex.lock();  
        cerr << "thread numero: "<< id <<" valore_update : "<< updated_Robots[id] <<" step  :" << step << endl;
        coutMutex.unlock();
    }  
}

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
    cerr << "dimensione vettore "<<Robots.size()<< endl;
    double left_margin, right_margin, top_margin, bottom_margin;
    cerr << "inserisci i confini: x_min, x_max , y_min, y_max " << endl;
    cin >> left_margin >> right_margin >> top_margin >> bottom_margin ;

    Robots_output.resize(no_of_robot);
    updated_Robots.resize(no_of_robot + 1, false);
    //vector<std::thread> threads;

    //salvataggio delle posizioni 
    ofstream outputFile("coordinates.txt",ios::app);
    //outputFile.open(outputFile,std::ios::out | std::ios::trunc);

    if (outputFile.is_open()) {
        outputFile << left_margin <<" " << right_margin <<" "<< top_margin <<" "<< bottom_margin << " ";
        outputFile << std::endl;

        for(int step = 0; step < 1000; step++){
            cerr << "ciclo numero"<< step << endl;
            vector<std::thread> threads;
            for(int j = 0; j < no_of_robot; ++j){
                std::string thread_name = "Rob" + std::to_string(j);
                threads.push_back(std::thread(updatePosition, j, step, left_margin, right_margin, top_margin, bottom_margin));
            }

            for (size_t z = 0; z < no_of_robot; ++z) {
                threads[z].join();
            }
            cerr << "tutte le thread sono chiuse: "<< step << endl;

            for(size_t i = 0; i< Robots_output.size(); ++i){
                outputFile << Robots_output[i].pos_x_ <<" "<< Robots_output[i].pos_y_<< " ";
                updated_Robots[i] = false;
                
            }
            cerr << "tutte le posizioni sono stampate: " << step << endl;
            for(size_t t = 0; t< Robots_output.size(); ++t){
                Robots[t] = Robots_output[t];
                cerr << "Robots modifica elemento numero: "<< t <<" : "<< step << endl;
            }
            cerr << "il vettore Robots è stato modificato: " << step<< endl;
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