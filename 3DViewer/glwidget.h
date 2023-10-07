#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

#include <QMessageBox>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>

extern "C" {
#include "../parser.h"
#include "../parser_utility.h"
#include "../transformations.h"
#include "../transformations_utility.h"
}

class glwidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  glwidget(QWidget *parent = nullptr);
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void open_obj();
  void draw_edge();
  void draw_vertices();
  void set_normalize();
  void reset_transformation();
  data_gg data_1 = {NULL, NULL, 0, 0, 0};
  double rotate_x;
  double rotate_y;
  double rotate_z;
  double move_x;
  double move_y;
  double move_z;
  double scale_val = 50;
  double thickness_of_edges = 1;
  double size_of_vertices = 1;
  int type_of_projection = 0;
  int type_edge = 0;
  int display_method = 0;
  int error = 0;
  char *filename;
  GLfloat normalize;
  QColor ColorEdge = Qt::black;
  QColor ColorBackground = Qt::white;
  QColor ColorVertices = Qt::red;
};

#endif  // GLWIDGET_H
