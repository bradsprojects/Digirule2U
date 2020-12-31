#include "adr2.h"

void dir_page( operation_info_t *poi )
{
	label_prohibited();
	list_page_eject();
}

void dir_nopage( operation_info_t *poi )
{
	label_prohibited();
	flags.dir.nopage = 1;
}
