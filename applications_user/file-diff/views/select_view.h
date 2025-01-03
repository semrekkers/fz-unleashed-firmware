#pragma once

#include <gui/view.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SelectViewFileA,
    SelectViewFileB,
} SelectViewFileType;

typedef struct SelectView SelectView;

typedef void (*SelectViewCallback)(SelectViewFileType file, void* context);

SelectView* select_view_alloc(SelectViewCallback callback, void* context);

void select_view_free(SelectView* select_view);

View* select_view_get_view(SelectView* select_view);

void select_view_set_file(SelectView* select_view, SelectViewFileType file, char* name);

void select_view_set_status_text(SelectView* select_view, const char* text);

#ifdef __cplusplus
}
#endif
