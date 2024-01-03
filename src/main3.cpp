#include "Robot.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <algorithm>

class RobotMonitor {
public:
    RobotMonitor(const std::vector<Robot>& initialRobots)
        : Robots(initialRobots), updatedRobots(initialRobots.size(), false) {}

    void updatePosition(int Robot_id, const int step, const double left_margin,
                        const double right_margin, const double top_margin, const double bottom_margin) {
        std::unique_lock<std::mutex> lock(mutexRobots);

        if (!updatedRobots[Robot_id]) {
            Robot robot_thread = Robots[Robot_id];
            robot_thread.position_update(Robots);
            robot_thread.borders(left_margin, right_margin, top_margin, bottom_margin);
            Robots_output[Robot_id] = robot_thread;
            updatedRobots[Robot_id] = true;

            std::unique_lock<std::mutex> coutLock(coutMutex);
            std::cerr << "thread numero: " << Robot_id << " valore_update: " << updatedRobots[Robot_id]
                      << " step: " << step << std::endl;
        }
    }

    void savePositionsToFile(std::ofstream& outputFile, int step) {
        std::unique_lock<std::mutex> lock(mutexRobots);

        for (size_t i = 0; i < Robots_output.size(); ++i) {
            outputFile << Robots_output[i].pos_x_ << " " << Robots_output[i].pos_y_ << " ";
            updatedRobots[i] = false;
        }

        std::unique_lock<std::mutex> coutLock(coutMutex);
        std::cerr << "tutte le posizioni sono stampate: " << step << std::endl;
    }

private:
    std::mutex mutexRobots;
    std::mutex coutMutex;
    std::vector<Robot> Robots;
    std::vector<Robot> Robots_output;
    std::vector<bool> updatedRobots;
};

void updatePosition(RobotMonitor& monitor, int Robot_id, const int step, const double left_margin,
                    const double right_margin, const double top_margin, const double bottom_margin) {
    monitor.updatePosition(Robot_id, step, left_margin, right_margin, top_margin, bottom_margin);
}

void savePositionsToFile(RobotMonitor& monitor, std::ofstream& outputFile, int step) {
    monitor.savePositionsToFile(outputFile, step);
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));
    std::cerr << "inizio" << std::endl;

    int no_of_robot;
    std::cerr << "inserisci il numero di Robot: ";
    std::cin >> no_of_robot;

    int left_margin, right_margin, top_margin, bottom_margin;
    std::cerr << "inserisci i confini: x_min, x_max, y_min, y_max " << std::endl;
    std::cin >> left_margin >> right_margin >> bottom_margin >> top_margin;

    std::vector<Robot> Robots;
    std::vector<Robot> Robots_output(no_of_robot);
    std::vector<bool> updated_Robots(no_of_robot, false);

    for (int i = 0; i < no_of_robot; ++i) {
        float rand_x = static_cast<float>(std::rand() % right_margin);
        float rand_y = static_cast<float>(std::rand() % top_margin);
        float rand_vx = static_cast<float>(std::rand()) / RAND_MAX - 0.5;
        float rand_vy = static_cast<float>(std::rand()) / RAND_MAX - 0.5;

        Robots.push_back(Robot(rand_x, rand_y, rand_vx, rand_vy));
    }

    std::ofstream outputFile("coordinates.txt", std::ios::trunc);

    if (outputFile.is_open()) {
        outputFile << left_margin << " " << right_margin << " " << bottom_margin << " " << top_margin << " ";
        outputFile << std::endl;

        RobotMonitor monitor(Robots);

        std::vector<std::thread> threads;
        for (int step = 0; step < 1000; step++) {
            std::cerr << "ciclo numero" << step << std::endl;

            threads.clear();
            for (int j = 0; j < no_of_robot; ++j) {
                threads.push_back(std::thread(updatePosition, std::ref(monitor), j, step, left_margin,
                                              right_margin, top_margin, bottom_margin));
            }

            for (size_t z = 0; z < no_of_robot; ++z) {
                threads[z].join();
            }

            std::cerr << "tutte le thread sono chiuse: " << step << std::endl;

            std::thread saveThread(savePositionsToFile, std::ref(monitor), std::ref(outputFile), step);
            saveThread.join();

            for (size_t t = 0; t < Robots_output.size(); ++t) {
                Robots[t] = Robots_output[t];
                std::cerr << "Robots modifica elemento numero: " << t << " : " << step << std::endl;
            }

            std::cerr << "il vettore Robots è stato modificato: " << step << std::endl;
            outputFile << std::endl;
        }

        outputFile.close();
    } else {
        std::cerr << "Errore nell'apertura del file di output." << std::endl;
        return 1;
    }

    return 0;
}
