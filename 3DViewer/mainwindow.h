#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <glwidget.h>
#include <qgifimage.h>

#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QOpenGLFramebufferObject>
#include <QRadioButton>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_open_file_clicked();
  void on_rotate_X_valueChanged(double value);
  void on_rotate_Y_valueChanged(double value);
  void on_rotate_Z_valueChanged(double value);
  void on_move_X_valueChanged(double value);
  void on_move_Y_valueChanged(double value);
  void on_move_Z_valueChanged(double value);
  void on_color_edge_clicked();
  void on_color_background_clicked();
  void on_color_vertices_clicked();
  void on_scale_value_valueChanged(double value);
  void on_display_method_no_clicked();
  void on_display_method_circle_clicked();
  void on_display_method_square_clicked();
  void on_type_solid_clicked();
  void on_type_dashed_clicked();
  void on_thickness_of_edge_valueChanged(double value);
  void on_size_of_vertices_valueChanged(double value);
  void on_type_parallel_clicked();
  void on_type_central_clicked();
  void on_pushButton_screen_clicked();
  void saveSettings();
  void loadSettings();
  void on_pushButton_GIF_clicked();
  void startTimergif();
  void setRadioButtonChecked(QRadioButton *radioButton, bool checked);

 private:
  Ui::MainWindow *ui;
  QSettings *settings;
  QGifImage *gif_frame;
  QTimer *timer;
  QString filePat;
  int timecount = 0;
};

#endif  // MAINWINDOW_H
