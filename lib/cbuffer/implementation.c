//					implementation dependencies
#include "interface.h"
#include <string.h>

result_t cbuffer_movePointerToStrEnd(char **buf, size_t *buf_length){
	size_t l = strlen(*buf);
	if(*buf_length <= l+1){
		return RESULT_FAILURE;
	 }
	*buf_length = *buf_length - l;
	*buf = &((*buf)[l]);
	return RESULT_SUCCESS;
}
