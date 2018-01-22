/*
 * kvmain.h: entry point for the execution of KOEVA program.
 *
 * BSD 3-clause license
 * Author : Al Rayan, Bogor Agricultural University, 2018 
 *          <alrayan12t@apps.ipb.ac.id>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the BSD 3-Clause License as 
 * published by the BSD Organization; 3-Clause version of the
 * license.
 */

/*

Goal and Theory of Operation

The primary goal of this program is as an entry point of KOEVA
application that handle and control menu loop, all needed opera-
tions that related to image processing, also IO operation such as
update User Interface that conducting in user defined io_handler
functor.

Moreover, image processing will be done in a asynchronous opera-
tions and may utilize all cores. This program will determine
grade of Green Coffee Bean(GBC). GBC image is captured through
two cameras: Top Camera and Bottom Camera.

The cameras, first, will be identified by a system call that run 
script of cameras identification USB port.

On exception, The device will restart itself when error occur. 
Conversely on normal circumstances, program that do graceful exit 
 will bring shutdown action of the device.

 */

#include <stdlib.h>

//#include <kvsession.h>
#include <kvmenu.h>
#include <koeva-util-debug.h>

/*
 * Automate creation of menu struct and attachment to menu 
 * hash table
 */
#define koeva_menu_add(mlist, __id) \
         do { \
          typeof(mlist) ____cursor = koeva_menu_create(#__id); \
          ASSERT_MEM(____cursor); \
          koeva_menu_set_menu_string(____cursor, \
                        kv_menu_ ## __id ## _menu_string); \
          koeva_menu_set_link_to(____cursor, \
                        kv_menu_ ## __id ## _link_to); \
          ____cursor->pre_exec = kv_menu_ ## __id ## _pre_exec; \
          ____cursor->exec = kv_menu_ ## __id ## _exec; \
          HASH_ADD_STR(mlist, id, ____cursor);\
        } while(0)

/*
 * Automate koeva_menu_map_t hash table memory deallocation
 */
#define koeva_menu_free(mlist) \
        do { \
          typeof(mlist) ____cursor, ____temp; \
          HASH_ITER(hh, mlist, ____cursor, ____temp) { \
                HASH_DEL(mlist, ____cursor); \
                koeva_menu_destroy(____cursor); \
          } \
        } while(0)

// Main Menu -----------------------------

const char *kv_menu_main_menu_string[] = {
        "-------KOEVA--------",
        "Insert GBC to tray  ",
        "A:Proceed           ",
        "B:Shutdown device   ",
};

const char *kv_menu_main_link_to[] = {
        "improc",
        "quit",
        NULL,
        NULL
};

int kv_menu_main_pre_exec(struct koeva_menu_map_t *this){return 0;}
int kv_menu_main_exec(struct koeva_menu_map_t *this){return 0;}
// Image Processing Menu -----------------

const char *kv_menu_improc_menu_string[] = {
        "-------KOEVA--------",
        " PROCESSING IMAGE:  ",
        "[[   ..........   ]]",
        "====================",
};

const char *kv_menu_improc_link_to[] = {
        "sumry",
        NULL,
        NULL,
        NULL
};

int kv_menu_improc_pre_exec(struct koeva_menu_map_t *this){return 0;}
int kv_menu_improc_exec(struct koeva_menu_map_t *this){return 0;}
// Summary Menu --------------------------

const char *kv_menu_sumry_menu_string[] = {
        "Processed GCB:      ",
        "Total defects:      ",
        "A:NextTray B:Evaluat",
        "     C:Restart      ",
};

const char *kv_menu_sumry_link_to[] = {
        "improc",
        "eval",
        "main",
        NULL
};

int kv_menu_sumry_pre_exec(struct koeva_menu_map_t *this){return 0;}
int kv_menu_sumry_exec(struct koeva_menu_map_t *this){return 0;}
// Evaulation Menu -----------------------

const char *kv_menu_eval_menu_string[] = {
        "Based on SNI Number:",
        "    1-2907-2008     ",
        "The GBC grade is( ) ",
        "A:Restart Scanning  ",
};

const char *kv_menu_eval_link_to[] = {
        "main",
        "-",
        NULL,
        NULL
};

int kv_menu_eval_pre_exec(struct koeva_menu_map_t *this){return 0;}
int kv_menu_eval_exec(struct koeva_menu_map_t *this){return 0;}
// Quit Menu -----------------------------

const char *kv_menu_quit_menu_string[] = {
        "--------KOEVA-------",
        "    SHUTTING DOWN   ",
        "     THE DEVICE     ",
        "========.....=======",
};

const char *kv_menu_quit_link_to[] = {
        NULL,
        NULL,
        NULL,
        NULL
};

int kv_menu_quit_pre_exec(struct koeva_menu_map_t *this){return 0;}
int kv_menu_quit_exec(struct koeva_menu_map_t *this){return 0;}

// End of Menus --------------------------

/*
 * User defined menu handler functor that will be called
 * in menu loop. See kvmenu.h for detailed documentation.
 */
int io_handler(struct koeva_menu_map_t *this,
               int *select)
{
        int i, link_to_count, c;

        for (link_to_count = 0; this->link_to[link_to_count][0]; 
                link_to_count++);

        link_to_count--;

        for (i = 0; i < 4; i++)
                printf("%s\n", this->menu_string[i]);

        if (link_to_count <= 0) {
                return 0;
        } else if (link_to_count == 1) {
                *select = 0; 
                return 0;
        } else if (link_to_count == 2 
                && this->link_to[1][0] == '-') {
                link_to_count--;
        }

        while (*select < 0 || *select > (link_to_count - 1)) {
                scanf("%d", select);
                while ((c = getchar()) != '\n' && c != EOF);
        }

        return 0;
}
               

int main(void)
{
        struct koeva_menu_map_t *menu_list = NULL;
        koeva_menu_add(menu_list, main);
        koeva_menu_add(menu_list, improc);
        koeva_menu_add(menu_list, sumry);
        koeva_menu_add(menu_list, eval);
        koeva_menu_add(menu_list, quit);

        ASSERT(!koeva_menu_loop_exec(menu_list, "main", io_handler),
               "Loop functions return error!");

        koeva_menu_free(menu_list);

        exit(0);
error:
        exit(EINVAL);
        return 0;
}
