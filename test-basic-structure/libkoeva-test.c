#include "../include/koeva-util-debug.h"
#include "../include/libkoeva.h"

int main(void)
{
        int i;
        int grade;
        double defects_value = 0;

        struct koeva_session *app = k_session_new();
        struct koeva_session *app2 = k_session_new();

        for (i = 0; i < KOPI_BAGUS; i++) {
                app->add_defects(app, D_RANTING_TANAH_BATU_BESAR);
                defects_value += app->defects_array[i];
        }

        grade = app->what_grade(app);

        LOG_I("Defects value : %lf", defects_value);
        LOG_I("Grade : %d", grade);

        k_session_destroy(app);
        k_session_destroy(app2);

        return 0;
}
