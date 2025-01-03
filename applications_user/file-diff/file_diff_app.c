#include "views/select_view.h"

#include <furi.h>
#include <furi_hal.h>

#include <gui/gui.h>
#include <gui/view_dispatcher.h>

#include <gui/modules/widget.h>
#include <gui/modules/submenu.h>

typedef struct {
    ViewDispatcher* view_dispatcher;
    SelectView* select_view;
} FileDiffApp;

int32_t file_diff_app(void* p) {
    UNUSED(p);

    FileDiffApp* app = (FileDiffApp*)malloc(sizeof(FileDiffApp));
    Gui* gui = furi_record_open(RECORD_GUI);
    app->view_dispatcher = view_dispatcher_alloc();
    app->select_view = select_view_alloc(NULL, NULL);

    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_add_view(app->view_dispatcher, 1, select_view_get_view(app->select_view));

    view_dispatcher_switch_to_view(app->view_dispatcher, 1);
    view_dispatcher_run(app->view_dispatcher);

    view_dispatcher_remove_view(app->view_dispatcher, 1);
    view_dispatcher_free(app->view_dispatcher);
    select_view_free(app->select_view);
    furi_record_close(RECORD_GUI);
    free(app);

    return 0;
}
