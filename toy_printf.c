/* toy-io.c
 * Limited versions of printf and scanf
 *
 * Programmer: Mayer Goldberg, 2018
 */

/*complete tasks 1b,1c,1d below*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
/* the states in the printf state-machine */
enum printf_state {
  st_printf_init,
  st_printf_percent,
  st_printf_octal2,
  st_printf_octal3
};

#define MAX_NUMBER_LENGTH 64
#define is_octal_char(ch) ('0' <= (ch) && (ch) <= '7')

int toy_printf(char *fs, ...);

const char *digit = "0123456789abcdef";
const char *DIGIT = "0123456789ABCDEF";

int print_int_helper(unsigned n, int radix, const char *digit) {
  int result;

  if (n < radix) {
    putchar(digit[n]);
    return 1;
  }
  else {
    result = print_int_helper(n / radix, radix, digit);
    putchar(digit[n % radix]);
    return 1 + result;
  }
}

int print_int(unsigned n, int radix, const char * digit) {
  if (radix < 2 || radix > 16) {
    toy_printf("Radix must be in [2..16]: Not %d\n", radix);
    exit(-1);
  }

  if (n > 0) {
    return print_int_helper(n, radix, digit);
  }
  if (n == 0) {
    putchar('0');
    return 1;
  }
  else {
    putchar('-');
    return 1 + print_int_helper(-n, radix, digit);
  }
}

void completeWidth(char toAdd, int width, int len){
    for(int j=0;j<width-len;j++) 
        putchar(toAdd);
}

/* SUPPORTED:
 *   %b, %d, %o, %x, %X -- 
 *     integers in binary, decimal, octal, hex, and HEX
 *   %s -- strings
 */

