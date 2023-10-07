#ifndef S21_3D_VIEWER_PARSER_UTILITY_H
#define S21_3D_VIEWER_PARSER_UTILITY_H

#include <stdio.h>

#include "parser.h"

#define VECTOR_INITIAL_SIZE 8
#define VECTOR_RESIZE_FACTOR 1.5

enum Errors {
  NO_ERROR,
  MEMORY_ALLOCATION_ERROR,
  FILE_OPEN_ERROR,
  FILE_ENDED_ERROR,
  INVALID_OBJ_FILE_ERROR,
  VECTOR_INITIALIZATION_ERROR,
};

typedef struct {
  char *start;
  unsigned int size;
  unsigned int offset;
} vector_char;

typedef struct {
  enum parse_type {
    NOTHING,
    VERTEX,
    INDEX,
  } type;
  union parse_value {
    vertex vert;
    edge ind;
  } value;
  unsigned int vertex_count;
} parse_result;

int read_line(FILE *file, vector_char *line);
int parse_line(char *line, parse_result *res);
int parse_lines(FILE *file, data_t *data);
int add_parse_result(data_t *data, parse_result *res);

int parse_vertex(char *line, parse_result *res);
int parse_index(char *line, parse_result *res);

// vector_char
int vector_char_create(vector_char *v);
int vector_char_push_back(vector_char *v, char c);
void vector_char_remove(vector_char *v);

// vector_vertex
int vector_vertex_create(data_t *v);
int vector_vertex_push_back(data_t *v, vertex vert);
void vector_vertex_remove(data_t *v);

// vector_indexes
int vector_index_create(data_t *v);
int vector_index_push_back(data_t *v, edge ind);
void vector_index_remove(data_t *v);

#endif
