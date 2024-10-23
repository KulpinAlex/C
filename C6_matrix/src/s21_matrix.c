#include "s21_matrix.h"

// A[i][j] <=> *(*(A + i) + j)
int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error = 1;
  if (result) {
    result->columns = 0;
    result->rows = 0;
    result->matrix = NULL;

    if (rows > 0 && columns > 0) {
      double **matrix = (double **)malloc(rows * sizeof(double *) +
                                          rows * columns * sizeof(double));
      if (matrix) {
        matrix[0] = (double *)(matrix + rows);
        for (int i = 1; i < rows; i++) {
          matrix[i] = matrix[0] + i * columns;
        }
        for (int i = 0; i < rows; i++) {
          for (int j = 0; j < columns; j++) {
            *(*(matrix + i) + j) = 0;
          }
        }
        result->columns = columns;
        result->rows = rows;
        result->matrix = matrix;
        error = 0;
      }
    }
  }
  return error;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix) free(A->matrix);
  A->matrix = NULL;
  A->columns = 0;
  A->rows = 0;
}

int s21_check_matrix(matrix_t *A) {
  return (A->rows <= 0 || A->columns <= 0 || A->matrix == NULL);
}

int s21_check_eq_matrix(matrix_t *A, matrix_t *B) {
  int error = 1;
  if (!s21_check_matrix(A) && !s21_check_matrix(B)) {
    error = (A->rows == B->rows && A->columns == B->columns) ? 0 : 2;
  }

  return error;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int error = s21_check_eq_matrix(A, B);
  if (!error) {
    int rows = A->rows;
    int columns = A->columns;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        double check = *(*(A->matrix + i) + j) - *(*(B->matrix + i) + j);
        check = check > 0 ? check : -1.0 * check;
        if (check >= 1e-7) error = 2;
      }
    }
  }
  return ((error == 0) ? SUCCESS : FAILURE);
}

int s21_sumsub_matrix(matrix_t *A, matrix_t *B, matrix_t *result,
                      double point) {
  int error = s21_check_eq_matrix(A, B);
  if (!error) {
    int rows = A->rows;
    int columns = A->columns;
    error = s21_create_matrix(rows, columns, result);
    if (!error) {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          *(*(result->matrix + i) + j) =
              *(*(A->matrix + i) + j) + *(*(B->matrix + i) + j) * point;
        }
      }
    }
  }
  return error;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_sumsub_matrix(A, B, result, 1.0);
}
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_sumsub_matrix(A, B, result, -1.0);
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int error = s21_check_matrix(A);
  if (!error) {
    int rows = A->rows;
    int columns = A->columns;
    error = s21_create_matrix(rows, columns, result);
    if (!error) {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          *(*(result->matrix + i) + j) = *(*(A->matrix + i) + j) * number;
        }
      }
    }
  }
  return error;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = s21_check_mult_matrix(A, B);
  if (!error) {
    int rows = A->rows;
    int columns = B->columns;
    int k = A->columns;
    error = s21_create_matrix(rows, columns, result);
    if (!error) {
      // A(i,k) × B(k,j).
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          double sum = 0;
          for (int t = 0; t < k; t++) {
            sum += *(*(A->matrix + i) + t) * (*(*(B->matrix + t) + j));
          }

          *(*(result->matrix + i) + j) = sum;
        }
      }
    }
  }
  return error;
}

int s21_check_mult_matrix(matrix_t *A, matrix_t *B) {
  int error = 1;
  if (!s21_check_matrix(A) && !s21_check_matrix(B)) {
    error = (A->columns == B->rows) ? 0 : 2;
  }
  return error;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int error = s21_check_matrix(A);
  if (!error) {
    int rows = A->columns;
    int columns = A->rows;
    error = s21_create_matrix(rows, columns, result);
    if (!error) {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          *(*(result->matrix + i) + j) = *(*(A->matrix + j) + i);
        }
      }
    }
  }
  return error;
}

int s21_determinant(matrix_t *A, double *result) {
  int error = s21_check_matrix(A);
  if (!error) {
    error = (A->columns == A->rows) ? error : 2;
    if (!error) {
      int order = A->rows;
      *result = s21_det(A, order);
    }
  }
  return error;
}

