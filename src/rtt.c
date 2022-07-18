#include <printf.h>
#include <SEGGER_RTT.h>
#include <string.h>
#include <rtt.h>

static char rtt_write_buf[RTT_PRINT_BUF_SIZE];

static void rtt_putcp(void *p, char c)
{
    *(*((char **) p))++ = c;
}

void rtt_printf(const char *fmt, ...)
{   
    va_list va;
    char *s = rtt_write_buf;
    
    va_start (va, fmt);
    tfp_format(&s, rtt_putcp, fmt, va);
    va_end (va);
    rtt_putcp(&s, 0);

    SEGGER_RTT_Write(0, rtt_write_buf, strlen(rtt_write_buf));
}
