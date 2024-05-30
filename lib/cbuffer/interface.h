#ifndef CHAR_BUFFER_H
#define CHAR_BUFFER_H

//					interface dependencies
#include <stddef.h>
#include "../result_t/interface.h"

extern result_t cbuffer_movePointerToStrEnd(char **buf, size_t *buf_length);

#endif
