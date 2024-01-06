#include "Robot.h"
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <algorithm>
#include <condition_variable>
#include <atomic>
#include <filesystem>
#include <chrono>

std::mutex mutex_update_position;

std::vector<Robot> Robots;
std::vector<Robot> Robots_output;
std::vector<bool> updated_Robots;

std::atomic<bool> ready(false);

std::condition_variable all_Robot;
std::condition_variable all_threads_ready;
std::condition_variable all_threads_printed;

bool allTrue(false);

bool checkAllTrue() {
    return std::all_of(updated_Robots.begin(), updated_Robots.end(), [](bool element) {
        return element;
    });
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

void updatePosition(int Robot_id, const int left_margin, const int right_margin, const int top_margin, const int bottom_margin) {
   
    std::unique_lock<std::mutex> lock_rob(mutex_update_position);
    for (int step_thread = 0; step_thread < 1000; step_thread++) 
    {
        Robot robot_thread = Robots[Robot_id];
        robot_thread.position_update(Robots);
        robot_thread.borders(left_margin, right_margin, top_margin, bottom_margin);     
        Robots_output[Robot_id] = robot_thread;
        updated_Robots[Robot_id] = true;    
        allTrue = checkAllTrue(); //Check if all threads have updated
        if (allTrue){
            //The last active thread (Robot) initiates the printing, stops, and when the printing is complete, it wakes up the other threads (Robots).
            ready.store(true);
            all_Robot.notify_one();
            all_threads_printed.wait(lock_rob);
            all_threads_ready.notify_all();
        } 
        else {
            //The threads (Robots) wait until all of them have finished the same step before proceeding to the next one
            all_threads_ready.wait(lock_rob);
        }
    }
}

void savePositions(std::ofstream& outputFile) {
   
   std::unique_lock<std::mutex> lock_print(mutex_update_position);
   for(int step_pthread = 0; step_pthread < 1000; step_pthread++){
        if(!ready.load())
            all_Robot.wait(lock_print);
        for (size_t i = 0; i < Robots_output.size(); ++i) {
            outputFile << Robots_output[i].pos_x_ << " " << Robots_output[i].pos_y_ << " ";
            updated_Robots[i] = false;
        }
        outputFile << std::endl;
        for (size_t t = 0; t < Robots_output.size(); ++t) {
            Robots[t] = Robots_output[t];
        }
        ready.store(false);
        allTrue = false;
        print_progress_bar(step_pthread, 999); //Function to generate a progress bar
        all_threads_printed.notify_all(); 
   }
}

int main() {
    
    int no_of_robot;
    cout << "Enter the number of Robots: ";
    cin >> no_of_robot ;
    int left_margin, right_margin, top_margin, bottom_margin;
    cout << "Enter the boundaries: x_min, x_max, y_min, y_max " << endl;
    cin >> left_margin >> right_margin >> bottom_margin >> top_margin;

    
    //Creation of a vector of robots with random positions and velocities
    std::srand(std::time(0));
    for (int i = 0; i < no_of_robot; ++i) {
        double rand_x = static_cast<double>(std::rand() % right_margin);    // Random x position
        double rand_y = static_cast<double>(std::rand() % top_margin);      // Random y position
        double rand_vx = 1.0 + static_cast<double>(std::rand()) / (RAND_MAX / 4.0); // Random x velocity 1 a 5
        double rand_vy = 1.0 + static_cast<double>(std::rand()) / (RAND_MAX / 4.0); // Random y velocity 1 a 5

        Robots.push_back(Robot(rand_x, rand_y, rand_vx, rand_vy));
    }
    Robots_output.resize(no_of_robot);
    updated_Robots.resize(no_of_robot, false);
    std::vector<std::thread> Robs;
    for (int j = 0; j < no_of_robot; ++j) {
        Robs.push_back(std::thread(updatePosition, j, left_margin, right_margin, top_margin, bottom_margin));
    }

    const std::string filename = "coordinates.txt";
    if (std::filesystem::exists(filename)) {
        // If the file exists, remove it
        if (std::filesystem::remove(filename)) {
            cout << "Existing file removed." << endl;
        }
    }
    std::ofstream outputFile("coordinates.txt", std::ios::app);
    cout << "File has been created." << endl;

    if (outputFile.is_open()) {
        outputFile << left_margin <<" " << right_margin <<" "<< bottom_margin << " " << top_margin << " ";
        outputFile << endl;
        for (size_t i = 0; i < Robots.size(); ++i) {
            outputFile << Robots[i].pos_x_ << " " << Robots[i].pos_y_ << " ";
        }
        outputFile << endl; 
    }else{
        cout << "Error opening the output file." << endl;
        return 1;
    }
    std::thread print_thread(savePositions, std::ref(outputFile));
    for (size_t z = 0; z < no_of_robot; ++z) {
        Robs[z].join();
    }
    print_thread.join();
    cout << " " << endl;
    outputFile.close();
    return 0;
}
