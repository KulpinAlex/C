#include "3dviewer.h"

/*!
    \defgroup 3dviewer_affin Affine transformations
    \ingroup affin
    \brief This section will describe basic affine transformations in 3D-object

     It currently supports the following 3D object transformation functions:
   check_input, translation_data, scale_data, rotation_data_ox,
   rotation_data_oy, rotation_data_oz.
*/
///@{
/*! \brief The function of the input data validation
     \param data structure with input values
     \param dx,dy,dz Parameters intended for changing the structure
      \returns Value 1 if data passed validation, 0 otherwise
*/
int check_input(Points* data, double dx, double dy, double dz) {
  int result = 1;
  if (data->count_point == 0) result = 0;
  if (data->count_poly == 0) result = 0;
  if (data->error != SUCCESS) result = 0;
  if (data->array_points == NULL) result = 0;
  if (data->array_poly == NULL) result = 0;
  if (data == NULL) result = 0;
  if (isnan(dx)) result = 0;
  if (isnan(dy)) result = 0;
  if (isnan(dz)) result = 0;
  if (isinf(dx)) result = 0;
  if (isinf(dy)) result = 0;
  if (isinf(dz)) result = 0;
  return result;
}

/*! \brief The function of translation of a 3D object along the axes Ox, Oy, Oz
     \param data structure with input values
     \param dx Coordinate shift parameter along the Ox axis
     \param dy Coordinate shift parameter along the Oy axis
     \param dz Coordinate shift parameter along the Oz axis
      \returns Value 1 if the object's coordinates were successfully shifted, 0
   otherwise
*/
int translation_data(Points* data, double dx, double dy, double dz) {
  int result = 1;
  if (check_input(data, dx, dy, dz)) {
    for (int i = 0; i < data->count_point * 3; i++) {
      if (i % 3 == 0) *(data->array_points + i) += dx;
      if (i % 3 == 1) *(data->array_points + i) += dy;
      if (i % 3 == 2) *(data->array_points + i) += dz;
    }
  } else {
    result = 0;
    data->error = ANOTHER_ERROR;
  }
  return result;
}

/*! \brief The function of scaling of a 3D object
     \param data structure with input values
     \param scale scale parameter
      \returns Value 1 if the object's coordinates were successfully scaled, 0
   otherwise
*/
int scale_data(Points* data, double scale) {
  int result = 1;
  if (check_input(data, scale, scale, scale)) {
    for (int i = 0; i < data->count_point * 3; i++) {
      *(data->array_points + i) *= scale;
    }
  } else {
    result = 0;
    data->error = ANOTHER_ERROR;
  }
  return result;
}

/*! \brief The function of rotation of a 3D object along the Ox axis
     \param data structure with input values
     \param angle angle of rotation
      \returns Value 1 if the object's coordinates were successfully
   recalculated, 0 otherwise
*/
int rotation_data_ox(Points* data, double angle) {
  int result = 1;
  if (check_input(data, angle, angle, angle)) {
    for (int i = 0; i < data->count_point * 3; i += 3) {
      double temp_y = *(data->array_points + i + 1);
      double temp_z = *(data->array_points + i + 2);
      *(data->array_points + i + 1) = temp_y * cos(angle) + temp_z * sin(angle);
      *(data->array_points + i + 2) =
          -1 * temp_y * sin(angle) + temp_z * cos(angle);
    }
  } else {
    result = 0;
    data->error = ANOTHER_ERROR;
  }
  return result;
}

/*! \brief The function of rotation of a 3D object along the Oy axis
     \param data structure with input values
     \param angle angle of rotation
      \returns Value 1 if the object's coordinates were successfully
   recalculated, 0 otherwise
*/
int rotation_data_oy(Points* data, double angle) {
  int result = 1;
  if (check_input(data, angle, angle, angle)) {
    for (int i = 0; i < data->count_point * 3; i += 3) {
      double temp_x = *(data->array_points + i);
      double temp_z = *(data->array_points + i + 2);
      *(data->array_points + i) = temp_x * cos(angle) - temp_z * sin(angle);
      *(data->array_points + i + 2) = temp_x * sin(angle) + temp_z * cos(angle);
    }
  } else {
    result = 0;
    data->error = ANOTHER_ERROR;
  }
  return result;
}

/*! \brief The function of rotation of a 3D object along the Oy axis
     \param data structure with input values
     \param angle angle of rotation
      \returns Value 1 if the object's coordinates were successfully
   recalculated, 0 otherwise
*/
int rotation_data_oz(Points* data, double angle) {
  int result = 1;
  if (check_input(data, angle, angle, angle)) {
    for (int i = 0; i < data->count_point * 3; i += 3) {
      double temp_x = *(data->array_points + i);
      double temp_y = *(data->array_points + i + 1);
      *(data->array_points + i) = temp_x * cos(angle) + temp_y * sin(angle);
      *(data->array_points + i + 1) =
          -1 * temp_x * sin(angle) + temp_y * cos(angle);
    }
  } else {
    result = 0;
    data->error = ANOTHER_ERROR;
  }
  return result;
}
///@}