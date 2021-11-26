#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void ui_init(void);

void ui_update(void);

void ui_window_begin(char* label,int x,int y, int w, int h);
void ui_window_end(void);

void ui_deinit(void);

#ifdef __cplusplus
}
#endif
