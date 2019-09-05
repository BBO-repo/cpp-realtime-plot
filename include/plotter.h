#ifndef PLOTTER_H
#define PLOTTER_H

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <queue>

using namespace std;
#include <matplotpp/matplotpp.h>

inline void wait_on_enter()
{
  std::string input;
  std::cout << "Enter to continue..." << std::endl;
  std::getline(std::cin, input);
}

class Plotter : public MatPlot {
protected:
  static Plotter *instance;

public:
  Plotter();
  Plotter (std::mutex *mutex, std::condition_variable *cond_var, bool *notified, bool *done, std::queue<double> *data);
  virtual ~Plotter();

  int is_run = 1;
  void getNewData();
  void startPlotter(int argc, char *argv[]);

  // wrapping GLUT to c++ with singleton design pattern is based on http://paulsolt.com/blog/2009/07/openglglut-classes-oop-and-problems
  static void displayWrapper();
  static void reshapeWrapper(int w, int h);
  static void idleWrapper();
  static void mouseWrapper(int button, int state, int x, int y);
  static void motionWrapper(int x, int y);
  static void passiveWrapper(int x, int y);
  static void keyboardWrapper(unsigned char key, int x, int y);
  static void setInstance(Plotter *mp);

private:
  void DISPLAY();
  std::vector<double> x, z;

  std::mutex *mutex_;
  std::condition_variable *cond_var_;
  bool *notified_, *done_;
  std::queue<double> *data_;
};

#endif
