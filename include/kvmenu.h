/*
 * kvmenu.h: utility for managing menu loop
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

Goals and Theory of Operation

The primary goal of this feature is to make abstraction of menu
that will be used and printed through terminal or through LCD
or custom io depending of implementation of io_handler that will
pass as an argument of loop functor. Variable integer _selector_
in the loop function is used to select string in cursor->link_to
which will be used as key for querying hash table that will return
the next menu.

More specifically, struct koeva_menu_map_t is an implementation of
hash map table. Learning library of uthash.h is a prerequisite 
knowledge for doing any further modification.


*/

#ifndef KOEVA_MENU__H
#define KOEVA_MENU__H

#include <stdlib.h>
#include <string.h>
#include "koeva-util-debug.h"
#include <uthash.h>

struct koeva_menu_map_t {
        char id[11];
        char menu_string[4][21];
        char link_to[4][11];
        int (*pre_exec)(struct koeva_menu_map_t*);
        int (*exec)(struct koeva_menu_map_t*);
        UT_hash_handle hh;
};
        

struct koeva_menu_map_t * koeva_menu_create(char *id)
{
        struct koeva_menu_map_t * new_menu = malloc(sizeof(*new_menu));
        ASSERT_MEM(new_menu);

        strncpy(new_menu->id, id, 10);

        return new_menu;
error:
        return NULL;
}

#define koeva_menu_destroy(M) \
        free(M)

#define koeva_menu_set_menu_string(struct, menus) \
        ({ int ____i; for (____i = 0; ____i < 4; ____i++) \
                memcpy((struct)->menu_string[____i], menus[____i], 20);})

#define koeva_menu_set_link_to(struct, links) \
        ({ int ____i; for (____i = 0; ____i < 4; ____i++) \
             if (links[____i]) \
               memcpy((struct)->link_to[____i], links[____i], 20);})

/*
 * koeva_menu_loop_exec - main function for iterating selecting menu
 * @menu_list: a menu_list that is created through HASH_ADD_STR()
 * @start_id: string key for querying hash table of menu_list
 * @io_handler: interface functor
 *
 * Returns 0 if success, -1 if otherwise.
 * Note: ASSERT_MEM is a debug utility from koeva-util-debug.h
 */
int koeva_menu_loop_exec(struct koeva_menu_map_t *menu_list, 
                         char *start_id,
                         int (*io_handler)(struct koeva_menu_map_t*,
                                           int*))
{
        char *key;
        int select;
        struct koeva_menu_map_t *cursor;

        HASH_FIND_STR(menu_list, start_id, cursor);
        ASSERT_MEM(cursor);
        
        for(;;) {
                if(cursor->pre_exec)
                        ASSERT(!cursor->pre_exec(cursor), "fault @%s",
                                __FUNCTION__);

                select = -1;

                ASSERT(!(*io_handler)(cursor, &select), "fault @%s",
                         __FUNCTION__);

                if(select == -1)
                        break;

                if(cursor->exec)
                        ASSERT(!cursor->exec(cursor), "fault @%s",
                                __FUNCTION__);

                key = cursor->link_to[select];
                HASH_FIND_STR(menu_list, key, cursor);
                ASSERT(cursor, "No such menu. Program aborted.");
        }

        return 0;
error:
        return -1;
}


#endif
