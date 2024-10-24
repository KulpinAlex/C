#ifndef VIEWERSETTINGS_H
#define VIEWERSETTINGS_H

#include <QColor>
#include <QObject>
#include <QSettings>

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
class ViewerSettings : public QObject {
  Q_OBJECT
 public:
  explicit ViewerSettings(QObject *parent = nullptr);
  QSettings settings;
  void saveConf();
  void loadConf(ViewerOGL *obj);
  QColor lineColor, pointColor, backColor;
  GLenum pointPref, linePref;
  double pointSize, lineSize;
  bool ortoC;
  QString pathToFiles;
 signals:
};

#endif  // VIEWERSETTINGS_H
