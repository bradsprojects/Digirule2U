#include "adr2.h"

void inst_1byte( operation_info_t *poi )
{
	label_optional();
	object->emit8( poi->operation );
}
