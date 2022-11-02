# pragma once

# include <stdbool.h>
# include "../types.h"

/**
 * Sets an fps cap
 *
 * @param fps -1 for uncapped
 */
void core_set_fps_cap(f32 fps);
f64 core_get_absolute_time();

void core_exit_application();

void core_init(char* company_name,
		char* application_name,
		u32 native_width, 
		u32 native_height);

void core_update();

void core_shutdown(void);
