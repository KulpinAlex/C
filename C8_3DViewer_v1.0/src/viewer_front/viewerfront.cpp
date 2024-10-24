#include "viewerfront.h"

#include "./ui_desktop.h"
#include "gif.h"

GifWriter gifData;

ViewerFront::ViewerFront(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ViewerFront) {
  ui->setupUi(this);
  ui->labelFileName->setText("");
  tmr = new QTimer();
  tmrStep = new QTimer();
  ui->progressBar->setVisible(false);
  ui->horizontalSlider_fake->setVisible(false);

  connect(tmr, SIGNAL(timeout()), this, SLOT(saveGif()));
  connect(tmrStep, SIGNAL(timeout()), this, SLOT(saveFrame()));

  connect(ui->pushButtonOpenFile, SIGNAL(clicked()), this, SLOT(ReadFile()));
  connect(ui->doubleSpinBoxModelScale, SIGNAL(valueChanged(double)), this,
          SLOT(affinModelScale(double)));
  connect(ui->dialAffineScale, SIGNAL(valueChanged(int)), this,
          SLOT(affinSynclScale(int)));

  connect(ui->dialPointSize, SIGNAL(valueChanged(int)), this,
          SLOT(pointResize(int)));
  connect(ui->dialLineSize, SIGNAL(valueChanged(int)), this,
          SLOT(lineResize(int)));
  connect(ui->pushButtonColorLine, SIGNAL(clicked()), this,
          SLOT(getColorLine()));
  connect(ui->pushButtonColorPoints, SIGNAL(clicked()), this,
          SLOT(getColorPoint()));
  connect(ui->pushButton_BackColor, SIGNAL(clicked()), this,
          SLOT(getColorBack()));

  connect(ui->doubleSpinBoxAfRotateY, SIGNAL(valueChanged(double)), this,
          SLOT(affinRotateY(double)));
  connect(ui->doubleSpinBoxAfRotateX, SIGNAL(valueChanged(double)), this,
          SLOT(affinRotateX(double)));
  connect(ui->doubleSpinBoxAfRotateZ, SIGNAL(valueChanged(double)), this,
          SLOT(affinRotateZ(double)));

  connect(ui->horizontalSlider_Y, SIGNAL(valueChanged(int)), this,
          SLOT(affinSyncSliderY(int)));
  connect(ui->horizontalSlider_X, SIGNAL(valueChanged(int)), this,
          SLOT(affinSyncSliderX(int)));
  connect(ui->horizontalSlider_Z, SIGNAL(valueChanged(int)), this,
          SLOT(affinSyncSliderZ(int)));

  connect(ui->doubleSpinBox_TrancelateX, SIGNAL(valueChanged(double)), this,
          SLOT(affinTranse()));
  connect(ui->doubleSpinBox_TrancelateY, SIGNAL(valueChanged(double)), this,
          SLOT(affinTranse()));
  connect(ui->doubleSpinBox_TrancelateZ, SIGNAL(valueChanged(double)), this,
          SLOT(affinTranse()));

  connect(ui->pushButton_resetX, SIGNAL(clicked()), this, SLOT(resetTranseX()));
  connect(ui->pushButton_resetY, SIGNAL(clicked()), this, SLOT(resetTranseY()));
  connect(ui->pushButton_resetZ, SIGNAL(clicked()), this, SLOT(resetTranseZ()));

  connect(ui->radioButtonOrt, SIGNAL(clicked(bool)), this,
          SLOT(ortClick(bool)));
  connect(ui->radioButtonFrust, SIGNAL(clicked(bool)), this,
          SLOT(frustClick(bool)));

  connect(ui->radioButton_LineStripple, SIGNAL(clicked()), this,
          SLOT(changeStripLine()));
  connect(ui->radioButton_lineFull, SIGNAL(clicked()), this,
          SLOT(changeFullLine()));

  connect(ui->radioButton_pointCircle, SIGNAL(clicked()), this,
          SLOT(changeCirclePoint()));
  connect(ui->radioButton_pointSquare, SIGNAL(clicked()), this,
          SLOT(changeSquarePoint()));
  connect(ui->radioButton_pointNull, SIGNAL(clicked()), this,
          SLOT(changeNullPoint()));

  connect(ui->pushButton_saveFile, SIGNAL(clicked()), this, SLOT(saveFile()));
  //  tmr->start();
}

