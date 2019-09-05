#include "plotter.h"

Plotter *Plotter::instance = 0;

Plotter::Plotter() : mutex_(0), cond_var_(0), notified_(0), done_(0), data_(0) {}

Plotter::Plotter(std::mutex *mutex, std::condition_variable *cond_var, bool *notified, bool *done, std::queue<double> *data)
    : mutex_(mutex), cond_var_(cond_var), notified_(notified), done_(done), data_(data) {}

Plotter::~Plotter() {}

void Plotter::setInstance(Plotter *mp) {
  instance = mp;
}

void Plotter::displayWrapper() {
  instance->display();
}

void Plotter::reshapeWrapper(int w, int h) {
  instance->reshape(w, h);
}

void Plotter::idleWrapper() {
  glutPostRedisplay();

  std::unique_lock<std::mutex> lock((*instance->mutex_));

  while (!(*(instance->notified_)))
  { // loop to avoid spurious wakeups
    (instance->cond_var_)->wait(lock);
  }
  instance->getNewData();
  *(instance->notified_) = false;

  if (*(instance->done_))
  {
    glutDestroyWindow(glutGetWindow());
  }
}

void Plotter::mouseWrapper(int button, int state, int x, int y) {
  instance->mouse(button, state, x, y);
}

void Plotter::motionWrapper(int x, int y) {
  instance->motion(x, y);
}

void Plotter::passiveWrapper(int x, int y) {
  instance->passivemotion(x, y);
}

void Plotter::keyboardWrapper(unsigned char key, int x, int y) {
  instance->keyboard(key, x, y);
  if (key == 'r') {
    if (instance->is_run == 0) {
      instance->is_run = 1;
    } else {
      instance->is_run = 0;
    }
  }
}

void Plotter::startPlotter(int argc, char *argv[]) {

  // Initialize GLUT
  glutInit(&argc, argv);
  glutCreateWindow(100, 100, 700, 600);
  glutSetWindowTitle("c++ real time plot");

  // Function callbacks
  glutDisplayFunc(displayWrapper);
  glutReshapeFunc(reshapeWrapper);
  glutMotionFunc(motionWrapper);
  glutMouseFunc(mouseWrapper);
  glutPassiveMotionFunc(passiveWrapper);
  glutKeyboardFunc(keyboardWrapper);

  // Loops
  glutIdleFunc(idleWrapper); // The program run loop
  glutMainLoop();            // Start the main GLUT thread
}

void Plotter::DISPLAY() {
  //plot
  title("press r to run and stop");
  plot(x, z);
  xlabel("x - samples");
  ylabel("y - random noise");
  grid(1);
}

void Plotter::getNewData() {
  if (is_run) {
    while (!data_->empty()) {
      x.push_back(x.size() + 1);
      z.push_back(data_->front());
      data_->pop();
    }
  }
}