// det 3x3 = a00⋅a11⋅a22 + a01⋅a12⋅a20 + a10⋅a21⋅a02 − a02⋅a11⋅a20 −
// a01⋅a10⋅a22− a12⋅a21⋅a00 det 2x2 = a00⋅a11 - a01⋅a10
double s21_det(matrix_t *A, int order) {
  double result = 0;
  if (order == 3) {
    result = A->matrix[0][0] * A->matrix[1][1] * A->matrix[2][2];
    result += A->matrix[0][1] * A->matrix[1][2] * A->matrix[2][0];
    result += A->matrix[1][0] * A->matrix[2][1] * A->matrix[0][2];
    result -= A->matrix[0][2] * A->matrix[1][1] * A->matrix[2][0];
    result -= A->matrix[0][1] * A->matrix[1][0] * A->matrix[2][2];
    result -= A->matrix[1][2] * A->matrix[2][1] * A->matrix[0][0];
  } else if (order == 2) {
    result = A->matrix[0][0] * A->matrix[1][1];
    result -= A->matrix[0][1] * A->matrix[1][0];
  } else if (order == 1) {
    result = A->matrix[0][0];
  } else {
    matrix_t temp = {0};
    if (!s21_create_matrix(order - 1, order - 1, &temp)) {
      for (int i = 0; i < order; i++) {
        s21_minor(A, 0, i, &temp);
        int sign = ((i % 2) == 0) ? 1 : -1;
        result += sign * A->matrix[0][i] * s21_det(&temp, order - 1);
      }
      s21_remove_matrix(&temp);
    }
  }

  return result;
}

void s21_minor(matrix_t *A, int row, int col, matrix_t *result) {
  int point1 = 0;
  result->columns = A->columns - 1;
  result->rows = A->rows - 1;
  for (int i = 0; i < A->rows; i++) {
    if (i == row) {
      point1 = -1;
      continue;
    }
    int point2 = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == col) {
        point2 = -1;
        continue;
      }
      *(*(result->matrix + i + point1) + j + point2) = *(*(A->matrix + i) + j);
    }
  }
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int error = s21_check_matrix(A);
  if (!error) {
    error = (A->columns == A->rows) ? error : 2;
    if (!error) {
      int row = A->rows;
      matrix_t temp = {0};
      if (!s21_create_matrix(row - 1, row - 1, &temp)) {
        error = s21_create_matrix(row, row, result);
        if (!error) {
          for (int i = 0; i < row; i++) {
            for (int j = 0; j < row; j++) {
              s21_minor(A, i, j, &temp);
              int sign = (((i + j) % 2) == 0) ? 1 : -1;
              *(*(result->matrix + i) + j) = sign * s21_det(&temp, row - 1);
            }
          }
        }
        s21_remove_matrix(&temp);
      }
    }
  }
  return error;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int error = s21_check_matrix(A);
  if (!error) {
    error = (A->columns == A->rows) ? error : 2;
    if (!error) {
      if (A->rows == 1) {
        double check =
            (A->matrix[0][0] > 0) ? A->matrix[0][0] : -1.0 * A->matrix[0][0];
        if (check < 1e-7) {
          error = 2;
        } else {
          error = s21_create_matrix(1, 1, result);
          result->matrix[0][0] = 1.0 / A->matrix[0][0];
        }
      } else {
        double det = 0;
        error = s21_determinant(A, &det);
        double check_det = (det > 0) ? det : -1.0 * det;
        if (check_det < 1e-7) error = 2;
        if (!error) {
          det = 1.0 / det;
          matrix_t temp1 = {0};
          error = s21_calc_complements(A, &temp1);
          if (!error) {
            matrix_t temp2 = {0};
            error = s21_transpose(&temp1, &temp2);
            if (!error) {
              error = s21_mult_number(&temp2, det, result);
              s21_remove_matrix(&temp2);
            }
            s21_remove_matrix(&temp1);
          }
        }
      }
    }
  }
  return error;
}