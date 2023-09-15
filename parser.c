#include "parser.h"
#include "parser_utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_obj(const char *filename, data_gg *data, int connect_vertex_count) {
  data_t obj_data;
  int rtn = 0;

  rtn = parse_file(filename, &obj_data);
  if (!rtn) {
    rtn = convert_data_t_to_data_gg(&obj_data, data, connect_vertex_count);
  }

  vector_vertex_remove(&obj_data);
  vector_index_remove(&obj_data);

  return rtn;
}

int convert_data_t_to_data_gg(data_t *source, data_gg *target,
                              int connect_vertex_count) {
  int rtn = 0;

  target->vertex = (double *)malloc(sizeof(double) * source->vertex_offset * 3);
  if (!target->vertex) {
    rtn = MEMORY_ALLOCATION_ERROR;
  }
  target->index =
      (int *)malloc(sizeof(int) * source->index_offset * connect_vertex_count);
  if (!target->index) {
    rtn = MEMORY_ALLOCATION_ERROR;
    free(target->vertex);
  }

  for (int i = 0; i < source->vertex_offset; ++i) {
    target->vertex[i * 3] = source->vertexes[i].x;
    target->vertex[i * 3 + 1] = source->vertexes[i].y;
    target->vertex[i * 3 + 2] = source->vertexes[i].z;
  }
  target->vertex_count = source->vertex_offset;

  for (int i = 0; i < source->index_offset; ++i) {
    for (int j = 0; j < connect_vertex_count; ++j) {
      target->index[i * connect_vertex_count + j] =
          source->indexes[i].indexes[j];
    }
  }
  target->index_count = source->index_offset;

  return rtn;
}

void free_data_gg(data_gg *data) {
  free(data->index);
  free(data->vertex);
  data->index = NULL;
  data->vertex = NULL;
  data->index_count = 0;
  data->vertex_count = 0;
}

int parse_file(const char *filename, data_t *data) {
  int rtn = 0;
  FILE *file = NULL;

  file = fopen(filename, "r");
  if (!file) {
    rtn = FILE_OPEN_ERROR;
  }

  if (!rtn) {
    parse_lines(file, data);
  }

  if (rtn != FILE_OPEN_ERROR) {
    fclose(file);
  }

  return rtn;
}

int parse_lines(FILE *file, data_t *data) {
  int rtn = 0;
  vector_char line;
  int vector_failed = 0;

  rtn = vector_vertex_create(data);
  vector_failed = rtn;
  if (!vector_failed) {
    rtn = vector_index_create(data);
  } else {
    vector_vertex_remove(data);
  }

  while (!rtn && !(rtn = read_line(file, &line))) {
    parse_result res = {NOTHING, {0}, data->vertex_offset};
    if (!rtn) {
      rtn = parse_line(line.start, &res);
    }
    if (!rtn) {
      rtn = add_parse_result(data, res);
    }
    if (rtn != VECTOR_INITIALIZATION_ERROR) {
      vector_char_remove(&line);
    }
  }

  return rtn;
}

int add_parse_result(data_t *data, parse_result res) {
  int rtn = 0;

  if (res.type == VERTEX) {
    rtn = vector_vertex_push_back(data, res.value.vert);
  } else if (res.type == INDEX) {
    rtn = vector_index_push_back(data, res.value.ind);
  }

  return rtn;
}

int parse_line(char *line, parse_result *res) {
  int rtn = 0;

  res->type = NOTHING;
  if (strncmp(line, "v ", 2) == 0) {
    rtn = parse_vertex(line + 2, res);
  } else if (strncmp(line, "f ", 2) == 0) {
    rtn = parse_index(line + 2, res);
  }

  return rtn;
}

int parse_vertex(char *line, parse_result *res) {
  int rtn = 0;
  int count = 0;
  vertex v = {0};

  count = sscanf(line, "%lf%lf%lf", &v.x, &v.y, &v.z);
  if (count == 3) {
    res->type = VERTEX;
    res->value.vert = v;
  } else {
    rtn = INVALID_OBJ_FILE_ERROR;
  }

  return rtn;
}

int parse_index(char *line, parse_result *res) {
  int rtn = 0;
  int count = 0;
  int comment = 0;

  line = strtok(line, " ");
  while (line && !comment && count < MAX_INDEXES && !rtn) {
    long value = strtol(line, NULL, 10);
    if ((value < 0 && (-value > res->vertex_count)) ||
        (value > res->vertex_count)) {
      rtn = INVALID_OBJ_FILE_ERROR;
    } else if (value != 0) {
      if (value < 0) {
        value = res->vertex_count + value + 1;
      }
      res->value.ind.indexes[count] = value;
      ++count;
    } else {
      if (*line == '#') {
        comment = 1;
      } else {
        rtn = INVALID_OBJ_FILE_ERROR;
      }
    }
    line = strtok(NULL, " ");
  }

  if (!rtn) {
    res->type = INDEX;
  }

  return rtn;
}

