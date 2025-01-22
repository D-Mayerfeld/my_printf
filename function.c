#include <stdio.h>
#include <stdarg.h>

#define False 0
#define True 1

//make sure that throws error if not one of the 4 data types
//make sure throws error if any mistake with any of the specifiers
//make sure throws error if tries to retireve from va_list and hits 0 (end of the list) i.e. there were not enough arguments specified by their function call
//make sure that if the general function returns with things still on va_list to throw a warning message
//indicate that I made the choice that if an unknown specifier is used it is ignored and no error message is thrown back
// make sure negative numbers naturally print with a negative sign

//functions to deal with each data type and their specifiers
void decimal_int(va_list args, char * flag, char * width, char * precision, char * length, char *stop){

  // Flags
  // While still in flag field and have not hit the start of another field, process each flag
  _Bool leftJ = False; // This will be set to true if justification is specified (because infomation is needed from the width field before printing)
  _Bool plus = False; // add plus sign before number
  _Bool space = False; // if no sign will be written add space before value
  _Bool zero = False;
  while ((flag != NULL) && (flag != width) && (flag != precision) && (flag != length) && (flag != stop)){
    if (*flag == '-') leftJ = True;
    if (*flag == '+') plus = True;
    if ((*flag == ' ') && (plus == False)) space = True; // only applies if no sign is otherwise printed CHECK LATER THAT NOT NEGATIVE (ONLY ADD SPACE IF NOT NEGATIVE)
    if (*flag == '0') zero = True;

    // IF INDICATE BOTH '-' AND '0' IN FLAG FIELD THEN THROW ERROR
    
    flag++; 
  }
  
  //Width (might pop an element from the argument list)
  //ADD ERROR IF YOU HAVE A STAR AND THEN A NUMBER AFTER IT
  int w = 0;
  // if the width field has a '*' then pop the width from the args list, if not then iterate through width until you hit the next field and convert the character number into an integer number (don't need to check that points to a number because done in original function)
  if ((width != NULL) && (*width == '*')) w = va_arg(args, int);
    else {
      while ((width != precision) && (width != length) && (width != stop)){
        w = (w*10) + (*width - '0'); 
        width++;
      }
    }

  //Precision -- (might pop an element from the argument list)
  int p = -1; //NULL value
  // if the width field has a '*' then pop the width from the args list, if not then iterate through width until you hit the next field and convert the character number into an integer number (don't need to check that points to a number because done in original function)
  if ((precision != NULL) && (*precision == '.') && (*(precision + 1) == '*')) p = va_arg(args, int);
  else if (precision && (*precision == '.')){
    precision++;
    p = 0;
    //while still in this field and still a number (if no number specified, remains zero)
    while ((precision != length) && (precision != stop) && (*precision >= 48) && (*precision <= 57)){ 
      p = (p*10) + (*precision - '0'); 
      precision++;
    }
  }

  //MUST ADD SOMETHING HERE TO ACCOUNT FOR ERROR TYPES -- LIKE WHAT IF PUT hhh OR hl... -- for now will only format if exact and nothing afterward (next increment hits stop), but write error message otherwise and remember that will only get here if only allowed letters
  //NOT ALLOWING J,Z,T OPTIONS -- MAKE NOTE OF THAT
  //Length (determine length and pop the decimal value)
  int decimal; //declare variable 
  if (length == NULL){
    decimal = va_arg(args, int);
  } 
  else if ((*length == 'h') && (*(length+1) == 'h') && ((length+2) == stop)){
    decimal = (signed char)va_arg(args, int);
  } 
  else if ((*length == 'h') && ((length+1) == stop)){
    decimal = (short int)va_arg(args, int);
  } 
  else if ((*length == 'l') && ((length+1) == stop)){
    decimal = (long int)va_arg(args, long int);
  } 
  else if ((*length == 'l') && (*(length+1) == 'l') && ((length+2) == stop)){
    decimal = (long long int)va_arg(args, long long int);
  } 
  // else {
  //THROW ERROR
  //}
  
  // CHECK THAT NEXT ARGUMENT MATCHES TYPE AND IF NOT THROW BACK AN ERROR -- FIGURE OUT HOW TO DO THIS
  
  // intializing variables for printing
  int decimalLength = 0; //length of the decimal number to be printed 
  int numFlags = 0; //amount of characters added by flags, and negative sign if negative
  _Bool negative = False; //if number is negative
  int paddingZeros = 0; //number of zeros to add
  int numSpaces = 0; //number of spaces to add

  // update decimalLength and negative variables
  if (decimal == 0) decimalLength = 1;
    else {
      if (decimal < 0){
        negative = True;
        decimal = -decimal;
      }
      int decimalCopy = decimal; //make a copy so it does not alter the value stored in decimal
      while (decimalCopy != 0){
        decimalCopy /= 10;
        decimalLength++;
      }
    }

  // update numFlags 
  if ((plus == True) || (negative == True) || (space == True)) numFlags = 1; 

  //update paddingZeros and numSpaces
  //first check for precision and update padding zeros
  if ((decimalLength + numFlags) < p){
    paddingZeros += (p-(decimalLength + numFlags));
  }
  // add spaces or padding zeros based on indicated width and if 0 flag is indicated
  if ((decimalLength + numFlags + paddingZeros) < w){
    if (zero == True) paddingZeros += (w-(decimalLength + numFlags + paddingZeros));
      else paddingZeros = (w-(decimalLength + numFlags + paddingZeros));
  }

  //print spaces if right justified
  if (leftJ == True) {
    while (numSpaces != 0){
      putchar(' ');
      numSpaces--;
    }
  }
  
  //print sign (neg, plus from flag, space from flag)
  if (negative == True) putchar('-');
  if ((plus == True) && (negative == False)) putchar('+');
  if ((space == True) && (negative == False)) putchar(' '); //don't have to check that plus is False as space can't be True if plus is False
  
  //print leading zeros
  while (paddingZeros != 0){
    putchar('0');
    paddingZeros--;
  }
  
  //print the decimal number
  if (decimal == 0){
    if (p != 0) putchar('0');
  } 
  else{
      char digits[decimalLength + 1];
      digits[decimalLength] = '\0'; //add null terminator to the string
      //decimal int to string
      for (int i = decimalLength - 1; i >= 0; i--){
        digits[i] = (decimal % 10) + '0';
        decimal /+ 10;
      }
      //print the string to the output
      for (int i = 0; digits[i] != '\0'; i++){
        putchar(digits[i]);
      }
    }
  
  //print spaces if left justified
  if (leftJ == False) {
    while (numSpaces != 0){
      putchar(' ');
      numSpaces--;
    }
  }  
}