ViewerFront::~ViewerFront() {
  freePoint(&ui->openGLWidget->model);
  delete tmr;
  delete tmrStep;
  delete ui;
}

void ViewerFront::ReadFile() {
  pathToSample = settings.pathToFiles;
  QString fileName =
      QFileDialog::getOpenFileName(this, "Open file:", pathToSample);
  if (fileName != "") {
    resetUser();
    ui->openGLWidget->newFile();
    freePoint(&ui->openGLWidget->model);
    initPoint(&ui->openGLWidget->model);

    ui->labelFileName->setText(fileName);
    QStringList pieces = fileName.split("/");
    QString name = pieces.value(pieces.length() - 1);
    QByteArray ba = fileName.toLocal8Bit();
    char *c_str2 = ba.data();
    parser(c_str2, &ui->openGLWidget->model);
    pieces.removeLast();
    settings.pathToFiles = pieces.join("/");
    if (errMsgBox()) {
      ui->openGLWidget->modelNormalize(1);
      loadConfig();
      ui->openGLWidget->restoreModel();
      ui->doubleSpinBoxModelScale->setValue(1);

      ui->label_countLines->setText(
          QString::number(ui->openGLWidget->model.count_poly));
      ui->label_countPoints->setText(
          QString::number(ui->openGLWidget->model.count_point));
      ui->labelFileNameInf->setText(name);
      // for (int i=0;)

      drawModel();
    }

  } else {
    freePoint(&ui->openGLWidget->model);
    initPoint(&ui->openGLWidget->model);
  }
}

int ViewerFront::errMsgBox() {
  int res;
  QMessageBox msgBox;

  switch (ui->openGLWidget->model.error) {
    case SUCCESS:
      res = 1;
      break;
    case FILE_NOT_OPEN:
      msgBox.setText("Ошибка открытия файла.");
      msgBox.exec();
      res = 0;
      break;
    case POINTS_MORE_BILLION:
      msgBox.setText("Количество точек больше 1 000 000.");
      msgBox.exec();
      res = 0;
      break;
    case ALLOCATION_ERROR:
      msgBox.setText("Ошибка выделения памяти.");
      msgBox.exec();
      res = 0;
      break;
    case OBJ_MORE_ONE:
      msgBox.setText("Файл содежит больше одной модели.");
      msgBox.exec();
      res = 0;
      break;

    case ANOTHER_ERROR:
      msgBox.setText("Ошибка аффинных преобразований.");
      msgBox.exec();
      res = 0;
      break;
    default:
      break;
  }
  return res;
}

void ViewerFront::getColorLine() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Change...");
  if (color.isValid()) {
    ui->openGLWidget->lineColor = color;
    settings.lineColor = color;
    settings.saveConf();
  }
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::getColorPoint() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Change...");
  if (color.isValid()) {
    settings.pointColor = color;
    settings.saveConf();
    ui->openGLWidget->pointColor = color;
  }
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::getColorBack() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Change...");
  if (color.isValid()) {
    settings.backColor = color;
    settings.saveConf();
    ui->openGLWidget->backColor = color;
  }
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::pointResize(int value) {
  ui->openGLWidget->pointSize = (double)value / 50;
  settings.pointSize = ui->openGLWidget->pointSize;
  settings.saveConf();
  ui->openGLWidget->update();
}

void ViewerFront::lineResize(int value) {
  ui->openGLWidget->lineSize = (double)value / 100;
  settings.lineSize = ui->openGLWidget->lineSize;
  settings.saveConf();
  ui->openGLWidget->update();
}

void ViewerFront::affinModelScale(double value) {
  ui->openGLWidget->restoreModel();
  ui->dialAffineScale->setValue((int)(value * 100));
  drawModel();
}

void ViewerFront::affinSynclScale(int value) {
  ui->doubleSpinBoxModelScale->setValue((double)value / 100);
}