int toy_printf(char *fs,...) {
  int chars_printed = 0;
  int int_value = 0;
  char *string_value;
  char char_value;
  //char octal_char;
  int arr_size;
  int* int_arr;
  char* char_arr;
  char** str_arr;
  char temp;
  int checkD;
  int width=0;
  int len;
  int digits_num = 0;
  int caseleft = 0;
  int casezero = 0;
  va_list args;
  enum printf_state state;

  va_start(args, fs);

  state = st_printf_init; 

  for (; *fs != '\0'; ++fs) {
    switch (state) {
    case st_printf_init:
      switch (*fs) {
      case '%':
	state = st_printf_percent;
	break;

      default:
	putchar(*fs);
	++chars_printed;
      }
      break;
          

    case st_printf_percent:
      switch (*fs) {
      case '%':
	putchar('%');
	++chars_printed;
	state = st_printf_init;
	break;

      case 'd':
        checkD = va_arg(args, int);
        if(checkD>=0) {
            int_value = checkD;
        }
        else {
            int_value = -checkD;
            putchar('-');
            ++chars_printed;
        }
        
        if(checkD<0) digits_num++;
        while(checkD!=0){
            checkD = checkD/10;
            digits_num++;
        }
        
        if(digits_num <= width && casezero) completeWidth('0',width, digits_num);
	chars_printed += print_int(int_value, 10, digit);
	state = st_printf_init;
	break;
      //task1b
      case 'u':
	int_value = va_arg(args, int);
	chars_printed += print_int((unsigned)int_value, 10, digit);
	state = st_printf_init;
	break;

      case 'b':
	int_value = va_arg(args, int);
	chars_printed += print_int((unsigned)int_value, 2, digit);
	state = st_printf_init;
	break;

      case 'o':
	int_value = va_arg(args, int);
	chars_printed += print_int((unsigned)int_value, 8, digit);
	state = st_printf_init;
	break;
	
      case 'x':
	int_value = va_arg(args, int);
	chars_printed += print_int((unsigned)int_value, 16, digit);
	state = st_printf_init;
	break;

      case 'X':
	int_value = va_arg(args, int);
	chars_printed += print_int((unsigned)int_value, 16, DIGIT);
	state = st_printf_init;
	break;

      case 's':
	string_value = va_arg(args, char *);
        len = strlen(string_value);
        if(len<width && caseleft){
            completeWidth(' ', width, len);
        }
	while(*string_value){
		chars_printed++;
		putchar(*string_value);
		string_value++;
	}
	if(len<width && !caseleft){
            completeWidth(' ', width, len);
            putchar('#');
        }
	state = st_printf_init;
	break;

      case 'c':
	char_value = (char)va_arg(args, int);
	putchar(char_value);
	++chars_printed;
	state = st_printf_init;
	break;
      //task1c
      case 'A':
        temp = *(fs+1);
        switch(temp){
            case 's':
                str_arr = va_arg(args, char **);
                arr_size = va_arg(args, int);
                putchar('{');
                ++chars_printed;
                for(int i=0;i<arr_size;i++){
                    if(i==arr_size-1) chars_printed +=toy_printf("\"%s\"}",str_arr[i]);
                     else chars_printed +=toy_printf("\"%s\" ,",str_arr[i]);
                }
                break;
            case 'c':
                char_arr = va_arg(args, char *);
                arr_size = va_arg(args, int);
                putchar('{');
                ++chars_printed;
                for(int i=0;i<arr_size;i++){
                    if(i==arr_size-1) chars_printed +=toy_printf("%c}",char_arr[i]);
                     else chars_printed +=toy_printf("%c, ",char_arr[i]);
                }
                break;
            case 'd':
                int_arr = va_arg(args, int *);
                arr_size = va_arg(args, int);
                putchar('{');
                ++chars_printed;
                for(int i=0;i<arr_size;i++){
                    if(i==arr_size-1) chars_printed +=toy_printf("%d}",int_arr[i]);
                     else chars_printed +=toy_printf("%d, ",int_arr[i]);
                }
                break;
            case 'u':
                int_arr = va_arg(args, int *);
                arr_size = va_arg(args, int);
                putchar('{');
                ++chars_printed;
                for(int i=0;i<arr_size;i++){
                    if(i==arr_size-1) chars_printed +=toy_printf("%u}",int_arr[i]);
                     else chars_printed +=toy_printf("%u, ",int_arr[i]);
                }
                break;
            case 'b':
                int_arr = va_arg(args, int *);
                arr_size = va_arg(args, int);
                putchar('{');
                ++chars_printed;
                for(int i=0;i<arr_size;i++){
                    if(i==arr_size-1) chars_printed +=toy_printf("%b}",int_arr[i]);
                     else chars_printed +=toy_printf("%b, ",int_arr[i]);
                }
                break;
            case 'o':
                int_arr = va_arg(args, int *);
                arr_size = va_arg(args, int);
                putchar('{');
                ++chars_printed;
                for(int i=0;i<arr_size;i++){
                    if(i==arr_size-1) chars_printed +=toy_printf("%o}",int_arr[i]);
                     else chars_printed +=toy_printf("%o, ",int_arr[i]);
                }
                break;
            case 'x':
                int_arr = va_arg(args, int *);
                arr_size = va_arg(args, int);
                putchar('{');
                ++chars_printed;
                for(int i=0;i<arr_size;i++){
                    if(i==arr_size-1) chars_printed +=toy_printf("%x}",int_arr[i]);
                     else chars_printed +=toy_printf("%x, ",int_arr[i]);
                }
                break;
            case 'X':
                int_arr = va_arg(args, int *);
                arr_size = va_arg(args, int);
                putchar('{');
                ++chars_printed;
                for(int i=0;i<arr_size;i++){
                    if(i==arr_size-1) chars_printed +=toy_printf("%X}",int_arr[i]);
                     else chars_printed +=toy_printf("%X, ",int_arr[i]);
                }
                break;
            default:
                break;
            
            
        }

        ++fs;
        state = st_printf_init;
        break;
        
        //task1d
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            while('0'<=*fs && *fs<='9'){
                width = (*fs - '0') + width*10;
                ++fs;
            }
            fs--;
            state = st_printf_percent;
            break;
            
        case '-':
            caseleft = 1;
            state = st_printf_percent;
            break;
            
        case '0':
            casezero = 1;
            state = st_printf_percent;
            break;
        

      default:
	toy_printf("Unhandled format %%%c...\n", *fs);
	exit(-1);
      }
      break;
    
    default:
      toy_printf("toy_printf: Unknown state -- %d\n", (int)state);
      exit(-1);
    }
  }

  va_end(args);

  return chars_printed;
}


