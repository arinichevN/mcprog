
#define COMMAND_IS(CMD) strncmp(cmd_str, CMD, PROTOCOL_COMMAND_SIZE_MAX) == 0
serveResult_t serveCommandACDP(void *ifc, ServerPackage *request, ServerPackage *response){
	iCommandACDP *icmd = ifc;
	char *buf = serverPackage_getCharBuffer(request);
	size_t buf_length = serverPackage_getCharBufferLength(request);
	char request_buffer[buf_length];
	size_t rbs = sizeof(char) * buf_length;
	memset(request_buffer, 0, rbs);
	memcpy(request_buffer, buf, rbs); 
	char *cmd_str = getCommand(request_buffer, buf_length);
	if(COMMAND_IS("add")){
		char *name_str = getParamStr(request_buffer, buf_length, 0);
		iCommandACDP_add(icmd, name_str);
		return SERVE_RESULT_SILENT;
	} else if(COMMAND_IS("clear")){
		iCommandACDP_clear(icmd);
		return SERVE_RESULT_SILENT;
	} else if(COMMAND_IS("delete")){
		char *name_str = getParamStr(request_buffer, buf_length, 0);
		iCommandACDP_delete(icmd, name_str);
		return SERVE_RESULT_SILENT;
	} else if(COMMAND_IS("print")){
		char *buf = serverPackage_getCharBuffer(response);
		size_t buf_size = serverPackage_getCharBufferSize(response);
		iCommandACDP_print(icmd, buf, buf_size);
		return SERVE_RESULT_RESPOND;
	}
	char *b = serverPackage_getCharBuffer(response);
	size_t bs = serverPackage_getCharBufferSize(response);
	snprintf(b, bs, "unknown command!\n\t expected: add | clear | delete | print\n");
	return SERVE_RESULT_RESPOND;
}

serveResult_t serveCommandEP(void *ifc, ServerPackage *request, ServerPackage *response){
	iCommandEP *icmd = ifc;
	char *buf = serverPackage_getCharBuffer(request);
	size_t buf_length = serverPackage_getCharBufferLength(request);
	char request_buffer[buf_length];
	size_t rbs = sizeof(char) * buf_length;
	memset(request_buffer, 0, rbs);
	memcpy(request_buffer, buf, rbs); 
	char *cmd_str = getCommand(request_buffer, buf_length);
	if(COMMAND_IS("exit")){
		iCommandEP_exit(icmd);
		return SERVE_RESULT_SILENT;
	} else if(COMMAND_IS("print")){
		char *buf = serverPackage_getCharBuffer(response);
		size_t buf_size = serverPackage_getCharBufferSize(response);
		iCommandEP_print(icmd, buf, buf_size);
		return SERVE_RESULT_RESPOND;
	}
	char *b = serverPackage_getCharBuffer(response);
	size_t bs = serverPackage_getCharBufferSize(response);
	snprintf(b, bs, "unknown command!\n\t expected: exit | print\n");
	return SERVE_RESULT_RESPOND;
}

serveResult_t serveCommandPStSp(void *ifc, ServerPackage *request, ServerPackage *response){
	iCommandPStSp *icmd = ifc;
	char *buf = serverPackage_getCharBuffer(request);
	size_t buf_length = serverPackage_getCharBufferLength(request);
	char request_buffer[buf_length];
	size_t rbs = sizeof(char) * buf_length;
	memset(request_buffer, 0, rbs);
	memcpy(request_buffer, buf, rbs); 
	char *cmd_str = getCommand(request_buffer, buf_length);
	if(COMMAND_IS("print")){
		char *buf = serverPackage_getCharBuffer(response);
		size_t buf_size = serverPackage_getCharBufferSize(response);
		iCommandPStSp_print(icmd, buf, buf_size);
		return SERVE_RESULT_RESPOND;
	} else if(COMMAND_IS("start")){
		iCommandPStSp_start(icmd);
		return SERVE_RESULT_SILENT;
	} else if(COMMAND_IS("stop")){
		iCommandPStSp_stop(icmd);
		return SERVE_RESULT_SILENT;
	}
	char *b = serverPackage_getCharBuffer(response);
	size_t bs = serverPackage_getCharBufferSize(response);
	snprintf(b, bs, "unknown command!\n\t expected: print | start | stop\n");
	return SERVE_RESULT_RESPOND;
}

serveResult_t serveCommand(void *ifc, ServerPackage *request, ServerPackage *response){
	iCommand *icmd = ifc;
	switch(icmd->kind){
		case ICOMMAND_KIND_ACDP:
			return serveCommandACDP(icmd->ifc, request, response);
		case ICOMMAND_KIND_EP:
			return serveCommandEP(icmd->ifc, request, response);
		case ICOMMAND_KIND_PSTSP:
			return serveCommandPStSp(icmd->ifc, request, response);
		default:
			break;
	}
	char *buf = serverPackage_getCharBuffer(response);
	size_t buf_size = serverPackage_getCharBufferSize(response);
	snprintf(buf, buf_size, "no interface\n");
	return SERVE_RESULT_RESPOND;
}

#undef COMMAND_IS
