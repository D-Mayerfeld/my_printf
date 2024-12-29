#include <stdio.h>
#include <stdarg.h>

void my_prinf(char *input_string, ...){
  char *string = input_string; //The variable "string" now points to the first character in the string
  
  // declare a va_list to hold the arguments. va_list is the type used to store the variable arguments.
  va_list args;

  // Initialize the argument list
  va_start(args, 0); // the last value in the list is 0, indicating the end of the list

  //va_arg(args, type) retrieves the next argument of that type in the list

  va_end(args); //clean up the va_list
  
}


int main() {
}




/* EXTRA CODE
  // check va_list is working
  while (1) {
    int value = va_arg(args, int);
    if (value == 0){
      break;
    }
    printf("%d\n", value);
  }
*/
