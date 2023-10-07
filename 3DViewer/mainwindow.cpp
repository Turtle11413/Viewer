#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  timer = new QTimer(0);
  settings = new QSettings("Settings.ini", "Settings", this);
  ui->scale_value->setValue(ui->openGLWidget->scale_val);
  loadSettings();
  connect(timer, SIGNAL(timeout()), this, SLOT(startTimergif()));
}

MainWindow::~MainWindow() {
  ui->openGLWidget->reset_transformation();
  saveSettings();
  delete settings;
  free_data_gg(&ui->openGLWidget->data_1);
  timer->stop();
  delete timer;
  delete ui;
}

void MainWindow::on_open_file_clicked() {
  QString filePath =
      QFileDialog::getOpenFileName(this, "Выберите файл", "/", "Файлы (*.obj)");
  if (!filePath.isEmpty()) {
    free_data_gg(&ui->openGLWidget->data_1);
    ui->name_file->setText(filePath);
    std::string filename = filePath.toStdString();
    ui->openGLWidget->filename = (char *)(filename.c_str());
    ui->openGLWidget->open_obj();
    double count_edge = ui->openGLWidget->data_1.edge_count;
    double count_vertices = ui->openGLWidget->data_1.vertex_count;
    QString count_edge_str = QString::number(count_edge, 'g', 15);
    QString count_vertices_str = QString::number(count_vertices, 'g', 15);
    ui->label_count_edge->setText(count_edge_str);
    ui->label_count_vertices->setText(count_vertices_str);
    QFileInfo fileInfo(filePath);
    ui->label_name_file->setText(fileInfo.baseName());
    ui->move_X->setValue(ui->openGLWidget->move_x);
    ui->move_Y->setValue(ui->openGLWidget->move_y);
    ui->move_Z->setValue(ui->openGLWidget->move_z);
    ui->rotate_X->setValue(ui->openGLWidget->rotate_x);
    ui->rotate_Y->setValue(ui->openGLWidget->rotate_y);
    ui->rotate_Z->setValue(ui->openGLWidget->rotate_z);
    ui->scale_value->setValue(ui->openGLWidget->scale_val);
    ui->size_of_vertices->setValue(ui->openGLWidget->size_of_vertices);
    ui->thickness_of_edge->setValue(ui->openGLWidget->thickness_of_edges);
  }
}

void MainWindow::on_rotate_X_valueChanged(double value) {
  rotate_X(&ui->openGLWidget->data_1, (value - ui->openGLWidget->rotate_x));
  ui->openGLWidget->rotate_x = value;
  ui->openGLWidget->update();
}

void MainWindow::on_rotate_Y_valueChanged(double value) {
  rotate_Y(&ui->openGLWidget->data_1, (value - ui->openGLWidget->rotate_y));
  ui->openGLWidget->rotate_y = value;
  ui->openGLWidget->update();
}

void MainWindow::on_rotate_Z_valueChanged(double value) {
  rotate_Z(&ui->openGLWidget->data_1, (value - ui->openGLWidget->rotate_z));
  ui->openGLWidget->rotate_z = value;
  ui->openGLWidget->update();
}

void MainWindow::on_move_X_valueChanged(double value) {
  move_X(&ui->openGLWidget->data_1, (value - ui->openGLWidget->move_x) *
                                        ui->openGLWidget->normalize / 100);
  ui->openGLWidget->move_x = value;
  ui->openGLWidget->update();
}

void MainWindow::on_move_Y_valueChanged(double value) {
  move_Y(&ui->openGLWidget->data_1, (value - ui->openGLWidget->move_y) *
                                        ui->openGLWidget->normalize / 100);
  ui->openGLWidget->move_y = value;
  ui->openGLWidget->update();
}

void MainWindow::on_move_Z_valueChanged(double value) {
  move_Z(&ui->openGLWidget->data_1, (value - ui->openGLWidget->move_z) *
                                        ui->openGLWidget->normalize / 100);
  ui->openGLWidget->move_z = value;
  ui->openGLWidget->update();
}

