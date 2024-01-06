#include "Robot.h"
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <cstdlib>  // Per la funzione rand()
#include <ctime>    
#include <thread>
#include <mutex>
#include <filesystem>

std::mutex mutex_Robots;   

vector<Robot> Robots;
vector<Robot> Robots_output;
vector<bool> updated_Robots;

void update_Position(int Robot_id, const int step, const double left_margin,const double right_margin,const double top_margin,const double bottom_margin ){
    
    Robot robot_thread = Robots[Robot_id];
    if(!updated_Robots[Robot_id]){
        mutex_Robots.lock();
        robot_thread.position_update(Robots);
        robot_thread.borders(left_margin, right_margin, top_margin, bottom_margin);
        Robots_output[Robot_id] = robot_thread;
        updated_Robots[Robot_id] = true;
        mutex_Robots.unlock();
    }  
}

void print_Robots(std::ofstream& outputFile, const int step){

    for(size_t i = 0; i< Robots_output.size(); ++i){
        outputFile << Robots_output[i].pos_x_ <<" "<< Robots_output[i].pos_y_<< " ";
        updated_Robots[i] = false;        
    }
    for(size_t t = 0; t< Robots_output.size(); ++t){
        Robots[t] = Robots_output[t];
    }
    outputFile << endl;
}

void print_progress_bar(int progress, int total) {
    const int bar_width = 50;
    float percentage = static_cast<float>(progress) / total;
    int progress_bar_width = static_cast<int>(percentage * bar_width);
    cout << "[";
    for (int i = 0; i < bar_width; ++i) {
        if (i < progress_bar_width) {
                cout << "=";
        } else {
                cout << " ";
        }
    }
    cout << "] " << static_cast<int>(percentage * 100.0) << "%\r";
    cout.flush();
}

int main() {
   
    int no_of_robot;
    cout << "Enter the number of Robots: ";
    cin >> no_of_robot ;

    int left_margin{0}, right_margin{1000}, top_margin{1000}, bottom_margin{0};
    
    // Creation of a vector of robots with random positions and velocities
    std::srand(std::time(0));
    for (int i = 0; i < no_of_robot; ++i) {
        double rand_x =  static_cast<double>(std::rand() % right_margin);   // Random x position
        double rand_y =  static_cast<double>(std::rand() % top_margin);     // Random y position
        double rand_vx = 1.0 + static_cast<double>(std::rand()) / (RAND_MAX / 4.0); // Random x velocity 1 a 5
        double rand_vy = 1.0 + static_cast<double>(std::rand()) / (RAND_MAX / 4.0); // Random y velocity 1 a 5  

        Robots.push_back(Robot(rand_x, rand_y, rand_vx, rand_vy));
    }
    Robots_output.resize(no_of_robot);
    updated_Robots.resize(no_of_robot, false);
    const std::string filename = "coordinates.txt";
    if (std::filesystem::exists(filename)) {
        // If the file exists, remove it
        if (std::filesystem::remove(filename)) {
            cout << "Existing file removed." << endl;
        }
    }
    std::ofstream outputFile("coordinates.txt",ios::app);
    cout << "File has been created." << endl;
    if (outputFile.is_open()) {
        outputFile << left_margin <<" " << right_margin <<" "<< bottom_margin << " " << top_margin << " ";
        outputFile << endl;
        for(int step = 0; step < 1000; step++){
            print_progress_bar(step, 999); //Function to generate a progress bar
            vector<std::thread> Robs;
            for(int j = 0; j < no_of_robot; ++j){
                Robs.push_back(std::thread(update_Position, j, step, left_margin, right_margin, top_margin, bottom_margin));
            }
            for (size_t z = 0; z < no_of_robot; ++z) {
                Robs[z].join();
            }
            std::thread print_thread(print_Robots, std::ref(outputFile),step);    
            print_thread.join();
        }  
        outputFile.close(); 
    }else {
        cerr << "Error opening the output file." << endl;
        return 1; //Exit the program with an error message
    }
    cout << " " << endl;
    return 0;
}