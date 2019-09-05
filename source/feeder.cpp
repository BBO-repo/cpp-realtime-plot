#include <iostream>
#include "feeder.h"

Feeder::Feeder() : mutex_(0), cond_var_(0), notified_(0), done_(0) {}

Feeder::Feeder(std::mutex *mutex, std::condition_variable *cond_var, bool *notified, bool *done, std::queue<double> *data)
: mutex_(mutex), cond_var_(cond_var), notified_(notified), done_(done), data_(data) {

  unsigned int seed = 0;//std::random_device{}()
  gen_.seed(seed);
  dist_ = std::uniform_real_distribution<double>(0.0,1.0);
}

Feeder::~Feeder() {}

void Feeder::run() {

  for (int i = 0; i < 1000; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::unique_lock<std::mutex> lock(*mutex_);
    data_->push(dist_(gen_));
    *(this->notified_) = true;
    (this->cond_var_)->notify_one();
  }

  *(this->done_) = true;
  (this->cond_var_)->notify_one();
}
