#ifndef VIEWEROGL_H
#define VIEWEROGL_H

#include <QColor>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>
#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
//#include <GL/gl.h>
//#include <GL/glut.h>
#endif
extern "C" {
#include "../viewer_back/3dviewer.h"
}

class ViewerOGL : public QOpenGLWidget, public QOpenGLFunctions {
  Q_OBJECT
 public:
  explicit ViewerOGL(QWidget *parent = nullptr);
  void newFile();
  void restoreModel();
  void modelNormalize(double value);
  Points model;
  double pointSize = 0.1;
  double lineSize = 0.1;
  double userScale, userRotateX, userRotateY, userRotateZ, userTranseX,
      userTranseY, userTranseZ, kNorm;
  bool orto;
  QColor lineColor, pointColor, backColor;
  GLenum pointPref, linePref;
  double widthK, heightK;
  int widgetH, widgetW;
  void saveHW();

 private:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
};

#endif  // VIEWEROGL_H
