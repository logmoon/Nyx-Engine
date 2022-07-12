# include "global.h"

Global global = {0};

void global_set_level_dimentions(u32 level_width, u32 level_height)
{
	global.level_width = level_width;
	global.level_height = level_height;
}
