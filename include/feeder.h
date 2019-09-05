#ifndef FEEDER_H
#define FEEDER_H

#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <random>

class Feeder {
public:
    Feeder();
    Feeder(std::mutex *mutex, std::condition_variable *cond_var, bool *notified, bool *done, std::queue<double> *data);
    ~Feeder();
    void run();

private:
    std::mutex *mutex_;
    std::condition_variable *cond_var_;
    bool *notified_, *done_;
    std::queue<double> *data_;
    std::mt19937 gen_;
    std::uniform_real_distribution<double> dist_;
};

#endif
