/* vector.c —— 你要实现的地方 */

#include "vector.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
/*
检查vector和成员变量是否为NULL，若存在返回-1,否则返回0
*/
int static inline check(const vector *v) {
    if (!v || !(v->cap) || !(v->data) || !(v->end)) {
        return -1;
    }
    return 0;
}
/*
 将vector的成员变量全部置空，成功返回0,失败返回-1
 */
int static inline set_null(vector *v) {
    if (!v)
        return -1;
    *v = (vector){.data = NULL, .end = NULL, .cap = NULL};
    return 0;
}

int vector_init(vector *v, size_t capacity) {
    if (capacity == 0) {
        set_null(v);
        return 0;
    }
    if (capacity >= (SIZE_MAX / sizeof(int))) {
        set_null(v);
        return -1;
    }
    // 实际内存为capacity + 1,为了存储cap
    int *head = malloc(capacity * sizeof(int));
    if (!head) {
        set_null(v);
        return -1;
    }
    *v = (vector){.data = head, .end = head, .cap = head + capacity};
    return 0;
}

void vector_destroy(vector *v) {
    if (!v)
        return;
    free(v->data);
    set_null(v);
}

size_t size(const vector *v) {
    if (check(v) == -1)
        return 0;
    return v->end - v->data;
}

size_t capacity(const vector *v) {
    if (check(v) == -1)
        return 0;
    return v->cap - v->data;
}

int empty(const vector *v) {
    return !size(v);
}

int get(const vector *v, size_t index, int *out) {
    if (check(v) == -1 || !out || index >= size(v))
        return -1;
    *out = (v->data)[index];

    return 0;
}

int set(vector *v, size_t index, int value) {
    if (check(v) == -1 || index >= size(v))
        return -1;
    (v->data)[index] = value;

    return 0;
}

int front(const vector *v, int *out) {
    if (check(v) == -1 || !out || empty(v))
        return -1;
    *out = *(v->data);
    return 0;
}

int back(const vector *v, int *out) {
    if (check(v) == -1 || !out || empty(v))
        return -1;
    *out = *(v->end - 1);
    return 0;
}

int push_back(vector *v, int value) {
    if (check(v) == -1) {
        vector_init(v, 1);
        return push_back(v, value);
    }
    size_t o_size = size(v);
    // 扩容
    if (v->end == v->cap) {
        int *new_p =
            realloc(v->data, (o_size == 0 ? 1 : o_size * 2) * sizeof(int));
        if (!new_p)
            return -1;
        *v = (vector){.data = new_p,
                      .end = new_p + o_size,
                      .cap = new_p + (o_size == 0 ? 1 : o_size * 2)};
    }
    *(v->end) = value;
    v->end++;
    return 0;
}

int pop_back(vector *v, int *out) {
    if (check(v) == -1 || !out || empty(v))
        return -1;
    *out = *(v->end - 1);
    v->end--;
    return 0;
}

int reserve(vector *v, size_t capacity) {
    if (capacity <= size(v))
        return 0;
    if (capacity == 0) {
        vector_destroy(v);
        return 0;
    }
    if (check(v) == -1) {
        return vector_init(v, capacity);
    }
    if (capacity >= SIZE_MAX / sizeof(int))
        return -1;
    size_t o_size = size(v);
    int *new_p = realloc(v->data, capacity * sizeof(int));
    if (!new_p)
        return -1;
    *v =
        (vector){.data = new_p, .end = new_p + o_size, .cap = new_p + capacity};
    return 0;
}

int shrink_to_fit(vector *v) {
    if (check(v) == -1)
        return 0;
    size_t o_size = size(v);
    if (o_size == 0) {
        vector_destroy(v);
        return 0;
    }
    int *new_p = realloc(v->data, o_size * sizeof(int));
    if (!new_p)
        return -1;
    *v = (vector){.data = new_p, .end = new_p + o_size, .cap = new_p + o_size};
    return 0;
}

void clear(vector *v) {
    if (check(v) == -1)
        return;
    v->end = v->data;
}