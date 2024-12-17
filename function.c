#include <stdio.h>
#include <stdarg.h>

void my_prinf(int string, ...){
  // declare a va list to hold the arguments. va_list is the type used to store the variable arguments.
  va_list args;

  // Initialize the argument list
  va_start(args, 0); // the last value in the list is 0, indicating the end of the list

  //va_args(args, type) retrieves the next argument

  va_end(args); //clean up the va_list
}


int main() {
}


