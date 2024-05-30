/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static size_t request_char_count_for_iserver = 128;
static size_t response_char_count_for_iserver = 4096;
static int port = 55555;

static result_t ifunc_begin(serveFunc_t serve_func){
	return tcpServer_begin(serve_func, request_char_count_for_iserver, response_char_count_for_iserver, port);
}

static void ifunc_greeting(){
	printf("+------------------------------------------------------+\n");
	printf("| You are using TcpServer                              |\n");
	printf("|   on port: %-42d|\n", port);
	printf("| Restrictions:                                        |\n");
	printf("|   max number of input characters: %-19ld|\n", request_char_count_for_iserver);
	printf("|   max number of output characters: %-18ld|\n", response_char_count_for_iserver);
	printf("| Usage example:                                       |\n");
	printf("|   printf \"app print\\n\" | nc 127.0.0.1 %-15d|\n", port);
	printf("+------------------------------------------------------+\n");
}

static void ifunc_control(){
	tcpServer_control();
}

static void ifunc_free(){
	tcpServer_free();
}

void tcpServer_buildServerInterface(iServer *ifc) {
	iServer_setParam(ifc, ifunc_begin, ifunc_greeting, ifunc_control, ifunc_free);
}
