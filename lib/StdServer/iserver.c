/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static size_t request_char_count_for_iserver = 128;
static size_t response_char_count_for_iserver = 4096;

static result_t ifunc_begin(serveFunc_t serve_func){
	return stdServer_begin(serve_func, request_char_count_for_iserver, response_char_count_for_iserver);
}

static void ifunc_greeting(){
	printf("+------------------------------------------------------+\n");
	printf("| You are using StdServer                              |\n");
	printf("| Restrictions:                                        |\n");
	printf("|   max number of input characters: %-19ld|\n", request_char_count_for_iserver);
	printf("|   max number of output characters: %-18ld|\n", response_char_count_for_iserver);
	printf("+------------------------------------------------------+\n");
}

static void ifunc_control(){
	stdServer_control();
}

static void ifunc_free(){
	stdServer_free();
}

void stdServer_buildServerInterface(iServer *ifc) {
	iServer_setParam(ifc, ifunc_begin, ifunc_greeting, ifunc_control, ifunc_free);
}
