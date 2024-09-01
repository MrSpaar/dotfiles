#pragma once

#include <stdint.h>
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

const size_t wbcffi_version = 1;
typedef struct wbcffi_module wbcffi_module;

typedef struct {
    wbcffi_module* obj;
    const char* waybar_version;
    GtkContainer* (*get_root_widget)(wbcffi_module* obj);
    void (*queue_update)(wbcffi_module*);
} wbcffi_init_info;

typedef struct {
    const char* key;
    const char* value;
} wbcffi_config_entry;

void* wbcffi_init(
    const wbcffi_init_info* init_info,
    const wbcffi_config_entry* config_entries,
    size_t config_entries_len
);

void wbcffi_deinit(void* instance);
void wbcffi_update(void* instance);
void wbcffi_refresh(void* instance, int signal);
void wbcffi_doaction(void* instance, const char* action_name);

#ifdef __cplusplus
}
#endif
