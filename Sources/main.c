#include <kinc/global.h>
#include <kinc/system.h>

int kickstart(int argc, char** argv) {
	
	
	kinc_set_update_callback(ui_update);
	
	kinc_start();
	
	return 0;
}
