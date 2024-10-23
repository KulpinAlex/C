#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_sum, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_sub, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_lbkt, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_rbkt, SIGNAL(clicked()), this, SLOT(digits_input()));
  connect(ui->pushButton_clear, SIGNAL(clicked()), this, SLOT(clear_button()));
  connect(ui->pushButton_res, SIGNAL(clicked()), this, SLOT(result_button()));
  connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(digits_input()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::digits_input() {
  if (ui->label->text() == "expression error") ui->label->clear();
  QPushButton *button = (QPushButton *)sender();
  QString temp = (ui->label->text() + button->text());
  ui->label->setText(temp);
}

void MainWindow::clear_button() {
  ui->label->clear();
  ui->label->setText("");
}

void MainWindow::result_button() {
  double r = 0.0;
  double x = ui->doubleSpinBox_x->value();
  QString input = ui->label->text();
  input = input.replace(".", ",");
  int err = s21_smartcalc(input.toLocal8Bit().data(), x, &r);
  QString result;
  if (!err) {
    result = QString::number(r, 'f', 7);
  } else {
    result = "expression error";
  }
  ui->label->setText(result);
}
