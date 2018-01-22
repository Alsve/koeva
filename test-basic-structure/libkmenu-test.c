#include "../include/kvmenu.h"
#include <unistd.h>

const char *main_menu_string[] = {
        "123",
        "",
        "",
        ""
};

const char *main_menu_link_to[] = {
        "brebi", "brebi", "brebi", "brebi"
};

int main_menu_pre_exec(struct koeva_menu_map_t *this)
{
        printf("Pre-executed function! :)\n");
        return 0;
}

int main_menu_exec(struct koeva_menu_map_t *this)
{
        printf("Exec main menu function! (:\n");
        return 0;
}

int main_menu_io_handler(struct koeva_menu_map_t *this, int *select)
{
        int i, j;
        for (i = 0; i < 4; i++) {
                printf("%s\n", this->menu_string[i]);
                usleep(500 * 1000);
        }

        scanf("%d", select);

        return 0;
}

int main(void)
{
        struct koeva_menu_map_t *cursor = koeva_menu_create("brebi");
        struct koeva_menu_map_t *temp;
        struct koeva_menu_map_t *koeva_menu_list = NULL; 

        koeva_menu_set_menu_string(cursor, main_menu_string);
        koeva_menu_set_link_to(cursor, main_menu_link_to);

        cursor->pre_exec = main_menu_pre_exec;
        cursor->exec = main_menu_exec;

        HASH_ADD_STR(koeva_menu_list, id, cursor);

        koeva_menu_loop_exec(koeva_menu_list, "brebi",
                        main_menu_io_handler); 

        HASH_ITER(hh, koeva_menu_list, cursor, temp) {
                HASH_DEL(koeva_menu_list, cursor);
                free(cursor);
        }

        return 0;
}