void ViewerFront::affinRotateY(double value) {
  ui->horizontalSlider_Y->setValue((int)value);
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::affinRotateX(double value) {
  ui->horizontalSlider_X->setValue((int)value);
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::affinRotateZ(double value) {
  ui->horizontalSlider_Z->setValue((int)value);
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::affinSyncSliderY(int value) {
  ui->doubleSpinBoxAfRotateY->setValue((double)value);
}
void ViewerFront::affinSyncSliderX(int value) {
  ui->doubleSpinBoxAfRotateX->setValue((double)value);
}
void ViewerFront::affinSyncSliderZ(int value) {
  ui->doubleSpinBoxAfRotateZ->setValue((double)value);
}

void ViewerFront::affinTranse() {
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::resetTranseX() {
  ui->doubleSpinBox_TrancelateX->setValue(0);
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::resetTranseY() {
  ui->doubleSpinBox_TrancelateY->setValue(0);
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::resetTranseZ() {
  ui->doubleSpinBox_TrancelateZ->setValue(0);
  ui->openGLWidget->restoreModel();
  drawModel();
}

void ViewerFront::ortClick(bool value) {
  ui->openGLWidget->restoreModel();
  ui->doubleSpinBoxModelScale->setValue(1);
  settings.ortoC = true;
  settings.saveConf();
  if (!ui->openGLWidget->orto) ui->openGLWidget->modelNormalize(1);

  ui->openGLWidget->orto = true;
  drawModel();
}
void ViewerFront::frustClick(bool value) {
  ui->openGLWidget->restoreModel();
  ui->doubleSpinBoxModelScale->setValue(1);
  settings.ortoC = false;
  settings.saveConf();
  if (ui->openGLWidget->orto) ui->openGLWidget->modelNormalize(3);
  ui->openGLWidget->orto = false;
  drawModel();
}

void ViewerFront::changeStripLine() {
  ui->openGLWidget->restoreModel();
  ui->openGLWidget->linePref = GL_LINE_STIPPLE;
  settings.linePref = GL_LINE_STIPPLE;
  settings.saveConf();
  drawModel();
}

void ViewerFront::changeFullLine() {
  ui->openGLWidget->restoreModel();
  ui->openGLWidget->linePref = GL_LINE;
  settings.linePref = GL_LINE;
  settings.saveConf();
  drawModel();
}

void ViewerFront::changeCirclePoint() {
  ui->openGLWidget->restoreModel();
  ui->openGLWidget->pointPref = GL_POINT_SMOOTH;
  settings.pointPref = GL_POINT_SMOOTH;
  settings.saveConf();
  drawModel();
}

void ViewerFront::changeSquarePoint() {
  ui->openGLWidget->restoreModel();
  ui->openGLWidget->pointPref = GL_POINT;
  settings.pointPref = GL_POINT;
  settings.saveConf();
  drawModel();
}

void ViewerFront::changeNullPoint() {
  ui->openGLWidget->restoreModel();
  ui->openGLWidget->pointPref = 0x0000;
  settings.pointPref = 0x0000;
  settings.saveConf();
  drawModel();
}

void ViewerFront::saveFile() {
  img = ui->openGLWidget->grabFramebuffer();
  if (ui->radioButton_gif->isChecked()) {
    timeToFin = 0;
    ui->progressBar->setVisible(true);
    ui->horizontalSlider_fake->setVisible(true);
    ui->pushButton_saveFile->setEnabled(false);
    //        tmr->start(5000);
    ui->openGLWidget->saveHW();
    ui->openGLWidget->resize(640, 480);
    setFixedSize(width(), height());
    QByteArray ba =
        (settings.pathToFiles + "/" + QTime::currentTime().toString() + ".gif")
            .toLocal8Bit();
    char *c_str2 = ba.data();
    GifBegin(&gifData, c_str2, 640, 480, 10);
    img = ui->openGLWidget->grabFramebuffer().scaled(
        640, 480, Qt::KeepAspectRatio, Qt::FastTransformation);
    GifWriteFrame(&gifData,
                  img.convertToFormat(QImage::Format_Indexed8)
                      .convertToFormat(QImage::Format_RGBA8888)
                      .bits(),
                  640, 480, 10);
    tmrStep->start(100);
  }
  if (ui->radioButton_jpg->isChecked()) {
    img.save(
        settings.pathToFiles + "/" + QTime::currentTime().toString() + ".jpg",
        "JPG");
  }
  if (ui->radioButton_bmp->isChecked()) {
    img.save(
        settings.pathToFiles + "/" + QTime::currentTime().toString() + ".bmp",
        "BMP");
  }
}

void ViewerFront::saveFrame() {
  tmrStep->start(100);
  timeToFin += 0.1;
  ui->label_timer->setText("Запись файла");
  ui->progressBar->setValue((int)(timeToFin * 10));
  img = ui->openGLWidget->grabFramebuffer().scaled(
      640, 480, Qt::KeepAspectRatio, Qt::FastTransformation);
  GifWriteFrame(&gifData,
                img.convertToFormat(QImage::Format_Indexed8)
                    .convertToFormat(QImage::Format_RGBA8888)
                    .bits(),
                640, 480, 10);

  if (timeToFin >= 5) saveGif();
}

void ViewerFront::saveGif() {
  GifEnd(&gifData);
  tmrStep->stop();
  tmr->stop();
  setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setMinimumSize(0, 0);
  ui->progressBar->setVisible(false);
  ui->openGLWidget->resize(ui->openGLWidget->widgetW,
                           ui->openGLWidget->widgetH);
  ui->label_timer->setText("-");
  ui->pushButton_saveFile->setEnabled(true);
  ui->horizontalSlider_fake->setVisible(false);
}

void ViewerFront::resetUser() {
  ui->radioButtonOrt->setChecked(true);
  ui->label_countPoints->setText(
      QString::number(ui->openGLWidget->model.count_point));
  ui->label_countLines->setText(
      QString::number(ui->openGLWidget->model.count_poly));

  ui->doubleSpinBoxModelScale->setValue(1);
  ui->doubleSpinBoxAfRotateX->setValue(0);
  ui->doubleSpinBoxAfRotateY->setValue(0);
  ui->doubleSpinBoxAfRotateZ->setValue(0);
  ui->doubleSpinBox_TrancelateX->setValue(0);
  ui->doubleSpinBox_TrancelateY->setValue(0);
  ui->doubleSpinBox_TrancelateZ->setValue(0);
}

void ViewerFront::drawModel() {
  scale_data(&ui->openGLWidget->model, ui->openGLWidget->userScale);
  double tmp = ui->openGLWidget->model.array_points[0];
  scale_data(&ui->openGLWidget->model, ui->doubleSpinBoxModelScale->value());
  ui->openGLWidget->userScale = tmp / ui->openGLWidget->model.array_points[0];

  rotation_data_oz(&ui->openGLWidget->model,
                   ui->doubleSpinBoxAfRotateZ->value() / 57.2957795131);
  rotation_data_oy(&ui->openGLWidget->model,
                   ui->doubleSpinBoxAfRotateY->value() / 57.2957795131);
  rotation_data_ox(&ui->openGLWidget->model,
                   ui->doubleSpinBoxAfRotateX->value() / 57.2957795131);

  translation_data(&ui->openGLWidget->model,
                   ui->doubleSpinBox_TrancelateX->value(),
                   ui->doubleSpinBox_TrancelateY->value(),
                   ui->doubleSpinBox_TrancelateZ->value());

  ui->openGLWidget->userRotateX =
      ui->doubleSpinBoxAfRotateX->value() / 57.2957795131 * (-1);
  ui->openGLWidget->userRotateY =
      ui->doubleSpinBoxAfRotateY->value() / 57.2957795131 * (-1);
  ui->openGLWidget->userRotateZ =
      ui->doubleSpinBoxAfRotateZ->value() / 57.2957795131 * (-1);

  ui->openGLWidget->userTranseX = ui->doubleSpinBox_TrancelateX->value() * (-1);
  ui->openGLWidget->userTranseY = ui->doubleSpinBox_TrancelateY->value() * (-1);
  ui->openGLWidget->userTranseZ = ui->doubleSpinBox_TrancelateZ->value() * (-1);
  ui->openGLWidget->update();
}

void ViewerFront::loadConfig() {
  settings.loadConf(ui->openGLWidget);
  if (ui->openGLWidget->orto) {
    ui->radioButtonOrt->setChecked(true);
    ortClick(true);
  } else {
    ui->openGLWidget->modelNormalize(3);
    frustClick(true);
    ui->radioButtonFrust->setChecked(true);
  }

  if (settings.linePref == GL_LINE_STIPPLE) {
    ui->radioButton_LineStripple->setChecked(true);
  } else {
    ui->radioButton_lineFull->setChecked(true);
  }

  switch (settings.pointPref) {
    case GL_POINT_SMOOTH:
      ui->radioButton_pointCircle->setChecked(true);
      break;
    case GL_POINT:
      ui->radioButton_pointSquare->setChecked(true);
      break;
    case 0x0000:
      ui->radioButton_pointNull->setChecked(true);
      break;
  }

  ui->dialLineSize->setValue((int)(settings.lineSize * 100));
  ui->dialPointSize->setValue((int)(settings.pointSize * 50));
}
