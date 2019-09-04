#ifndef PLOTTER_H
#define PLOTTER_H

#include <iostream>
#include <condition_variable>
#include <mutex>

using namespace std;
#include <matplotpp/matplotpp.h>

inline void wait_on_enter()
{
  std::string input;
  std::cout << "Enter to continue..." << std::endl;
  std::getline(std::cin, input);
}

class MP : public MatPlot
{
protected:
  static MP *instance;

public:
  MP();
  MP (std::mutex *mutex, std::condition_variable *cond_var, bool *notified, bool *done);
  ~MP();

  int is_run = 1;
  void getNewData();
  void startPlotter(int argc, char *argv[]);

  // wrapping GLUT to c++ with singleton design pater is based on http://paulsolt.com/blog/2009/07/openglglut-classes-oop-and-problems
  static void displayWrapper();
  static void reshapeWrapper(int w, int h);
  static void idleWrapper();
  static void mouseWrapper(int button, int state, int x, int y);
  static void motionWrapper(int x, int y);
  static void passiveWrapper(int x, int y);
  static void keyboardWrapper(unsigned char key, int x, int y);
  static void setInstance(MP *mp);

private:
  void DISPLAY();
  double t = 0; // time
  dvec x, z;

  std::mutex *mutex_;
  std::condition_variable *cond_var_;
  bool *notified_, *done_;
};
#endif
