#include "plotter.h"

MP *MP::instance = 0;

MP::MP() : mutex_(0), cond_var_(0), notified_(0), done_(0) {}

MP::MP(std::mutex *mutex, std::condition_variable *cond_var, bool *notified, bool *done)
    : mutex_(mutex), cond_var_(cond_var), notified_(notified), done_(done) {}

MP::~MP() {}

void MP::setInstance(MP *mp)
{
  instance = mp;
}

void MP::displayWrapper()
{
  instance->display();
}

void MP::reshapeWrapper(int w, int h)
{
  instance->reshape(w, h);
}

void MP::idleWrapper()
{
  glutPostRedisplay();

  std::unique_lock<std::mutex> lock((*instance->mutex_));

  while (!(*(instance->notified_)))
  { // loop to avoid spurious wakeups
    (instance->cond_var_)->wait(lock);
  }
  instance->getNewData();
  *(instance->notified_) = false;

  if( *(instance->done_) ) {
    glutDestroyWindow(glutGetWindow());
  }
}

void MP::mouseWrapper(int button, int state, int x, int y)
{
  instance->mouse(button, state, x, y);
}

void MP::motionWrapper(int x, int y)
{
  instance->motion(x, y);
}

void MP::passiveWrapper(int x, int y)
{
  instance->passivemotion(x, y);
}

void MP::keyboardWrapper(unsigned char key, int x, int y)
{
  instance->keyboard(key, x, y);
  if (key == 'r')
  {
    if (instance->is_run == 0)
    {
      instance->is_run = 1;
    }
    else
    {
      instance->is_run = 0;
    }
  }
}

void MP::startPlotter(int argc, char *argv[])
{

  // Initialize GLUT
  glutInit(&argc, argv);
  glutCreateWindow(100, 100, 400, 300);

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

void MP::DISPLAY()
{
  //plot
  title("press r to run and stop");
  plot(x, z);
}

void MP::getNewData()
{

  if (is_run)
  {

    t += 0.02;
    x.push_back(x.size() + 1);
    z.push_back(sin(3 * x.back() + t));

    // int n = 10;
    // for (int i = 0; i < n; ++i)
    // {
    //   t += 0.02;
    //   x.push_back(x.size() + 1);
    //   z.push_back(sin(3 * x[i] + t));
    // }
  }
}
