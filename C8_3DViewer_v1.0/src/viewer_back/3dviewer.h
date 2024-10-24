/*!
\file
\brief Header file with descriptions of functions

This file contains the definitions of the main functions used in the application
*/
#ifndef SRC_VIEWER_BACK_3DVIEWER_H
#define SRC_VIEWER_BACK_3DVIEWER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// parser
#define buffer 1024  // max length of line

enum error {
  SUCCESS,
  FILE_NOT_OPEN,
  POINTS_MORE_BILLION,
  ALLOCATION_ERROR,
  ANOTHER_ERROR,
  OBJ_MORE_ONE
};

typedef struct {
  unsigned int count_point;
  unsigned int count_poly;
  double *array_points;
  unsigned int *array_poly;
  unsigned int error;
} Points;

void parser(char *fpath, Points *model);
void initPoint(Points *a);
void freePoint(Points *a);
void check_points_poly(char *fpath, Points *model);

// affine
int translation_data(Points *data, double dx, double dy, double dz);
int scale_data(Points *data, double scale);
int rotation_data_ox(Points *data, double angle);
int rotation_data_oy(Points *data, double angle);
int rotation_data_oz(Points *data, double angle);

int translation_data(Points *data, double dx, double dy, double dz);

#endif
