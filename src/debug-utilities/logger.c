#include "logger.h"
#include "../devices/serial/serial.h"
#include <stdarg.h>
//---------------------------String funcs------------------------
unsigned int strlen(const char *s)
{
    unsigned int count = 0;

    while(*s!='\0')
    {
        count++;
        s++;
    }
    return count;
}
void puts(char* string)
{
      for(int i=0;i<strlen(string);i++)
    {
        char output = string[i];
        EmeraldDevices_Serial_write_serial((int)output);
    }  
}
void putchar(char character)
{
    EmeraldDevices_Serial_write_serial((int)(character));
}
//--------------------------------------------------Printf-------------------
char *convert(unsigned int num, int base) 
{ 
	static char Representation[]= "0123456789ABCDEF";
	static char buffer[50]; 
	char *ptr; 
	
	ptr = &buffer[49]; 
	*ptr = '\0'; 
	
	do 
	{ 
		*--ptr = Representation[num%base]; 
		num /= base; 
	}while(num != 0); 
	
	return(ptr); 
}

void log(char* format,...) 
{ 
	char* logstr = "[\033[1;33mLOG\033[1;33m\033[0m] ";
    puts(logstr);
	char *traverse; 
	unsigned int i; 
	char *s; 
	
	va_list arg; 
	va_start(arg, format); 
	
	for(traverse = format; *traverse != '\0'; traverse++) 
	{ 
		while( *traverse != '%' ) 
		{ 
			putchar(*traverse);
			traverse++; 
		} 
		
		traverse++; 
		
		switch(*traverse) 
		{ 
			case 'c' : i = va_arg(arg,int);
						putchar(i);
						break; 
						
			case 'd' : i = va_arg(arg,int);
						if(i<0) 
						{ 
							i = -i;
							putchar('-'); 
						} 
						puts(convert(i,10));
						break; 
						
			case 'o': i = va_arg(arg,unsigned int);
						puts(convert(i,8));
						break; 
						
			case 's': s = va_arg(arg,char *);
						puts(s); 
						break; 
						
			case 'x': i = va_arg(arg,unsigned int);
						puts(convert(i,16));
						break; 
		}	
	} 
	
	va_end(arg); 


    EmeraldDevices_Serial_write_serial(0x0A);
} 
 