void hexadecimal_int(va_list args, char * flag, char * width, char * precision, char * length, char *stop){
}

void character(va_list args, char * flag, char * width, char * precision, char * length, char *stop){
}

void strings(va_list args, char * flag, char * width, char * precision, char * length, char *stop){
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
    current = *string; //current points to what is stored in the memory location

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
      char specifier = '\0'; // the specifier is stored as a character, not a pointer, and has an initial null character value

      string ++; //increment string to point to the next character after the %
      
      //Set each pointer to the first character in that field
      //DEAL WITH CASE IF INSERT ## OR ++ --> THROW AN ERROR OR MAKE SURE DOESN'T WORK
      if ((*string == '-')||(*string == '+')||(*string == '#')||(*string == '0')||(*string == ' ')) flag = string; //set flag to point to the first flag
      while ((*string == '-')||(*string == '+')||(*string == '#')||(*string == '0')||(*string == ' ')) string++; //increment string until the end of the flags are reached

      if (((*string >= 48) && (*string <= 57))||(*string == '*')) width = string; //set width to point to the first width specifier
      while (((*string >= 48) && (*string <= 57))||(*string == '*')) string++; //increment string until the end of the width specifiers are reached

      if (*string == '.') precision = string;
      while ((*string == '*')||((*string >= 48) && (*string <= 57))) string++;

      if ((*string == 'h')||(*string == 'l')) length = string;
      while ((*string == 'h')||(*string == 'l')) string++;

      //CAN CUT THIS OUT -- AND MERGE WITH THE BELOW, ALSO ADD A CHECK FIRST TO MAKE SURE THAT AT THIS POINT YOU ACTUALLY HIT ONE OF THE 4 OPTIONS BELOW
      if (*string == 'd') specifier = 'd';
      if (*string == 'x') specifier = 'x';
      if (*string == 'c') specifier = 'c';
      if (*string == 's') specifier = 's';

      string++; //increment the string pointer -- now pointing to the next character to be printed

      char * stop = string; // the variable next contains a copy of the pointer in the string to be used in the functions below
      // functions to deal with the data type, argument from args, and specifications 
      if (specifier == 'd') decimal_int(args, flag, width, precision, length, stop);
      if (specifier == 'x') hexadecimal_int(args, flag, width, precision, length, stop);
      if (specifier == 'c') character(args, flag, width, precision, length, stop);
      if (specifier == 's') strings(args, flag, width, precision, length, stop);
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
