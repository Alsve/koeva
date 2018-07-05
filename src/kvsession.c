/*
 * ksession.c: utility for managing session state of application
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

The primary goal of this feature is to hold session state of koeva program
holding defects value of each defects type in an array of double contained 
in koeva_session struct. 
 
variable struct koeva_session will hold several variables such as total_beans; 
beans that have been processed, and defects_array that hold total value of 
each defect produced by analysis.

*/

#include <stdlib.h>
#include <errno.h>

#include <koeva-util-debug.h>
#include <kvsession.h>

/*
 * koeva_session_add_defects - add defects value to index-th index
 * @this: the object session that will be altered
 * @value: using SNI_1_2907_2008_DEFECTS enumerable as index and value
 *
 */
static void koeva_session_add_defects(struct koeva_session *this,
                                      koeva_defect_list value)
{
        this->defects_array[value] += SNI_1_2907_2008_DEFECTS_VALUE[value];
}

/*
 * koeva_session_what_grade - return grade based on SNI 1-2907-2008
 * @this: the object session
 *
 * Returns grade
 */
static int koeva_session_what_grade(struct koeva_session *this)
{
        int i;
        int ret;
        double sum = 0;

        for (i = 0; i < KOPI_BAGUS; i++)
                sum += this->defects_array[i];

        if      (sum < 12)  ret = 1;
        else if (sum < 26)  ret = 2;
        else if (sum < 45)  ret = 3;
        else if (sum < 81)  ret = 4;
        else if (sum < 151) ret = 5;
        else                ret = 6;
 
        return ret;
}

/*
 * kv_session_new - create new session variable
 *
 * Returns struct koeva_session object memory.
 */
struct koeva_session * kv_session_new()
{
        struct koeva_session *new_session = malloc(sizeof(*new_session));
        ASSERT_MEM(new_session);
        new_session->add_defects = koeva_session_add_defects;
        new_session->what_grade = koeva_session_what_grade;
        return new_session;

error:
        return NULL;
}

/*
 * kv_session_destroy - destroy session variable
 * @this: koeva_session object that will be freed
 */
void kv_session_destroy(struct koeva_session *this)
{
        free(this);
}
