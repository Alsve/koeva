#include <uthash.h>
#include <stdio.h>

struct my_struct {
        char name[10];
        int id;
        UT_hash_handle hh;
};

int main(void)
{
        const char **n, *names[] = { "joe", "bob", "betty", NULL };
        struct my_struct *s, *tmp, *users = NULL;
        int i = 0;

        for (n = names; *n != NULL; n++ ) {
                s = malloc(sizeof(*s));
                strncpy(s->name, *n, 10);
                s->id = i++;
                HASH_ADD_STR(users, name, s );
        }

        HASH_FIND_STR(users, "betty", s);
        if (s) printf("betty's id is %d\n", s->id);

        /* free the hash table contents */
        HASH_ITER(hh, users, s, tmp) {
                HASH_DEL(users, s);
                free(s);
        }

        return 0;
}
