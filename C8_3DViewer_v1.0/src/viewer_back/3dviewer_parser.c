#include "3dviewer.h"

/*!
    \defgroup 3dviewer_parser Parsing input data
    \ingroup parser
    \brief This section will describe the basic functions of parsing an input
   file into a data structure.

     It currently supports the following file parsing functions: parser,
   initPoint, freePoint, check_points_poly.
*/
///@{
/*! \brief The function of the input file parsing
     \param fpath path to file
     \param model structure with input values
*/
void parser(char *fpath, Points *model) {
  check_points_poly(fpath, model);
  if (!model->error) {
    model->array_points = (double *)realloc(
        model->array_points, sizeof(double) * model->count_point * 3);
    model->array_poly = (unsigned int *)realloc(
        model->array_poly, sizeof(unsigned int) * model->count_poly * 2);
    if (!model->array_points || !model->array_poly) {
      model->error = ALLOCATION_ERROR;
    } else {
      FILE *file = fopen(fpath, "r");
      if (file) {
        char str[buffer];
        int tmp_count_point = 0;
        int tmp_count_poly = 0;

        while (fgets(str, buffer, file)) {
          if (!strncmp(str, "v ", 2)) {
            char *tmp_str;
            tmp_str = str + 2;
            for (int i = 0; i < 3; i++) {
              model->array_points[tmp_count_point * 3 + i] =
                  strtod(tmp_str, &tmp_str);
            }
            tmp_count_point++;
          }
          if (!strncmp(str, "f ", 2)) {
            int i = 0;
            char *tmp_str;
            tmp_str = strtok(str + 2, " \n");
            while (tmp_str) {
              unsigned int point = atoi(tmp_str);
              (point) ? model->array_poly[tmp_count_poly + i] = point - 1 : i--;
              if (!(i % 2) || !i) {
                tmp_str = strtok(NULL, " \n");
              }
              i++;
            }
            model->array_poly[tmp_count_poly + i] =
                model->array_poly[tmp_count_poly];
            tmp_count_poly = tmp_count_poly + i + 1;
          }
        }
      } else {
        model->error = FILE_NOT_OPEN;
      }
      fclose(file);
    }
  }
}

/*! \brief The function of the init structure
     \param a pointer to structure to initialize
*/
void initPoint(Points *a) {
  a->count_point = 0;
  a->count_poly = 0;
  a->array_points = (double *)malloc(sizeof(double));
  a->array_poly = (unsigned int *)malloc(sizeof(unsigned int));
  if (!a->array_points || !a->array_poly) {
    a->error = ALLOCATION_ERROR;
  } else {
    a->error = SUCCESS;
  }
}

/*! \brief Structure memory cleanup function
     \param a pointer to structure to clean memory
*/
void freePoint(Points *a) {
  free(a->array_points);
  free(a->array_poly);
}

/*! \brief File pre-processing function
     \param fpath path to file
     \param model structure with input values
*/
void check_points_poly(char *fpath, Points *model) {
  FILE *file = fopen(fpath, "r");
  if (file) {
    char str[buffer];
    int i = 0;
    int count_obj = 0;
    while (fgets(str, buffer, file) && model->error != POINTS_MORE_BILLION) {
      if (!strncmp(str, "v ", 2)) {
        (!count_obj) ? model->count_point++ : (model->error = OBJ_MORE_ONE);
      }
      if (!strncmp(str, "f ", 2)) {
        char *tmp_str;
        tmp_str = str + 2;

        char *tmp_strtok = strtok(tmp_str, " \n");
        int tmp = atoi(tmp_strtok);
        while (tmp) {
          model->count_poly++;
          tmp_str = NULL;
          tmp_strtok = strtok(tmp_str, " \n");
          tmp = (tmp_strtok) ? atoi(tmp_strtok) : 0;
        }

        count_obj = 1;
      }
      if (model->count_point > 1000000) model->error = POINTS_MORE_BILLION;
    }
    fclose(file);
  } else {
    model->error = FILE_NOT_OPEN;
  }
}
///@}
