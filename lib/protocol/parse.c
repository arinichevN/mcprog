
static char *getCommandSubstring(char *buf, size_t *buf_length){
	size_t i = 0;
	while(i+1 < *buf_length){
		char c = buf[i];
		i++;
		if(c == DATA_DELIMITER){
			size_t new_length = *buf_length - i;
			*buf_length = new_length;
			return &buf[i];
		}
		
	}
#ifdef MODE_TEST
	fprintf(stderr, "getCommandSubstring(): command not found\n");
#endif
	return NULL;
}

static char *getParamSubstring(char *buf, size_t *buf_length, int param_index){
	size_t i = 0;
	//skip path
	while(i+1 < *buf_length){
		char c = buf[i];
		i++;
		if(c == DATA_DELIMITER){
			break;
		}
	}
	//searching parameter
	int ind = 0;
	while(i+1 < *buf_length){
		char c = buf[i];
		i++;
		if(c == DATA_DELIMITER){
			if(ind == param_index){
				size_t new_length = *buf_length - i;
				*buf_length = new_length;
				return &buf[i];
			}
			ind++;
		}
	}
#ifdef MODE_TEST
	fprintf(stderr, "getParamSubstring(): param not found\n");
#endif
	return NULL;
}

static char *writeStrToBuffer(char *src, size_t src_length){
	memset(sbuffer, 0, SBUFFER_SIZE);
	size_t i = 0, bi = 0;
	while(bi < SBUFFER_LENGTH_MAX - 1){
		if(i >= src_length){
#ifdef MODE_TEST
			fprintf(stderr, "writeStrToBuffer(): string not found\n");
#endif
			return "\0";
		}
		char c = src[i];
		if(c == DATA_DELIMITER || c == PACKAGE_END_DELIMITER){
			return sbuffer;
		}
		sbuffer[bi] = c;
		bi++;
		i++;
	}
#ifdef MODE_TEST
	fprintf(stderr, "writeStrToBuffer(): sbuffer overflow\n");
#endif
	return sbuffer;
}

static char *getCommand(char *buf, size_t buf_length){
	char *cmd = getCommandSubstring(buf, &buf_length);
	if(cmd == NULL){
		return "\0";
	}
	return writeStrToBuffer(cmd, buf_length);
}

static char *getParamStr(char *buf, size_t buf_length, int param_index){
	char *param = getParamSubstring(buf, &buf_length, param_index);
	if(param == NULL){
		return NULL;
	}
	return writeStrToBuffer(param, buf_length);
}

static Node *getNodeByPath(Node *node, char *buf, size_t buf_length){
	char name[NAME_LENGTH_MAX];
	memset(name, 0, sizeof(char) * NAME_LENGTH_MAX);
	size_t n = 0;
	size_t i = 0;
	while(i+1 < buf_length){
		if(n >= NAME_LENGTH_MAX){
		#ifdef MODE_TEST
			fprintf(stderr, "getNodeByPath(): name buffer overflow\n");
		#endif
			return NULL;
		}
		char c = buf[i];
		i++;
		if(c == NAME_DELIMITER){
			Node *next_node = node_findByName(node, name);
			size_t new_buf_length = buf_length - i;
			return getNodeByPath(next_node, &buf[i], new_buf_length);
		} else if(c == DATA_DELIMITER || c == PACKAGE_END_DELIMITER){
			return node_findByName(node, name);
		}
		name[n] = c;
		n++;
	}
#ifdef MODE_TEST
	fprintf(stderr, "getNodeByPath(): failed to parse name\n");
#endif
	return NULL;
}
