#include "libk.h"
#include <debug-utilities/logger.h>
void kassert(bool condition)
{
    if (!condition)
        log(ERROR, "Assertion failed!");
}
