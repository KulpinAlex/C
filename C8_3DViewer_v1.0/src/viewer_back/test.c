#include "3dviewer.h"

int main() {
  Points example;
  initPoint(&example);
  parser(test_file_obj, &example);
  if (!example.error) {
    for (int i = 0; i < example.count_point * 3; i++) {
      printf("%f\n", example.array_points[i]);
    }
    for (int i = 0; i < example.count_poly * 2; i++) {
      printf("%d\n", example.array_poly[i]);
    }
  }
  freePoint(&example);
  return 0;
}