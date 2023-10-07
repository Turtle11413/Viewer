#ifndef S21_3D_VIEWER_TRANSFORMATIONS_H
#define S21_3D_VIEWER_TRANSFORMATIONS_H

#include "parser.h"
#include "transformations_utility.h"

void move_X(data_gg *data, double move_x);
void move_Y(data_gg *data, double move_y);
void move_Z(data_gg *data, double move_z);

void rotate_X(data_gg *data, double angle_x);
void rotate_Y(data_gg *data, double angle_y);
void rotate_Z(data_gg *data, double angle_z);

void scale(data_gg *data, double scale);

#endif
