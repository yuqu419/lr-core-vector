/* vector.c —— 你要实现的地方 */

#include "vector.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
static inline int check_not_null(const vector *v) {
    return v && v->data && v->cap && v->end;
}
int vector_init(vector *v, size_t capacity) {
    if (!v)
        return -1;
    if (capacity >= SIZE_MAX / sizeof(int)) {
        *v = (vector){.cap = NULL, .data = NULL, .end = NULL};
        return -1;
    }
    if (capacity == 0) {
        *v = (vector){.cap = NULL, .data = NULL, .end = NULL};
        return 0;
    }
    size_t memory_size = capacity * sizeof(int);
    int *head = (int *)malloc(memory_size);
    if (!head)
        return -1;
    *v = (vector){.data = head, .end = head, .cap = head + capacity};
    return 0;
}

void vector_destroy(vector *v) {
    if (!v)
        return;
    free(v->data);
    *v = (vector){.cap = NULL, .data = NULL, .end = NULL};
}

size_t size(const vector *v) {
    if (!check_not_null(v))
        return 0;
    return v->end - v->data;
}

size_t capacity(const vector *v) {
    if (!check_not_null(v))
        return 0;
    return v->cap - v->data;
}

int empty(const vector *v) {
    return !size(v);
}

int get(const vector *v, size_t index, int *out) {
    if (!check_not_null(v)) {
        return -1;
    }
    if (index >= size(v))
        return -1;
    *out = *(v->data + index);
    return 0;
}

int set(vector *v, size_t index, int value) {
    if (!check_not_null(v)) {
        return -1;
    }
    if (index >= size(v))
        return -1;
    *(v->data + index) = value;
    return 0;
}

int front(const vector *v, int *out) {
    if (!check_not_null(v) || empty(v))
        return -1;
    *out = *(v->data);
    return 0;
}

int back(const vector *v, int *out) {
    if (!check_not_null(v) || empty(v))
        return -1;
    *out = *(v->data + size(v) - 1);
    return 0;
}
int push_back(vector *v, int value) {
    if (!check_not_null(v)) {
        int res = vector_init(v, 1);
        if (res == -1)
            return -1;
    }
    size_t cur_size = size(v);
    if (cur_size >= capacity(v)) {
    if(cur_size >= SIZE_MAX / sizeof(int) / 2){
        return -1;
    }
        size_t new_cap = cur_size == 0 ? 1 : cur_size * 2;
        int res = reserve(v, new_cap);
        if (res == -1)
            return -1;
    }
    *(v->end) = value;
    v->end += 1;
    return 0;
}

int pop_back(vector *v, int *out) {
    if (!check_not_null(v)) {
        return -1;
    }
    if (empty(v))
        return -1;
    *out = *(v->end - 1);
    v->end -= 1;
    return 0;
}

int reserve(vector *v, size_t capacity) {
    if (!check_not_null(v)) {
        int res = vector_init(v, capacity);
        if (res == -1)
            return -1;
        return 0;
    }
    if (capacity >= SIZE_MAX / sizeof(int))
        return -1;
    size_t cur_size = size(v);
    if (capacity <= v->cap - v->data)
        return 0;
    size_t new_cap = sizeof(int) * capacity;
    int *new_ptr = (int *)realloc(v->data, new_cap);
    if (!new_ptr)
        return -1;
    *v = (vector){
        .data = new_ptr, .end = new_ptr + cur_size, .cap = new_ptr + capacity};
    return 0;
}

int shrink_to_fit(vector *v) {
    if (!check_not_null(v)) {
        int res = vector_init(v, 0);
        if (res == -1)
            return -1;
        return 0;
    }
    size_t cur_size = size(v);
    if (cur_size == 0) {
        vector_destroy(v);
        return 0;
    }
    size_t new_cap = sizeof(int) * cur_size;
    int *new_ptr = (int *)realloc(v->data, new_cap);
    if (!new_ptr)
        return -1;
    *v = (vector){
        .data = new_ptr, .end = new_ptr + cur_size, .cap = new_ptr + cur_size};
    return 0;
}

void clear(vector *v) {
    if (!check_not_null(v))
        return;
    v->end = v->data;
}
