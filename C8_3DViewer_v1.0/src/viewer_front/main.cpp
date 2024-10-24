#include <QApplication>

#include "viewerfront.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_NUMERIC, "C");
  //    QSurfaceFormat
  ViewerFront w;
  w.show();
  return a.exec();
}