int read_line(FILE *file, vector_char *line) {
  int rtn = 0;
  char c;

  rtn = vector_char_create(line);
  if (!rtn) {
    c = getc(file);
  }
  while (c != EOF && !rtn && c != '\n') {
    rtn = vector_char_push_back(line, c);
    c = getc(file);
  }
  if (!rtn) {
    rtn = vector_char_push_back(line, 0);
  }
  if (!rtn && c == EOF) {
    rtn = FILE_ENDED_ERROR;
  }
  if (rtn && rtn < VECTOR_INITIALIZATION_ERROR) {
    vector_char_remove(line);
  }

  return rtn;
}

// -------------------------  VECTOR_CHAR  ------------------------

int vector_char_create(vector_char *v) {
  int rtn = 0;
  *v = (vector_char){NULL, 0};

  v->start = (char *)malloc(sizeof(char) * VECTOR_INITIAL_SIZE);
  if (!v->start) {
    rtn = VECTOR_INITIALIZATION_ERROR;
  } else {
    v->size = VECTOR_INITIAL_SIZE;
    v->offset = 0;
  }

  return rtn;
}

int vector_char_push_back(vector_char *v, char c) {
  int rtn = 0;

  if (v->size == v->offset) {
    char *temp =
        (char *)realloc(v->start, sizeof(char) * (v->size + v->size / 2));
    if (!temp) {
      rtn = MEMORY_ALLOCATION_ERROR;
    } else {
      v->start = temp;
      v->size += v->size / 2;
    }
  }
  if (!rtn) {
    v->start[v->offset] = c;
    ++(v->offset);
  }

  return rtn;
}

void vector_char_remove(vector_char *v) {
  free(v->start);
  v->start = NULL;
  v->offset = 0;
  v->size = 0;
}

// -------------------------  VECTOR_VERTEX  ------------------------

int vector_vertex_create(data_t *v) {
  int rtn = 0;
  v->vertexes = NULL;
  v->vertex_count = 0;
  v->vertex_offset = 0;

  v->vertexes = (vertex *)malloc(sizeof(vertex) * VECTOR_INITIAL_SIZE);
  if (!v->vertexes) {
    rtn = VECTOR_INITIALIZATION_ERROR;
  } else {
    v->vertex_count = VECTOR_INITIAL_SIZE;
    v->vertex_offset = 0;
  }

  return rtn;
}

int vector_vertex_push_back(data_t *v, vertex vert) {
  int rtn = 0;

  if (v->vertex_count == v->vertex_offset) {
    vertex *temp = (vertex *)realloc(
        v->vertexes, sizeof(vertex) * (v->vertex_count + v->vertex_count / 2));
    if (!temp) {
      rtn = MEMORY_ALLOCATION_ERROR;
    } else {
      v->vertexes = temp;
      v->vertex_count += v->vertex_count / 2;
    }
  }
  if (!rtn) {
    v->vertexes[v->vertex_offset] = vert;
    ++(v->vertex_offset);
  }

  return rtn;
}

void vector_vertex_remove(data_t *v) {
  free(v->vertexes);
  v->vertexes = NULL;
  v->vertex_offset = 0;
  v->vertex_count = 0;
}

// -------------------------  VECTOR_VERTEX  ------------------------

int vector_index_create(data_t *v) {
  int rtn = 0;
  v->indexes = NULL;
  v->index_count = 0;
  v->index_offset = 0;

  v->indexes = (edge *)malloc(sizeof(edge) * VECTOR_INITIAL_SIZE);
  if (!v->indexes) {
    rtn = VECTOR_INITIALIZATION_ERROR;
  } else {
    v->index_count = VECTOR_INITIAL_SIZE;
    v->index_offset = 0;
  }

  return rtn;
}

int vector_index_push_back(data_t *v, edge ind) {
  int rtn = 0;

  if (v->index_count == v->index_offset) {
    edge *temp = (edge *)realloc(
        v->indexes, sizeof(edge) * (v->index_count + v->index_count / 2));
    if (!temp) {
      rtn = MEMORY_ALLOCATION_ERROR;
    } else {
      v->indexes = temp;
      v->index_count += v->index_count / 2;
    }
  }
  if (!rtn) {
    for (int i = 0; i < MAX_INDEXES; ++i) {
      v->indexes[v->index_offset].indexes[i] = ind.indexes[i];
    }
    ++(v->index_offset);
  }

  return rtn;
}

void vector_index_remove(data_t *v) {
  free(v->indexes);
  v->indexes = NULL;
  v->index_offset = 0;
  v->index_count = 0;
}
