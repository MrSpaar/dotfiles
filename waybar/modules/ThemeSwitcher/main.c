#include "module.h"
#include <stdio.h>
#include <stdlib.h>


#define CONFIG_COUNT 12
#define CURRENT(var) config.var[config.index]
#define OTHER(var) config.var[(config.index+1) % 2]

struct {
    int index;
    char* gtk3[2];
    char* label[2];
    char* scheme[2];
    char* wallpaper[2];

    char *rofi[2];
    char* dunst_fg[2];
    char* dunst_bg[2];
} config = {
    0,
    {NULL, NULL},
    {"Light ☀️", "Dark 🌕"},
    {"light", "dark"},
    {NULL, NULL},
    {NULL, NULL},
    {NULL, NULL},
};

struct {
    const char *key;
    char **dest;
    int allocated;
} settings[CONFIG_COUNT] = {
    {"light_theme_gtk3", config.gtk3, 0},
    {"dark_theme_gtk3", config.gtk3 + 1, 0},
    {"dark_theme_rofi", config.rofi, 0},
    {"light_theme_rofi", config.rofi + 1, 0},
    {"light_theme_wallpaper", config.wallpaper, 0},
    {"dark_theme_wallpaper", config.wallpaper + 1, 0},
    {"light_theme_label", config.label, 0},
    {"dark_theme_label", config.label + 1, 0},
    {"light_theme_dunst_bg", config.dunst_bg, 0},
    {"dark_theme_dunst_bg", config.dunst_bg + 1, 0},
    {"light_theme_dunst_fg", config.dunst_fg, 0},
    {"dark_theme_dunst_fg", config.dunst_fg + 1, 0},
};


int is_dark_theme() {
    FILE *fp = popen(
        "[[ $(gsettings get org.gnome.desktop.interface color-scheme) == \"'prefer-dark'\"  ]] && echo '1' || echo '0'", "r"
    );

    return fgetc(fp) == (int) '1';
}

void apply_theme() {
    char command[256];

    snprintf(
        command, sizeof(command),
        "gsettings set org.gnome.desktop.interface color-scheme 'prefer-%s'", CURRENT(scheme)
    );
    system(command);

    if (CURRENT(gtk3) != NULL) {
        snprintf(
            command, sizeof(command),
            "gsettings set org.gnome.desktop.interface gtk-theme '%s'", CURRENT(gtk3)
        );
        system(command);
    }

    if (CURRENT(wallpaper) != NULL) {
        snprintf(
            command, sizeof(command),
            "hyprctl hyprpaper wallpaper \", %s\"", CURRENT(wallpaper)
        );
        system(command);
    }

    if (CURRENT(rofi) != NULL) {
        snprintf(
            command, sizeof(command),
            "sed -i 's|@theme \"%s\"|@theme \"%s\"|' ~/.config/rofi/config.rasi",
            CURRENT(rofi), OTHER(rofi)
        );
        system(command);
    }

    if (CURRENT(dunst_bg) != NULL) {
        snprintf(
            command, sizeof(command),
            "sed -i 's|background = \"%s\"|background = \"%s\"|' ~/.config/dunst/dunstrc && killall dunst",
            OTHER(dunst_bg), CURRENT(dunst_bg)
        );
        system(command);
    }

    if (CURRENT(dunst_fg) != NULL) {
        snprintf(
            command, sizeof(command),
            "sed -i 's|foreground = \"%s\"|foreground = \"%s\"|' ~/.config/dunst/dunstrc && killall dunst",
            OTHER(dunst_fg), CURRENT(dunst_fg)
        );
        system(command);
    }
}

void update_button(GtkButton *button) {
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(button));

    gtk_style_context_remove_class(context, OTHER(scheme));
    gtk_style_context_add_class(context, CURRENT(scheme));
    gtk_button_set_label(button, CURRENT(label));
}


void onclicked(GtkButton* button) {
    config.index = (config.index + 1) % 2;
    apply_theme();
    update_button(button);
}

void* wbcffi_init(
    const wbcffi_init_info* init_info,
    const wbcffi_config_entry* config_entries,
    size_t config_entries_len
) {
    for (size_t i = 0; i < config_entries_len; i++) {
        for (int j = 0; j < CONFIG_COUNT; j++) {
            if (strcmp(config_entries[i].key, settings[j].key)  == 0) {
                settings[j].allocated = 1;
                *(settings[j].dest) = strdup(config_entries[i].value);
                break;
            }
        }
    }

    GtkContainer *root = init_info->get_root_widget(init_info->obj);
    GtkButton *button = GTK_BUTTON(gtk_button_new_with_label(""));

    config.index = is_dark_theme();
    update_button(button);

    char command[256];
    if (CURRENT(wallpaper) != NULL) {
        snprintf(
            command, sizeof(command),
            "hyprctl hyprpaper preload %s", CURRENT(wallpaper)
        );

        system(command);
    }
    if (OTHER(wallpaper) != NULL) {
        snprintf(
            command, sizeof(command),
            "hyprctl hyprpaper preload %s", OTHER(wallpaper)
        );

        system(command);
    }

    gtk_widget_set_name(GTK_WIDGET(button), "theme-switcher");
    g_signal_connect(button, "clicked", G_CALLBACK(onclicked), NULL);
    gtk_container_add(GTK_CONTAINER(root), GTK_WIDGET(button));

    return init_info->obj;
}

void wbcffi_deinit(void *inst) {
    for (int i = 0; i < CONFIG_COUNT; i++) {
        if (settings[i].allocated == 1)
            free(*settings[i].dest);
    }
}
