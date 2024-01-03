#include "Robot.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <algorithm>
#include <condition_variable>
#include <atomic>

std::mutex mutex_update;
std::mutex mutex_Robots;
std::mutex coutMutex;
std::mutex mutex_print;

std::vector<Robot> Robots;
std::vector<Robot> Robots_output;
std::vector<bool> updated_Robots;
//std::vector<std::thread> threads;
std::atomic<bool> ready = false;

bool isUpdateReady = false;
bool isPrintReady = false;

int current_Step;

std::condition_variable all_Robot_update;
std::condition_variable all_Robot_printed;

void updatePosition(int Robot_id, const int left_margin, const int right_margin, const int top_margin, const int bottom_margin) {
    
    
    for(int step_thread = 0; step_thread < 10; step_thread++){

        Robots[Robot_id].position_update(Robots);
        Robots[Robot_id].borders(left_margin, right_margin, top_margin, bottom_margin);
        Robots_output[Robot_id] = Robots[Robot_id];
        updated_Robots[Robot_id] = true;
        {
            std::unique_lock<std::mutex> lock(mutex_update);
            std::cout << "Rob " << Robot_id << " - Step " << step_thread << std::endl;
        //    if (step_thread == 9){
        //        ready.store(true) ;
            current_Step = step_thread;
            // Verifica se tutti gli elementi del vettore sono true
            bool allTrue = std::all_of(updated_Robots.begin(), updated_Robots.end(), [](bool element) {
                return element;
            });
            if (allTrue) {
                ready.store(true) ;
                std::cout << "Tutti gli elementi del vettore sono veri." << std::endl;
                all_Robot_printed.notify_all();
            }else if (updated_Robots[Robot_id]){
               std::cout << "Rob " << Robot_id << " si è bloccato- Step " << step_thread << std::endl;
                // Sblocca manualmente il mutex prima della fine dello scope
                lock.unlock();
                std::unique_lock<std::mutex> lock(mutex_update);
                all_Robot_printed.wait(lock, [&] { return !ready.load(); });
                ready.store(false);
            }

        }
        {
            std::unique_lock<std::mutex> lock(mutex_update);
            all_Robot_printed.wait(lock, [&] { return !ready.load(); });
            ready.store(false);
        }
    }

}

void savePositions(std::ofstream& outputFile) {
    for(int step_thread = 0; step_thread < 10; ++step_thread){

        std::unique_lock<std::mutex> lock(mutex_update);
        all_Robot_printed.wait(lock, [&] { return ready.load() && current_Step == step_thread; });

        for (size_t i = 0; i < Robots_output.size(); ++i) {
            outputFile << Robots_output[i].pos_x_ << " " << Robots_output[i].pos_y_ << " ";
            updated_Robots[i] = false;
        }
        outputFile << std::endl;

        coutMutex.lock();
        cerr << "Tutte le posizioni sono stampate: " << endl;
        coutMutex.unlock();

        for(size_t t = 0; t< Robots_output.size(); ++t){
            Robots[t] = Robots_output[t];
        }
        coutMutex.lock();
        cerr << "quanto vale steps nella stampa:  " <<  current_Step << endl;
        coutMutex.unlock();

        ready.store(false);
        cerr << "Ready: "<< ready.load() << endl;
        all_Robot_printed.notify_all();
        
    }
}

int main() {
    std::srand(std::time(0));
    cerr << "inizio" << endl;
    int no_of_robot{10};
    //cerr << "inserisci il numero di Robot: ";
    //cin >> no_of_robot;

    int left_margin{0}, right_margin{100}, top_margin{100}, bottom_margin{0};
    //cerr << "inserisci i confini: x_min, x_max , y_min, y_max " << endl;
    //cin >> left_margin >> right_margin >> bottom_margin >> top_margin;

    // Crea 100 oggetti Robot con posizioni e velocità casuali
    for (int i = 0; i < no_of_robot; ++i) {
        float rand_x = static_cast<float>(std::rand() % right_margin); // Posizione x casuale
        float rand_y = static_cast<float>(std::rand() % top_margin);    // Posizione y casuale
        float rand_vx = static_cast<float>(std::rand()) / RAND_MAX - 0.5; // Velocità x casuale tra -0.5 e 0.5
        float rand_vy = static_cast<float>(std::rand()) / RAND_MAX - 0.5; // Velocità y casuale tra -0.5 e 0.5

        Robots.push_back(Robot(rand_x, rand_y, rand_vx, rand_vy));
    }

    cerr << "dimensione vettore " << Robots.size() << endl;

    Robots_output.resize(no_of_robot);
    updated_Robots.resize(no_of_robot, false);

    std::vector<std::thread> Rob;

    // Inizia le thread di aggiornamento delle posizioni
    for (int j = 0; j < no_of_robot; ++j) {
        Rob.push_back(std::thread(updatePosition, j, left_margin, right_margin, top_margin, bottom_margin));
    }

    std::ofstream outputFile("coordinates.txt", std::ios::app);
    if (!outputFile.is_open()) {
        std::cerr << "Errore nell'apertura del file di output." << std::endl;
        return 1;  // Esce se non è possibile aprire il file
    }else{
        outputFile << left_margin <<" " << right_margin <<" "<< bottom_margin << " " << top_margin << " ";
        outputFile << std::endl;
        for (size_t i = 0; i < Robots.size(); ++i) {
            outputFile << Robots[i].pos_x_ << " " << Robots[i].pos_y_ << " ";
        }
        outputFile << std::endl;

    }
    // Inizia la thread per il salvataggio delle posizioni
    std::thread print_thread(savePositions, std::ref(outputFile));

    // Attendi che tutte le thread di aggiornamento delle posizioni siano chiuse
    for (size_t z = 0; z < no_of_robot; ++z) {
        Rob[z].join();
    }

    // Attendi che la thread di salvataggio delle posizioni sia chiusa
    print_thread.join();
    cerr << "Tutte le Rob sono chiuse." << endl;

    outputFile.close();
    cerr << "Tutte le thread sono chiuse." << endl;

    return 0;
}
