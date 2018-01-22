#include <uthash.h>
#include <stdio.h>

struct address_hash_t {
        char primary_key[10];
        char address[60];
        double area_of_house;
        double floor_amount;
        UT_hash_handle hh;
};


int main(int argc, char **argv)
{
        int i = 0;
        const char **iterator;
        struct address_hash_t *my_book_address = NULL, 
                              *cursor,
                              *temp;

        const char *primary_key[] = {
                "brabi",
                "kimbo",
                "corot"
        };

        const char *address_of_house[] = {
                "Jl. rambutan no. 16, Jatiasih, Bekasi Selatan",
                "Jl. lawi-lawi E1 no. 11, Jatiwaringin, Bekasi Timur",
                "Jl. brabat raya tidak teratur no. suka-suka, Namek",
                NULL
        };

        const double area_of_house[] = {
                245.121,
                123.456,
                678.890
        };


        double floor_amount[] = {3, 4, 2};

        for (iterator = address_of_house; *iterator != NULL; 
                        iterator++, i++) {
                cursor = malloc(sizeof(*cursor));
                strncpy(cursor->address, *iterator, 59);
                strncpy(cursor->primary_key, primary_key[i], 9);
                cursor->area_of_house = area_of_house[i];
                cursor->floor_amount = floor_amount[i];
                HASH_ADD_STR(my_book_address, primary_key, cursor);
        }

        HASH_FIND_STR(address_of_house, "corot", cursor);
        if (cursor)
                printf("The address of corot is : %s\n", cursor->address);

        HASH_ITER(hh, address_of_house, cursor, tmp) {
                printf("Name : %s\n", cursor->primary_key);
                printf("Address : %s\n", cursor->address);
                printf("Building's area wide : %lf\n", 
                        cursor->area_of_house);
                printf("Building's floor amount : %lf\n\n",
                        cursor->floor_amount);
                HASH_DEL(address_of_house, cursor);
                free(cursor);
        }

        return 0;

}
