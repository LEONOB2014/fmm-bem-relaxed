#include "LaplaceSphericalBEM.hpp"
#include "BioMeshReader.hpp"
#include "BEMConfig.hpp"

#include <GL/glut.h>

std::vector<LaplaceSphericalBEM::source_type> disp_panels;
float scale_window;
double omega;

void display()
{
  typedef LaplaceSphericalBEM::point_type point_type;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glRotatef(omega, -0.2, 1., 0.);
  float scale_color = 1;

  for (auto p : disp_panels) {
    glBegin(GL_TRIANGLES);
    glVertex3f(p.vertices[0][0]/scale_window, p.vertices[0][1]/scale_window, p.vertices[0][2]/scale_window);
    glVertex3f(p.vertices[1][0]/scale_window, p.vertices[1][1]/scale_window, p.vertices[1][2]/scale_window);
    glVertex3f(p.vertices[2][0]/scale_window, p.vertices[2][1]/scale_window, p.vertices[2][2]/scale_window);
    glColor3f(1,1,1);
    glEnd();
  }
  glutSwapBuffers();
  glFlush();
}

void animate()
{
  omega -= .1;
  glutPostRedisplay();
}

void init()
{
  glClearColor(0.,0.,0.2,1.);
  glClearDepth(1.);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
}

int main(int argc, char **argv)
{
  const char *vert_file = "test.vert";
  const char *face_file = "test.face";

  typedef LaplaceSphericalBEM kernel_type;
  typedef kernel_type::point_type point_type;
  typedef kernel_type::source_type source_type;

  typedef BioMeshReader<point_type, source_type> reader;

  // necessary to create panels
  BEMConfig::Init();
  auto config = BEMConfig::Instance();
  config->setK(3);

  std::vector<source_type> panels;
  reader::ReadMesh(vert_file, face_file, panels);

  // display panels
  disp_panels = panels;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(400,400);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Molecule");
  glutDisplayFunc(display);
  glutIdleFunc(animate);
  glEnable(GL_DEPTH_TEST);
  omega = 0;
  scale_window = 1;
  init();

  glutMainLoop();
}

