#include <stdio.h>
#include <stdarg.h>

void my_prinf(char *input_string, ...){
  char * string = input_string; //The variable "string" now points to the first character in the string
  
  // declare a va_list to hold the arguments. va_list is the type used to store the variable arguments.
  va_list args;

  // Initialize the argument list
  va_start(args, 0); // the last value in the list is 0, indicating the end of the list

  //Loop through the string and write to stdout
  char current; //initializes a variable to store the current character in from the string
  while(*string != '\0'){
    current = *string; //current points to that is stored in the memory location

    // While we are up to a regular character, print the character to the stdout
    if (current != '%'){
      putchar(current);
      string ++; //increments the pointer
    }

    //add thing for %%

    if (current == '%'){
      //create pointers for each of the different fields and set equal to NULL. The pointer will remain NULL if no sub-specifier is indicated for the field
      char * flag = NULL;
      int * width = NULL;
      int * precision = NULL;
      char * length = NULL;
      char specifier = NULL; // the specifier is stored as a character, not a pointer

      string ++; //increment string to point to the next character after the %
      
      //Set each pointer to the first character in that field
      if ((*string == '-')||(*string == '+')||(*string == '#')||(*string == '0')) flag = string; //set flag to point to the first flag
      while ((*string == '-')||(*string == '+')||(*string == '#')||(*string == '0')) string++; //increment string until the end of the flags are reached

      if (((*string >= 48) && (*string <= 57))||(*string == '*')) width = string; //set width to point to the first width specifier
      while (((*string >= 48) && (*string <= 57))||(*string == '*')) string++; //increment string until the end of the width specifiers are reached

      if (*string == '.') precision = string;
      while ((*string == '*')((*string >= 48) && (*string <= 57))) string++;

      if ((*string == 'h')||(*string == 'l')||(*string == 'j')||(*string == 'z')||(*string == 't')) length = string;
      while ((*string == 'h')||(*string == 'l')||(*string == 'j')||(*string == 'z')||(*string == 't')) string++;

      if (*string == 'd') specifier = d;
      if (*string == 'x') specifier = x;
      if (*string == 'c') specifier = c;
      if (*string == 's') specifier = s;

      string++; //increment the string pointer -- now pointing to the next character to be printed
      
    }
    
     
    
  }

 
  //va_arg(args, type) retrieves the next argument of that type in the list

  va_end(args); //clean up the va_list
  
}


int main() {
}




/* EXTRA CODE

// check string is working and iterate through it
void iterating_string(char *input_string){
  char *string = input_string; //The variable "string" now points to the first character in the string
  char current; //initializes a variable to store the current character in from the string
  for(string; *string != '\0'; string++){
    current = *string;
    putchar(current);
  }
}



// check va_list is working
  while (1) {
    int value = va_arg(args, int);
    if (value == 0){
      break;
    }
    printf("%d\n", value);
  }
*/
