#include "viewerogl.h"
#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
//#include <GL/gl.h>
//#include <GL/glut.h>
#endif
#include <math.h>

ViewerOGL::ViewerOGL(QWidget *parent) : QOpenGLWidget{parent} {
  userScale = 1;
  orto = true;
  lineColor.setRgbF(0, 1, 0, 1);
  pointColor.setRgbF(1, 0, 0, 1);
  backColor.setRgb(0, 0, 0, 1);
  pointPref = GL_POINT_SMOOTH;
  linePref = GL_LINE;
  initPoint(&model);
}

void ViewerOGL::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  ;
}

void ViewerOGL::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  if (width >= height) {
    heightK = (double)height / width;
    widthK = 1;
  } else {
    widthK = (double)width / height;
    heightK = 1;
  }
}

void ViewerOGL::paintGL() {
  if (model.array_points && model.count_point != 0 && !model.error) {
    glClearColor(((double)backColor.red() + 1) / 256,
                 ((double)backColor.green() + 1) / 256,
                 ((double)backColor.blue() + 1) / 256, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (orto) {
      glOrtho(-2 * widthK, 2 * widthK, -2 * heightK, 2 * heightK, -100, 100);
    } else {
      glFrustum(-1 * widthK, 1 * widthK, -1 * heightK, 1 * heightK, 1, 2000);
      glTranslatef(0.0, 0.0, -10);
    }
    glMatrixMode(GL_MODELVIEW);

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_DOUBLE, 0, model.array_points);
    glPointSize(pointSize);
    glEnable(pointPref);
    glColor3d(((double)pointColor.red() + 1) / 256,
              ((double)pointColor.green() + 1) / 256,
              ((double)pointColor.blue() + 1) / 256);
    if (pointPref != 0x0000) glDrawArrays(GL_POINTS, 0, model.count_point);
    glDisable(pointPref);

    glEnable(linePref);
    glLineStipple(2, linePref);  // Устанавливаем режим рисования линий
    glLineWidth(lineSize);
    glColor3d(((double)lineColor.red() + 1) / 256,
              ((double)lineColor.green() + 1) / 256,
              ((double)lineColor.blue() + 1) / 256);
    glDrawElements(GL_LINES, model.count_poly * 2, GL_UNSIGNED_INT,
                   model.array_poly);
    glDisable(linePref);

    glDisableClientState(GL_VERTEX_ARRAY);
  }
}

void ViewerOGL::modelNormalize(double value) {
  double tmp = std::fabs(model.array_points[0]);
  for (int i = 0; i < model.count_point; i++) {
    if (std::fabs(model.array_points[i]) > tmp)
      tmp = std::fabs(model.array_points[i]);
  }
  scale_data(&model, (double)value / tmp);
}

void ViewerOGL::newFile() {
  userScale = 1;
  userRotateX = 0;
  userRotateY = 0;
  userRotateZ = 0;
  userTranseX = 0;
  userTranseY = 0;
  userTranseZ = 0;
  kNorm = 1;
  update();
}

void ViewerOGL::restoreModel() {
  translation_data(&model, userTranseX, userTranseY, userTranseZ);

  rotation_data_ox(&model, userRotateX);
  rotation_data_oy(&model, userRotateY);
  rotation_data_oz(&model, userRotateZ);
}

void ViewerOGL::saveHW() {
  widgetH = this->size().height();
  widgetW = this->size().width();
}
