#include "select_view.h"

struct SelectView {
    View* view;
    SelectViewCallback callback;
    void* callback_context;
};

typedef struct {
    SelectViewFileType selected_file;
    char* file_a;
    char* file_b;
    const char* status_text;
} SelectViewModel;

static void
    canvas_draw_textbox(Canvas* canvas, int y, bool selected, const char* text1, const char* text2) {
    canvas_draw_str(canvas, 0, y + 9, text1);
    if(selected) {
        canvas_draw_rbox(canvas, 27, y, 95, 12, 2);
        canvas_invert_color(canvas);
        canvas_draw_str(canvas, 29, y + 9, text2);
        canvas_invert_color(canvas);
    } else {
        canvas_draw_rframe(canvas, 27, y, 95, 12, 2);
        canvas_draw_str(canvas, 29, y + 9, text2);
    }
}

static void select_view_draw_callback(Canvas* canvas, void* context) {
    SelectViewModel* model = context;
    const char* no_file = "<no file>";

    canvas_draw_textbox(
        canvas,
        0,
        model->selected_file == SelectViewFileA,
        "File A:",
        model->file_a ? model->file_a : no_file);
    canvas_draw_textbox(
        canvas,
        17,
        model->selected_file == SelectViewFileB,
        "File B:",
        model->file_b ? model->file_b : no_file);

    canvas_draw_str(canvas, 0, 60, model->status_text ? model->status_text : "");
}

static bool select_view_input_callback(InputEvent* event, void* context) {
    SelectView* select_view = context;
    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyUp:
            with_view_model(
                select_view->view,
                SelectViewModel * model,
                { model->selected_file = SelectViewFileA; },
                true);
            return true;
        case InputKeyDown:
            with_view_model(
                select_view->view,
                SelectViewModel * model,
                { model->selected_file = SelectViewFileB; },
                true);
            return true;
        case InputKeyOk:
            if(select_view->callback) {
                SelectViewFileType selected_file;
                with_view_model(
                    select_view->view,
                    SelectViewModel * model,
                    { selected_file = model->selected_file; },
                    false);
                select_view->callback(selected_file, select_view->callback_context);
            }
            return true;
        default:
            break;
        }
    }
    return false;
}

SelectView* select_view_alloc(SelectViewCallback callback, void* context) {
    SelectView* v = (SelectView*)malloc(sizeof(SelectView));
    v->callback = callback;
    v->callback_context = context;
    v->view = view_alloc();
    view_allocate_model(v->view, ViewModelTypeLocking, sizeof(SelectViewModel));
    view_set_context(v->view, v);
    view_set_draw_callback(v->view, select_view_draw_callback);
    view_set_input_callback(v->view, select_view_input_callback);

    with_view_model(
        v->view,
        SelectViewModel * model,
        {
            model->selected_file = SelectViewFileA;
            model->file_a = NULL;
            model->file_b = NULL;
            model->status_text = NULL;
        },
        true);

    return v;
}

void select_view_free(SelectView* view) {
    with_view_model(
        view->view,
        SelectViewModel * model,
        {
            free(model->file_a);
            free(model->file_b);
        },
        false);
    view_free_model(view->view);
    view_free(view->view);
    free(view);
}

View* select_view_get_view(SelectView* view) {
    return view->view;
}

void select_view_set_file(SelectView* view, SelectViewFileType file, char* name) {
    size_t len = strlen(name) + 1;
    char* namecpy = (char*)malloc(len);
    memcpy(namecpy, name, len);
    char* old_name;
    with_view_model(
        view->view,
        SelectViewModel * model,
        {
            if(file == SelectViewFileA) {
                old_name = model->file_a;
                model->file_a = namecpy;
            } else {
                old_name = model->file_b;
                model->file_b = namecpy;
            }
        },
        true);
    free(old_name);
}

void select_view_set_status_text(SelectView* view, const char* text) {
    with_view_model(view->view, SelectViewModel * model, { model->status_text = text; }, true);
}
