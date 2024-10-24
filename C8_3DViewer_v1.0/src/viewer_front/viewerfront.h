#ifndef VIEWERFRONT_H
#define VIEWERFRONT_H

#include <math.h>

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <QtOpenGL>
//#include "gif.h"

#include "viewersettings.h"

extern "C" {
#include "../viewer_back/3dviewer.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class ViewerFront;
}
QT_END_NAMESPACE

// typedef struct {
//   FILE* f;
//   uint8_t* oldImage;
//   bool firstFrame;
// } GifWriter;

class ViewerFront : public QMainWindow {
  Q_OBJECT

 public:
  ViewerFront(QWidget *parent = nullptr);

  ViewerSettings settings;
  //  GifWriter gifData;
  ~ViewerFront();

 private slots:
  void ReadFile();

  void pointResize(int value);
  void lineResize(int value);

  void affinRotateY(double value);
  void affinRotateX(double value);
  void affinRotateZ(double value);

  void affinSyncSliderY(int value);
  void affinSyncSliderX(int value);
  void affinSyncSliderZ(int value);

  void affinTranse();

  void resetTranseX();
  void resetTranseY();
  void resetTranseZ();

  void affinModelScale(double value);
  void affinSynclScale(int value);

  void ortClick(bool value);
  void frustClick(bool value);

  void getColorLine();
  void getColorPoint();
  void getColorBack();

  void changeStripLine();
  void changeFullLine();

  void changeCirclePoint();
  void changeSquarePoint();
  void changeNullPoint();

  void saveGif();
  void saveFrame();
  void saveFile();

 private:
  QString pathToSample;
  void resetUser();
  void drawModel();
  void loadConfig();
  Ui::ViewerFront *ui;
  QTimer *tmr, *tmrStep;
  QImage img;
  double timeToFin;
  int errMsgBox();
};
#endif  // VIEWERFRONT_H
