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

    //add code here to save the modifier values -- what each is -- and pass into function dependent on data type
    // update the string pointer accordingly
     
    
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