void MainWindow::on_color_edge_clicked() {
  QColor ColorValue = QColorDialog::getColor(ui->openGLWidget->ColorEdge, this,
                                             tr("Select Color"));
  if (ColorValue.isValid()) {
    ui->openGLWidget->ColorEdge = ColorValue;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_color_background_clicked() {
  QColor ColorValue = QColorDialog::getColor(ui->openGLWidget->ColorBackground,
                                             this, tr("Select Color"));
  if (ColorValue.isValid()) {
    ui->openGLWidget->ColorBackground = ColorValue;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_color_vertices_clicked() {
  QColor ColorValue = QColorDialog::getColor(ui->openGLWidget->ColorVertices,
                                             this, tr("Select Color"));
  if (ColorValue.isValid()) {
    ui->openGLWidget->ColorVertices = ColorValue;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_scale_value_valueChanged(double value) {
  double val = (double)value / ui->openGLWidget->scale_val;
  scale(&ui->openGLWidget->data_1, val);
  ui->openGLWidget->scale_val = value;
  ui->openGLWidget->update();
}

void MainWindow::on_display_method_no_clicked() {
  ui->openGLWidget->display_method = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_display_method_circle_clicked() {
  ui->openGLWidget->display_method = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_display_method_square_clicked() {
  ui->openGLWidget->display_method = 2;
  ui->openGLWidget->update();
}

void MainWindow::on_type_solid_clicked() {
  ui->openGLWidget->type_edge = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_type_dashed_clicked() {
  ui->openGLWidget->type_edge = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_thickness_of_edge_valueChanged(double value) {
  ui->openGLWidget->thickness_of_edges = value;
  ui->openGLWidget->update();
}

void MainWindow::on_size_of_vertices_valueChanged(double value) {
  ui->openGLWidget->size_of_vertices = value;
  ui->openGLWidget->update();
}

void MainWindow::on_type_parallel_clicked() {
  ui->openGLWidget->type_of_projection = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_type_central_clicked() {
  ui->openGLWidget->type_of_projection = 1;
  ui->openGLWidget->update();
}

void MainWindow::saveSettings() {
  settings->setValue("TypeProjection", ui->openGLWidget->type_of_projection);
  settings->setValue("TypeEdge", ui->openGLWidget->type_edge);
  settings->setValue("ThicknessOfEdge", ui->openGLWidget->thickness_of_edges);
  settings->setValue("ColorEdge",
                     ui->openGLWidget->ColorEdge.name(QColor::HexRgb));
  settings->setValue("DisplayMethod", ui->openGLWidget->display_method);
  settings->setValue("SizeVertices", ui->openGLWidget->size_of_vertices);
  settings->setValue("ColorVertices",
                     ui->openGLWidget->ColorVertices.name(QColor::HexRgb));
  settings->setValue("ColorBackground",
                     ui->openGLWidget->ColorBackground.name(QColor::HexRgb));
}

void MainWindow::loadSettings() {
  ui->openGLWidget->type_of_projection =
      settings->value("TypeProjection", 0).toInt();
  ui->openGLWidget->type_edge = settings->value("TypeEdge", 0).toInt();
  ui->openGLWidget->thickness_of_edges =
      settings->value("ThicknessOfEdge", 0).toDouble();
  ui->openGLWidget->ColorEdge = settings->value("ColorEdge").toString();
  ui->openGLWidget->display_method =
      settings->value("DisplayMethod", 0).toInt();
  ui->openGLWidget->size_of_vertices =
      settings->value("SizeVertices", 0).toDouble();
  ui->openGLWidget->ColorVertices = settings->value("ColorVertices").toString();
  ui->openGLWidget->ColorBackground =
      settings->value("ColorBackground").toString();
  setRadioButtonChecked(ui->type_parallel,
                        ui->openGLWidget->type_of_projection == 0);
  setRadioButtonChecked(ui->type_central,
                        ui->openGLWidget->type_of_projection == 1);
  setRadioButtonChecked(ui->type_solid, ui->openGLWidget->type_edge == 0);
  setRadioButtonChecked(ui->type_dashed, ui->openGLWidget->type_edge == 1);
  setRadioButtonChecked(ui->display_method_no,
                        ui->openGLWidget->display_method == 0);
  setRadioButtonChecked(ui->display_method_circle,
                        ui->openGLWidget->display_method == 1);
  setRadioButtonChecked(ui->display_method_square,
                        ui->openGLWidget->display_method == 2);
  ui->size_of_vertices->setValue(ui->openGLWidget->size_of_vertices);
  ui->thickness_of_edge->setValue(ui->openGLWidget->thickness_of_edges);
}

void MainWindow::setRadioButtonChecked(QRadioButton *radioButton,
                                       bool checked) {
  if (radioButton) {
    radioButton->setChecked(checked);
  }
}

void MainWindow::on_pushButton_screen_clicked() {
  QString filePath = QFileDialog::getSaveFileName(
      this, tr("Save File"), "/", tr("Text Files (*.jpeg);;Файлы (*.bmp)"));
  QFile file(filePath);
  if (file.open(QIODevice::WriteOnly)) {
    QImage img = ui->openGLWidget->grabFramebuffer();
    if (img.save(filePath)) {
      QMessageBox::information(this, tr("Success"),
                               tr("Image saved successfully."));
    } else {
      QMessageBox::warning(this, tr("Error"), tr("Failed to save the image."));
    }
    file.close();
  } else {
    QMessageBox::warning(this, tr("Error"), tr("Could not save the file."));
  }
}

void MainWindow::on_pushButton_GIF_clicked() {
  filePat = QFileDialog::getSaveFileName(this, tr("Save File"), "/",
                                         tr("Файлы (*.gif)"));

  if (filePat.isEmpty()) {
    QMessageBox::warning(this, tr("Error"), tr("Could not save the file."));
  } else {
    ui->pushButton_GIF->setEnabled(false);
    gif_frame = new QGifImage;
    gif_frame->setDefaultDelay(10);
    timer->setInterval(100);
    timecount = 0;
    timer->start();
  }
}
void MainWindow::startTimergif() {
  if (gif_frame && timer) {
    QImage img = ui->openGLWidget->grabFramebuffer();
    QSize img_size(640, 480);
    QImage scaled_img = img.scaled(img_size);
    gif_frame->addFrame(scaled_img);
    const int MaxFrames = 50;
    if (timecount == MaxFrames) {
      timer->stop();
      gif_frame->save(filePat);
      QMessageBox::information(0, "/", "Gif animation saved successfully");
      delete gif_frame;
      gif_frame = nullptr;
      ui->pushButton_GIF->setEnabled(true);
    }
    timecount++;
  }
}
