# pragma once

# include <stdbool.h>
# include "../types.h"

void core_init(char* company_name, char* app_name, u32 window_width, u32 window_height, bool fullscreen);
void core_update();
void core_shutdown(void);
