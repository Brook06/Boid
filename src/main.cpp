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
#include <filesystem>

std::mutex mutexRobots;
std::mutex coutMutex;   

vector<Robot> Robots;
vector<Robot> Robots_output;
vector<bool> updated_Robots;

void update_Position(int Robot_id, const int step, const double left_margin,const double right_margin,const double top_margin,const double bottom_margin ){
    
    Robot robot_thread = Robots[Robot_id];
    if(!updated_Robots[Robot_id]){
        //std::unique_lock<std::mutex> lock(mutexRobots);
        robot_thread.position_update(Robots);
        robot_thread.borders(left_margin, right_margin, top_margin, bottom_margin);
        //std::lock_guard<std::mutex> lock(mutexRobots);
        mutexRobots.lock();
        Robots_output[Robot_id] = robot_thread;
        updated_Robots[Robot_id] = true;
        mutexRobots.unlock();

        coutMutex.lock();  
        cerr << "thread numero: "<< Robot_id <<" valore_update : "<< updated_Robots[Robot_id] <<" step  :" << step << endl;
        coutMutex.unlock();
    }  
}

void print_Robots(std::ofstream& outputFile, const int step){

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


int main() {
    std::srand(std::time(0));
    cerr << "inizio" << endl;
    int no_of_robot;
    cerr << "inserisci il numero di Robot: ";
    cin >> no_of_robot ;

    int left_margin, right_margin, top_margin, bottom_margin;
    cerr << "inserisci i confini: x_min, x_max , y_min, y_max " << endl;
    cin >> left_margin >> right_margin >> bottom_margin >> top_margin;

    // Crea 100 oggetti Robot con posizioni e velocità casuali
    for (int i = 0; i < no_of_robot; ++i) {
        float rand_x = static_cast<float>(std::rand() % right_margin); // Posizione x casuale
        float rand_y = static_cast<float>(std::rand() % top_margin); // Posizione y casuale
        float rand_vx = static_cast<float>(std::rand()) / RAND_MAX - 0.5; // Velocità x casuale tra -0.5 e 0.5
        float rand_vy = static_cast<float>(std::rand()) / RAND_MAX - 0.5; // Velocità y casuale tra -0.5 e 0.5

        Robots.push_back(Robot(rand_x, rand_y, rand_vx, rand_vy));
    }

    //cerr << "fine creazione robot di prova" << endl;
    cerr << "dimensione vettore "<<Robots.size()<< endl;

    Robots_output.resize(no_of_robot);
    updated_Robots.resize(no_of_robot, false);


    //salvataggio delle posizioni 
    const std::string filename = "coordinates.txt";
    if (std::filesystem::exists(filename)) {
        // If the file exists, remove it
        if (std::filesystem::remove(filename)) {
            std::cout << "Existing file removed." << std::endl;
        }
    }
    ofstream outputFile("coordinates.txt",ios::app);

    if (outputFile.is_open()) {
        outputFile << left_margin <<" " << right_margin <<" "<< bottom_margin << " " << top_margin << " ";
        outputFile << std::endl;

        for(int step = 0; step < 1000; step++){
            cerr << "ciclo numero"<< step << endl;
            vector<std::thread> Robs;
            for(int j = 0; j < no_of_robot; ++j){
                //std::string thread_name = "Rob" + std::to_string(j);
                Robs.push_back(std::thread(update_Position, j, step, left_margin, right_margin, top_margin, bottom_margin));
            }
            for (size_t z = 0; z < no_of_robot; ++z) {
                Robs[z].join();
            }
            cerr << "Tutte le Rob sono chiuse: "<< step << endl;
            // Inizia la thread per il salvataggio delle posizioni
            std::thread print_thread(print_Robots, std::ref(outputFile),step);
            
            print_thread.join();
            cerr << "print_thread è stata chiuse allo step: "<< step << endl;
        }  
        outputFile.close(); 
    }else {
            cerr << "Errore nell'apertura del file di output." << endl;
            return 1; // Termina il programma con un codice di errore
        }

    return 0;
}