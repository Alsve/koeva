#include <uthash.h>
#include <stdio.h>
#include <string.h>

struct map_t_address {
        char name[10];
        char address[60];
        double area_of_house;
        int floor_amount;
        UT_hash_handle hh;
};

int main(int argc, char **argv)
{
        int i;
        int floor_amount[] = {3, 2, 5};
        double area_of_house[] = {123.456, 345.456, 678.890};

        struct map_t_address *my_address_book = NULL;
        struct map_t_address *cursor, *temp;

        const char *name[] = {
                "Brebi",
                "Corot",
                "Kinie"
        };

        const char *address_list[] = {
                "Jl. Rambutan Indah no. 16 RT.06/11, Jatiasih, Bekasi",
                "Jl. Lawi-lawi Blok E6 No. 11, Jatiwaringin, Bekasi",
                "Jl. Namek, Gargantia, Laut Jawa"
        };

        for (i = 0; i < 3; i++) {
                cursor = malloc(sizeof(*cursor));
                strncpy(cursor->name, name[i], 9);
                strncpy(cursor->address, address_list[i], 59);
                cursor->floor_amount = floor_amount[i];
                cursor->area_of_house = area_of_house[i];
                HASH_ADD_STR(my_address_book, name, cursor);
        }

        HASH_FIND_STR(my_address_book, "brebi", cursor);
        if (cursor)
                printf("Brebi is in the hashmap\n");

        printf("I'll reveal what is inside the hashmap table: \n");
        printf("---While at that, you only can see once! \n");

        HASH_ITER(hh, my_address_book, cursor, temp) {
                printf("Name : %s\n", cursor->name);
                printf("Address : %s\n", cursor->address);
                printf("Floor amount : %d\n", cursor->floor_amount);
                printf("Area of house : %lf\n", cursor->area_of_house);
                printf("Deleting the record... DONE!\n\n");
                HASH_DEL(my_address_book, cursor);
                free(cursor);
        }

        return 0;
}
