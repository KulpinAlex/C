#include "viewersettings.h"

ViewerSettings::ViewerSettings(QObject *parent) : QObject{parent} {
  QSettings settings("3dviewer", "conf");

  settings.beginGroup("ConfValue");

  if (settings.contains("lineColor")) {
    lineColor = settings.value("lineColor").value<QColor>();
  } else {
    lineColor.setRgbF(0, 1, 0, 1);
  }

  if (settings.contains("pointColor")) {
    pointColor = settings.value("pointColor").value<QColor>();
  } else {
    pointColor.setRgbF(1, 0, 0, 1);
  }

  if (settings.contains("backColor")) {
    backColor = settings.value("backColor").value<QColor>();
  } else {
    backColor.setRgb(0, 0, 0, 1);
  }

  if (settings.contains("orto")) {
    ortoC = settings.value("orto").value<bool>();
  } else {
    ortoC = true;
  }

  if (settings.contains("pointSize")) {
    pointSize = settings.value("pointSize").value<double>();
  } else {
    pointSize = 0.5;
  }

  if (settings.contains("lineSize")) {
    lineSize = settings.value("lineSize").value<double>();
  } else {
    lineSize = 0.5;
  }

  if (settings.contains("pointPref")) {
    pointPref = settings.value("pointPref").value<GLenum>();
  } else {
    pointPref = GL_POINT;
  }

  if (settings.contains("linePref")) {
    linePref = settings.value("linePref").value<GLenum>();
  } else {
    linePref = GL_LINE;
  }

  if (settings.contains("pathToFiles")) {
    pathToFiles = settings.value("pathToFiles").value<QString>();
  } else {
    pathToFiles = "~/";
  }

  settings.endGroup();
  qDebug() << QString::number(linePref);
  qDebug() << QString::number(pointPref);
}

void ViewerSettings::saveConf() {
  QSettings settings("3dviewer", "conf");
  settings.beginGroup("ConfValue");
  settings.setValue("lineColor", lineColor);
  settings.setValue("pointColor", pointColor);
  settings.setValue("backColor", backColor);
  settings.setValue("orto", ortoC);
  settings.setValue("pointSize", pointSize);
  settings.setValue("lineSize", lineSize);
  settings.setValue("pointPref", pointPref);
  settings.setValue("linePref", linePref);
  settings.setValue("pathToFiles", pathToFiles);
  settings.endGroup();
}

void ViewerSettings::loadConf(ViewerOGL *obj) {
  obj->lineColor = lineColor;
  obj->pointColor = pointColor;
  obj->backColor = backColor;
  obj->orto = ortoC;
  obj->pointSize = pointSize;
  obj->lineSize = lineSize;
  obj->linePref = linePref;
  obj->pointPref = pointPref;
}