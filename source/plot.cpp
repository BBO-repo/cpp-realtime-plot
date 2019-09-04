#include <thread>
#include <chrono>
#include <iostream>
#include <queue>

#include "plotter.h"
#include "feeder.h"

int main(int argc, char *argv[])
{
  // condition variable https://fr.cppreference.com/w/cpp/thread/condition_variable
  //std::queue<int> produced_nums;
  std::mutex m;
  std::condition_variable cond_var;
  bool done = false;
  bool notified = false;
  std::thread producer([&]() {
    for (int i = 0; i < 1000; ++i)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::unique_lock<std::mutex> lock(m);
      //std::cout << "producing " << i << '\n';
      notified = true;
      cond_var.notify_one();
    }

    done = true;
    cond_var.notify_one();
  });

  MP *mp = new MP(&m,&cond_var,&notified, &done);
  mp->setInstance(mp);
  mp->startPlotter(argc, argv);

  delete mp;

  producer.join();

  return 0;
}
