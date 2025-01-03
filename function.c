#include <stdio.h>
#include <stdarg.h>

//make sure that throws error if not one of the 4 data types
//make sure throws error if any mistake with any of the specifiers
//make sure throws error if tries to retireve from va_list and hits 0 (end of the list) i.e. there were not enough arguments specified by their function call
//make sure that if the general function returns with things still on va_list to throw a warning message
//indicate that I made the choice that if an unknown specifier is used it is ignored and no error message is thrown back

//functions to deal with each data type and their specifiers
void decimal_int(va_list * args, char * flag, char * width, char * precision, char * length){
}

void hexadecimal_int(va_list * args, char * flag, char * width, char * precision, char * length){
}

void character(va_list * args, char * flag, char * width, char * precision, char * length){
}

void string(va_list * args, char * flag, char * width, char * precision, char * length){
}




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

    //ADD THING FOR %%

    if (current == '%'){
      //create pointers for each of the different fields and set equal to NULL. The pointer will remain NULL if no sub-specifier is indicated for the field
      char * flag = NULL;
      char * width = NULL;
      char * precision = NULL;
      char * length = NULL;
      char specifier = '\0'; // the specifier is stored as a character, not a pointer, and has in initial null character value

      string ++; //increment string to point to the next character after the %
      
      //Set each pointer to the first character in that field
      if ((*string == '-')||(*string == '+')||(*string == '#')||(*string == '0')) flag = string; //set flag to point to the first flag
      while ((*string == '-')||(*string == '+')||(*string == '#')||(*string == '0')) string++; //increment string until the end of the flags are reached

      if (((*string >= 48) && (*string <= 57))||(*string == '*')) width = string; //set width to point to the first width specifier
      while (((*string >= 48) && (*string <= 57))||(*string == '*')) string++; //increment string until the end of the width specifiers are reached

      if (*string == '.') precision = string;
      while ((*string == '*')||((*string >= 48) && (*string <= 57))) string++;

      if ((*string == 'h')||(*string == 'l')||(*string == 'j')||(*string == 'z')||(*string == 't')) length = string;
      while ((*string == 'h')||(*string == 'l')||(*string == 'j')||(*string == 'z')||(*string == 't')) string++;

      //CAN CUT THIS OUT -- AND PERGE WITH THE BELOW, ALSO ADD A CHECK FIRST TO MAKE SURE THAT AT THIS POINT YOU ACTUALLY HIT ONE OF THE 4 OPTIONS BELOW
      if (*string == 'd') specifier = 'd';
      if (*string == 'x') specifier = 'x';
      if (*string == 'c') specifier = 'c';
      if (*string == 's') specifier = 's';

      string++; //increment the string pointer -- now pointing to the next character to be printed

      // functions to deal with the data type, argument from args, and specifications 
      if (specifier == 'd') decimal_int(&args, flag, width, precision, length);
      if (specifier == 'x') hexadecimal_int(&args, flag, width, precision, length);
      if (specifier == 'c') character(&args, flag, width, precision, length);
      if (specifier == 's') string(&args, flag, width, precision, length);
      // when the function returns, string is set to the next character to print and va_list is set so va_arg will retrieve the next argument on the stack
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

// check the pointers to each specifier field is working
      if (flag != NULL) putchar(*flag);
      if (width != NULL) putchar(*width);
      if (precision != NULL) putchar(*precision);
      if (length != NULL) putchar(*length);
      putchar(specifier);

      

// check va_list is working
  while (1) {
    int value = va_arg(args, int);
    if (value == 0){
      break;
    }
    printf("%d\n", value);
  }
*/
