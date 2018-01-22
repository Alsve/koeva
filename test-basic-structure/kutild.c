#include "include/koeva-util-debug.h"

int main(void)
{
        LOG_E("Hello world! I am an error.");
        LOG_E("Error message with number : %d", 5);
        LOG_D("Hello world! I am a debug message.");
        LOG_D("Debug message with number : %d", 10);
        ASSERT(1 + 1 == 2, "This should not be printed");
        LOG_W("Hello world! I am a warning message.");
        LOG_W("A warning message with a number : %d", 15);
        ASSERT_DEBUG(2 + 2 - 1 == 3, "This should not be printed.");

error:
        return 0;
}
