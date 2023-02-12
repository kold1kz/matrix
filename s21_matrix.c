#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int res = UNCORRECT_MATRIX;
  result->rows = rows;
  result->columns = columns;
  result->matrix = calloc(rows, sizeof(double *));
  if (result->matrix != NULL && rows > 0 && columns > 0) {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = calloc(columns, sizeof(double *));
    }
    res = OK;
  }
  return res;
}

// void show_matrix(matrix_t *A) {
//   for (int i = 0; i < A->rows; i++) {       // строка
//     for (int j = 0; j < A->columns; j++) {  // столбец
//       printf("%.1f ", A->matrix[i][j]);
//     }
//     printf("\n");
//   }
//   printf("\n");
// }

void s21_remove_matrix(matrix_t *A) {
  if (A->columns > 0 && A->rows > 0 && A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = FAILURE;
  if ((A->rows == B->rows) && (B->columns == A->columns) &&
      (A->rows > 0 && B->rows > 0 && B->columns > 0 && A->columns > 0) &&
      (A->matrix != NULL && B->matrix != NULL)) {
    res = SUCCESS;
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if ((fabs(A->matrix[i][j] - B->matrix[i][j])) > EPS) {
          res = FAILURE;
        }
      }
    }
  }
  return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = UNCORRECT_MATRIX;
  if (s21_create_matrix(A->rows, A->columns, result)) {
    return UNCORRECT_MATRIX;
  }
  if ((A->rows > 0 && B->rows > 0 && B->columns > 0 && A->columns > 0) &&
      (A->matrix != NULL && B->matrix != NULL)) {
    if ((A->rows != B->rows) || (B->columns != A->columns)) {
      res = ERROR_CALC;
    } else {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
      }
      res = OK;
    }
  }
  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = UNCORRECT_MATRIX;
  if (s21_create_matrix(A->rows, A->columns, result)) {
    return UNCORRECT_MATRIX;
  }
  if ((A->rows > 0 && B->rows > 0 && B->columns > 0 && A->columns > 0) &&
      (A->matrix != NULL && B->matrix != NULL)) {
    if ((A->rows != B->rows) || (B->columns != A->columns)) {
      res = ERROR_CALC;
    } else {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
      }
      res = OK;
    }
  }
  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = UNCORRECT_MATRIX;

  if (s21_create_matrix(A->rows, A->columns, result)) {
    return UNCORRECT_MATRIX;
  }

  if ((A->rows > 0 && A->columns > 0) && (A->matrix != NULL)) {
    res = OK;
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = UNCORRECT_MATRIX;
  if (s21_create_matrix(A->rows, B->columns, result)) {
    return UNCORRECT_MATRIX;
  }
  if (A->columns != B->rows) {
    res = ERROR_CALC;
  } else {
    if ((A->rows > 0 && A->columns > 0) && (B->rows > 0 && B->columns > 0) &&
        (A->matrix != NULL) && (B->matrix != NULL)) {
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++) {
          result->matrix[i][j] = 0;
          for (int q = 0; q < A->columns; q++) {
            result->matrix[i][j] += (A->matrix[i][q] * B->matrix[q][j]);
          }
        }
      }
      res = OK;
    }
  }
  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = UNCORRECT_MATRIX;
  if (A->columns > 0 && A->rows > 0 && A->matrix != NULL) {
    if (s21_create_matrix(A->columns, A->rows, result)) {
      return UNCORRECT_MATRIX;
    }
    for (int i = 0; i < A->columns; i++) {
      for (int j = 0; j < A->rows; j++) {
        result->matrix[i][j] = A->matrix[j][i];
      }
    }
    res = OK;
  }
  return res;
}

double matrix2x2(matrix_t *A) {
  double det = 0;
  if (A->rows != A->columns)
    det = FAILURE;
  else if (A->rows == 2)
    det = A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  return det;
}

void simple_minor(matrix_t *A, matrix_t *res, int rows, int columns, int size) {
  int out_rows = 0;
  for (int i = 0; i < size - 1; i++) {
    if (i == rows) out_rows = 1;

    int out_columns = 0;
    for (int j = 0; j < size - 1; j++) {
      if (j == columns) out_columns = 1;
      res->matrix[i][j] = A->matrix[i + out_rows][j + out_columns];
    }
  }
}

double simple_det(matrix_t *A) {
  double det = 0;
  int sign = 1;
  if (A->rows == 2) {
    det = matrix2x2(A);
  } else {
    matrix_t temp;
    s21_create_matrix(A->rows - 1, A->rows - 1, &temp);
    for (int i = 0; i < A->rows; i++) {
      simple_minor(A, &temp, 0, i, A->rows);
      det = det + (sign * A->matrix[0][i] * simple_det(&temp));
      sign *= -1;
    }
    s21_remove_matrix(&temp);
  }
  return det;
}

int s21_determinant(matrix_t *A, double *result) {
  int res = OK;
  if (!A || !result || !A->matrix)
    res = UNCORRECT_MATRIX;
  else if (A->columns != A->rows)
    res = ERROR_CALC;
  else if (A->rows == 1)
    *result = A->matrix[0][0];
  else
    *result = simple_det(A);
  return res;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int res = OK;
  if (!A || !A->matrix || !result || (A->rows <= 0) || (A->columns <= 0)) {
    res = UNCORRECT_MATRIX;
  } else if (A->columns != A->rows) {
    res = ERROR_CALC;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->rows; j++) {
        matrix_t temp;
        double det = 0;
        s21_create_matrix(A->columns - 1, A->columns - 1, &temp);
        simple_minor(A, &temp, i, j, A->rows);
        s21_determinant(&temp, &det);
        result->matrix[i][j] = det * pow(-1, i + j);
        s21_remove_matrix(&temp);
      }
    }
  }
  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int res = OK;
  double det = 0;
  if (A == NULL || result == NULL || A->rows <= 0 || A->columns <= 0) {
    res = UNCORRECT_MATRIX;
  } else if (s21_determinant(A, &det) != 0 || fabs(det) < EPS) {
    res = ERROR_CALC;
  } else if (A->rows != A->columns) {
    res = ERROR_CALC;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    if (A->rows == 1) {
      result->matrix[0][0] = 1.0 / det;
    } else {
      matrix_t algdop;
      s21_calc_complements(A, &algdop);
      matrix_t transp;
      s21_transpose(&algdop, &transp);
      for (int i = 0; i < transp.rows; i++) {
        for (int j = 0; j < transp.columns; j++) {
          result->matrix[i][j] = transp.matrix[i][j] / det;
        }
      }
      s21_remove_matrix(&algdop);
      s21_remove_matrix(&transp);
    }
  }
  return res;
}
