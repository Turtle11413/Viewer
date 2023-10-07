#ifndef S21_3D_VIEWER_PARSER_H
#define S21_3D_VIEWER_PARSER_H

#define MAX_INDEXES 5

typedef struct {
  double *vertex;
  int *index;
  unsigned int vertex_count;
  unsigned int index_count;
  unsigned int edge_count;
} data_gg;

typedef struct {
  double x;
  double y;
  double z;
} vertex;

typedef struct {
  int indexes[MAX_INDEXES];
  int count;
} edge;

typedef struct {
  vertex *vertexes;
  unsigned int vertex_count;
  unsigned int vertex_offset;
  edge *indexes;
  unsigned int index_count;
  unsigned int index_offset;
} data_t;

// Парсит объект в "data_gg" который понимается OpenGL-ом
int parse_obj(const char *filename, data_gg *data);

// конвертирует адекватное представление "data_t" в "data_gg", который
// понимается OpenGL-ом
int convert_data_t_to_data_gg(data_t *source, data_gg *target);

// парсит файл в "data_t"
int parse_file(const char *filename, data_t *data);

// очищает "data_gg"
void free_data_gg(data_gg *data);

#endif
