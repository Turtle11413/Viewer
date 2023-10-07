#include "glwidget.h"

glwidget::glwidget(QWidget *parent) : QOpenGLWidget(parent) {
  move_x = 0;
  move_y = 0;
  move_z = 0;
  rotate_x = 0;
  rotate_y = 0;
  rotate_z = 0;
  filename = nullptr;
}

void glwidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

void glwidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void glwidget::paintGL() {
  glClearColor(ColorBackground.redF(), ColorBackground.greenF(),
               ColorBackground.blueF(), 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glVertexPointer(3, GL_DOUBLE, 0, data_1.vertex);
  double aspect = (double)width() / (double)height();
  if (type_of_projection == 0) {
    glFrustum(-1 * aspect * normalize, 1 * normalize * aspect, -1 * normalize,
              1 * normalize, normalize, 1000 * normalize);
    glTranslatef(0, 0, -2 * normalize);
  }
  if (type_of_projection == 1) {
    glOrtho(-1 * normalize * aspect, 1 * normalize * aspect, -1 * normalize,
            1 * normalize, -1000 * normalize, 1000 * normalize);
  }
  glRotatef(30, 1, 0, 0);
  glEnableClientState(GL_VERTEX_ARRAY);
  draw_edge();
  if (display_method) draw_vertices();
}

void glwidget::open_obj() {
  char *str = filename;
  error = parse_obj(str, &data_1);
  if (!error) {
    set_normalize();
    reset_transformation();
    update();
  } else {
    QMessageBox::critical(this, "Error", "Error");
  }
}

void glwidget::draw_edge() {
  if (type_edge) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0b1111000011110000);
  }
  glLineWidth(thickness_of_edges);
  glColor3f(ColorEdge.redF(), ColorEdge.greenF(), ColorEdge.blueF());
  glDrawElements(GL_LINES, data_1.index_count, GL_UNSIGNED_INT, data_1.index);
  glDisable(GL_LINE_STIPPLE);
  if (type_edge) {
    glDisable(GL_LINE_STIPPLE);
  }
}

void glwidget::draw_vertices() {
  if (display_method == 1) {
    glEnable(GL_POINT_SMOOTH);
  }
  glPointSize(size_of_vertices);
  glColor3f(ColorVertices.redF(), ColorVertices.greenF(),
            ColorVertices.blueF());
  glDrawArrays(GL_POINTS, 0, data_1.vertex_count);
  if (display_method == 1) {
    glDisable(GL_POINT_SMOOTH);
  }
}

void glwidget::set_normalize() {
  normalize = 0;
  const size_t vertexCount = data_1.vertex_count * 3;
  for (size_t i = 0; i < vertexCount; i++) {
    const double vertexValue = fabs(data_1.vertex[i]);
    if (vertexValue > normalize) {
      normalize = vertexValue;
    }
  }
}

void glwidget::reset_transformation() {
  move_x = 0;
  move_y = 0;
  move_z = 0;
  rotate_x = 0;
  rotate_y = 0;
  rotate_z = 0;
  scale_val = 50;
  size_of_vertices = 1;
  thickness_of_edges = 1;
}
