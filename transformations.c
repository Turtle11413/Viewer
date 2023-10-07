#include "transformations.h"

void move_X(data_gg *data, double move_x) {
  for (unsigned int i = 0; i < data->vertex_count * 3; i += 3) {
    data->vertex[i] += move_x;
  }
}

void move_Y(data_gg *data, double move_y) {
  for (unsigned int i = 1; i < data->vertex_count * 3; i += 3) {
    data->vertex[i] += move_y;
  }
}

void move_Z(data_gg *data, double move_z) {
  for (unsigned int i = 0; i < data->vertex_count * 3; i += 3) {
    data->vertex[i + 2] += move_z;
  }
}

void rotate_X(data_gg *data, double angle_x) {
  double sin_x = sin(convertAngleToRadians(angle_x));
  double cos_x = cos(convertAngleToRadians(angle_x));
  for (unsigned int i = 0; i < data->vertex_count * 3; i += 3) {
    double y = data->vertex[i + 1];
    double z = data->vertex[i + 2];
    data->vertex[i + 1] = cos_x * y - sin_x * z;
    data->vertex[i + 2] = cos_x * z + sin_x * y;
  }
}

void rotate_Y(data_gg *data, double angle_y) {
  double sin_y = sin(convertAngleToRadians(angle_y));
  double cos_y = cos(convertAngleToRadians(angle_y));
  for (unsigned int i = 0; i < data->vertex_count * 3; i += 3) {
    double x = data->vertex[i];
    double z = data->vertex[i + 2];
    data->vertex[i] = cos_y * x + sin_y * z;
    data->vertex[i + 2] = cos_y * z - sin_y * x;
  }
}

void rotate_Z(data_gg *data, double angle_z) {
  double sin_z = sin(convertAngleToRadians(angle_z));
  double cos_z = cos(convertAngleToRadians(angle_z));
  for (unsigned int i = 0; i < data->vertex_count * 3; i += 3) {
    double x = data->vertex[i];
    double y = data->vertex[i + 1];
    data->vertex[i] = cos_z * x - sin_z * y;
    data->vertex[i + 1] = cos_z * y + sin_z * x;
  }
}

double convertAngleToRadians(double angle) { return PI * angle / 180; }

void scale(data_gg *data, double scale) {
  for (unsigned int i = 0; i < data->vertex_count * 3; i += 3) {
    data->vertex[i] *= scale;
    data->vertex[i + 1] *= scale;
    data->vertex[i + 2] *= scale;
  }
}
