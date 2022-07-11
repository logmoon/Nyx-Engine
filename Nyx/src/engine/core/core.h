# pragma once

# include <stdbool.h>
# include "../types.h"

void core_init(char* company_name,
		char* application_name,
		u32 native_width, 
		u32 native_height,
		u32 window_width,
		u32 window_height,
		bool fullscreen);
void core_update();
void core_shutdown(void);
