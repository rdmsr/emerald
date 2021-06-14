#include <emerald/io/scan.h>
#include <emerald/str/str.h>

char scan_peek(Scanner *scan, size_t offset)
{
    if (scan->head + offset >= scan->size)
    {
        return '\0';
    }

    return scan->buffer[scan->head + offset];
}

char scan_current(Scanner *scan)
{
    return scan_peek(scan, 0);
}

void scan_forward(Scanner *scan)
{
    if (scan_ended(scan))
    {
        return;
    }

    scan->head++;
}

void scan_forward_n(Scanner *scan, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++)
    {
        scan_forward(scan);
    }
}

bool scan_ended(Scanner *scan)
{
    return scan->head + 1 > scan->size;
}

bool scan_continue(Scanner *scan)
{
    return !scan_ended(scan);
}

bool scan_current_is_c(Scanner *scan, char c)
{
    return scan_current_is_b(scan, &c, 1);
}

bool scan_current_is_b(Scanner *scan, char *buffer, size_t size)
{
    size_t i;

    for (i = 0; i < size; i++)
    {
        if (scan_peek(scan, i) != buffer[i])
        {
            return false;
        }
    }

    return true;
}

bool scan_current_is_w(Scanner *scan, char *word)
{
    return scan_current_is_b(scan, word, cstrlen(word));
}

bool scan_any_b(Scanner *scan, char *buffer, size_t size)
{
    size_t i;

    for (i = 0; i < size; i++)
    {
        if (scan_current_is_c(scan, buffer[i]))
        {
            return true;
        }
    }

    return false;
}

bool scan_skip_b(Scanner *scan, char *buffer, size_t size)
{
    if (!scan_current_is_b(scan, buffer, size))
    {
        return false;
    }

    scan_forward_n(scan, size);

    return true;
}

bool scan_any_c(Scanner *scan, char c)
{
    return scan_current(scan) == c;
}

bool scan_any_w(Scanner *scan, char *word)
{
    return scan_any_b(scan, word, cstrlen(word));
}

bool scan_skip_c(Scanner *scan, char c)
{
    return scan_skip_b(scan, &c, 1);
}

bool scan_skip_w(Scanner *scan, char *word)
{
    return scan_skip_b(scan, word, cstrlen(word));
}
