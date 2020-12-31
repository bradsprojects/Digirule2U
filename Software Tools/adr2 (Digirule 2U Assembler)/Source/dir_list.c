#include "adr2.h"

void dir_list( operation_info_t *poi )
{
	label_prohibited();
	flags.dir.nolist = 0;
}

void dir_nolist( operation_info_t *poi )
{
	label_prohibited();
	flags.dir.nolist = 1;
}
