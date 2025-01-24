/*
The function returns the number of characters printed. If error encountered, returns -1
Note that if you input too few parameters, the function will run, but will produce unexpected behavior. 
//NOT ALLOWING J,Z,T OPTIONS -- MAKE NOTE OF THAT
// if in the length field indicate the wrong type, or wrong type anywhere in function will not return error and will get unexpected behavior

My changes: 
1. If an error occurs, it still prints everything up to where the error was encountered and stops printing immediately
*/

#include <stdio.h>
#include <stdarg.h>

#define False 0
#define True 1

//functions to deal with each data type and their specifiers
int decimal_int(va_list args, char * flag, char * width, char * precision, char * length, char *stop){
  int charPrintedCounter = 0; //will be added to numPrintedChar when the function returns to the old function

  // Flags
  // While still in flag field and have not hit the start of another field, process each flag
  _Bool leftJ = False; // This will be set to true if justification is specified (because infomation is needed from the width field before printing)
  _Bool plus = False; // add plus sign before number
  _Bool space = False; // if no sign will be written add space before value
  _Bool zero = False;
  _Bool hashtag = False; //in this data type if present it is an error and will print everything otherwise, but will return the error code of -1
  while ((flag != NULL) && (flag != width) && (flag != precision) && (flag != length) && (flag != stop)){
    if (*flag == '-') leftJ = True;
    else if (*flag == '+') plus = True;
    else if (*flag == ' ') space = True; // only applies if no sign is otherwise printed 
    else if (*flag == '0') zero = True;  
    else if (*flag == '#') hashtag = True;
    flag++; 
  }
  
  //Width (might pop an element from the argument list)
  int w = 0;
  // if the width field has a '*' then pop the width from the args list, if not then iterate through width until you hit the next field and convert the character number into an integer number (don't need to check that points to a number because done in original function)
  if ((width != NULL) && (*width == '*')) {
    w = va_arg(args, int);
    //throw an error if width was indicated with a * and a number after it (which will get past the pointers in the my_printf function)
    if ((*(width+1) >= 48) && (*(width+1) <= 57)) return -1;
  }
    else {
      while ((width != NULL) && (width != precision) && (width != length) && (width != stop)){
        w = (w*10) + (*width - '0'); 
        width++;
      }
    }

  //Precision -- (might pop an element from the argument list)
  int p = -1; //NULL value
  // if the width field has a '*' then pop the width from the args list, if not then iterate through width until you hit the next field and convert the character number into an integer number (don't need to check that points to a number because done in original function)
  if ((precision != NULL) && (*precision == '.') && (*(precision + 1) == '*')){
    p = va_arg(args, int);
    if (((precision+2) != length) && ((precision+2) != stop)) return -1; //if there was .* and then a number, throw an error
  } 
  else if ((precision != NULL) && (*precision == '.')){
    precision++;
    p = 0;
    //while still in this field and still a number (if no number specified, remains zero)
    while ((precision != length) && (precision != stop) && (*precision >= 48) && (*precision <= 57)){
      p = (p*10) + (*precision - '0'); 
      precision++;
    }    
  }
  
  //Length (determine length and pop the decimal value)
  long long int decimal; //declare variable 
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
    decimal = va_arg(args, long int);
  } 
  else if ((*length == 'l') && (*(length+1) == 'l') && ((length+2) == stop)){
    decimal = va_arg(args, long long int);
  } 
  else return -1;
  
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
  if (decimalLength < p){
    paddingZeros += (p-decimalLength);
  }
  // add spaces or padding zeros based on indicated width and if 0 flag is indicated
  if ((decimalLength + numFlags + paddingZeros) < w){
    if ((zero == True) && (leftJ == False)) paddingZeros += (w-(decimalLength + numFlags + paddingZeros));
      else numSpaces = (w-(decimalLength + numFlags + paddingZeros));
  }
  

  //print spaces if right justified
  if (leftJ == False) {
    while (numSpaces != 0){
      putchar(' ');
      numSpaces--;
      charPrintedCounter++;
    }
  }
  
  //print sign (neg, plus from flag, space from flag)
  if (negative == True){
    putchar('-');
    charPrintedCounter++;
  } 
  if ((plus == True) && (negative == False)){
    putchar('+');
    charPrintedCounter++;
  } 
  if ((space == True) && (negative == False) && (plus == False)){
    putchar(' '); //don't have to check that plus is False as space can't be True if plus is False
    charPrintedCounter++;
  } 
  
  //print leading zeros
  while (paddingZeros != 0){
    putchar('0');
    paddingZeros--;
    charPrintedCounter++;
  }
  
  //print the decimal number
  if (decimal == 0){
    if (p != 0) {
      putchar('0');
      charPrintedCounter++;
    }
  } 
  else{
      char digits[decimalLength + 1];
      digits[decimalLength] = '\0'; //add null terminator to the string
      
      //decimal int to string
      int i = decimalLength - 1;
      while (i >= 0){
        digits[i] = (decimal % 10) + '0';
        decimal /= 10;
        i--;  
      }
      
      //print the string to the output
      int j = 0;
      while (digits[j] != '\0'){
        putchar(digits[j]);
        j++;
        charPrintedCounter++;
      }
    }
  
  //print spaces if left justified
  if (leftJ == True) {
    while (numSpaces != 0){
      putchar(' ');
      numSpaces--;
      charPrintedCounter++;
    }
  } 
  
  if ((hashtag == True) || ((plus == True) && (space == True)) || ((zero == True) && (leftJ == True))) return -1; //to indicate error even though printed number
  else return charPrintedCounter;
}

int hexadecimal_int(va_list args, char * flag, char * width, char * precision, char * length, char *stop){
  int charPrintedCounter = 0; //will be added to numPrintedChar when the function returns to the old function

  // Flags
  // While still in flag field and have not hit the start of another field, process each flag
  _Bool leftJ = False; // This will be set to true if justification is specified (because infomation is needed from the width field before printing)
  _Bool plus = False; // add plus sign before number
  _Bool space = False; // if no sign will be written add space before value
  _Bool zero = False;
  _Bool hashtag = False; //in this data type if present it is an error and will print everything otherwise, but will return the error code of -1
  while ((flag != NULL) && (flag != width) && (flag != precision) && (flag != length) && (flag != stop)){
    if (*flag == '-') leftJ = True;
    else if (*flag == '+') plus = True;
    else if (*flag == ' ') space = True; // only applies if no sign is otherwise printed 
    else if (*flag == '0') zero = True;  
    else if (*flag == '#') hashtag = True;
    flag++; 
  }
  
  //Width (might pop an element from the argument list)
  int w = 0;
  // if the width field has a '*' then pop the width from the args list, if not then iterate through width until you hit the next field and convert the character number into an integer number (don't need to check that points to a number because done in original function)
  if ((width != NULL) && (*width == '*')) {
    w = va_arg(args, int);
    //throw an error if width was indicated with a * and a number after it (which will get past the pointers in the my_printf function)
    if ((*(width+1) >= 48) && (*(width+1) <= 57)) return -1;
  }
    else {
      while ((width != NULL) && (width != precision) && (width != length) && (width != stop)){
        w = (w*10) + (*width - '0'); 
        width++;
      }
    }

  //Precision -- (might pop an element from the argument list)
  int p = -1; //NULL value
  // if the width field has a '*' then pop the width from the args list, if not then iterate through width until you hit the next field and convert the character number into an integer number (don't need to check that points to a number because done in original function)
  if ((precision != NULL) && (*precision == '.') && (*(precision + 1) == '*')){
    p = va_arg(args, int);
    if (((precision+2) != length) && ((precision+2) != stop)) return -1; //if there was .* and then a number, throw an error
  } 
  else if ((precision != NULL) && (*precision == '.')){
    precision++;
    p = 0;
    //while still in this field and still a number (if no number specified, remains zero)
    while ((precision != length) && (precision != stop) && (*precision >= 48) && (*precision <= 57)){
      p = (p*10) + (*precision - '0'); 
      precision++;
    }    
  }
  
  //Length (determine length and pop the decimal value)
  long long int hex = 0; //declare variable 
  if (length == NULL){
    hex = va_arg(args, unsigned int);
  } 
  else if ((*length == 'h') && (*(length+1) == 'h') && ((length+2) == stop)){
    hex = (unsigned char)va_arg(args, unsigned int);
  } 
  else if ((*length == 'h') && ((length+1) == stop)){
    hex = (unsigned short int)va_arg(args, unsigned int);
  } 
  else if ((*length == 'l') && ((length+1) == stop)){
    hex = va_arg(args, unsigned long int);
  } 
  else if ((*length == 'l') && (*(length+1) == 'l') && ((length+2) == stop)){
    hex = va_arg(args, unsigned long long int);
  } 
  else return -1;
  
  // intializing variables for printing
  int hexLength = 0; //length of the decimal number to be printed 
  int numFlags = 0; //amount of characters added by flags, and negative sign if negative
  _Bool negative = False; //if number is negative
  int paddingZeros = 0; //number of zeros to add
  int numSpaces = 0; //number of spaces to add

  // update hexLength and negative variables
  if (hex == 0) hexLength = 1;
  else if (hex <= 0){
    hexLength = 8;
    negative = True;
  }
  else {  
    long long int hexCopy = hex; //make a copy so it does not alter the value stored in decimal
    while (hexCopy != 0){
      hexCopy >>= 4; //divide by 16 (right shift by 4 bits)
      hexLength++;
    }
  }

  // update numFlags 
  if ((hex != 0) && (hashtag == True)) numFlags = 2; 

  //update paddingZeros and numSpaces
  //first check for precision and update padding zeros
  if (hexLength < p){
    paddingZeros += (p-hexLength);
  }
  // add spaces or padding zeros based on indicated width and if 0 flag is indicated
  if ((hexLength + numFlags + paddingZeros) < w){
    if ((zero == True) && (leftJ == False) && (p == -1)) paddingZeros += (w-(hexLength + numFlags + paddingZeros)); //because if precision is set the 0 flag is ignored and spaces are added
      else numSpaces = (w-(hexLength + numFlags + paddingZeros));
  }

  //print spaces if right justified
  if (leftJ == False) {
    while (numSpaces != 0){
      putchar(' ');
      numSpaces--;
      charPrintedCounter++;
    }
  } 

  //print 0x if indicated and hex is not zero
  if ((hashtag == True) && (hex != 0)){
    putchar('0');
    putchar('x');
    charPrintedCounter = charPrintedCounter + 2;
  }
  
  //print leading zeros
  while (paddingZeros != 0){
    putchar('0');
    paddingZeros--;
    charPrintedCounter++;
  }

  //print the hex number
  if (hex == 0){
    if (p != 0) {
      putchar('0');
      charPrintedCounter++;
    }
  } 
  else if (negative == False){
    char hex_digits[] = "0123456789abcdef"; //array of all the hex characters

    char digits[hexLength + 1];
    digits[hexLength] = '\0'; //add null terminator to the string

    //hex int to string
    int i = hexLength - 1;
    while (i >= 0){
      digits[i] = hex_digits[hex % 16]; //divide by 16 and add the character for the remainder to the string
      hex /= 16;
      i--;  
    }
      
    //print the string to the output
    int j = 0;
    while (digits[j] != '\0'){
      putchar(digits[j]);
      j++;
      charPrintedCounter++;
    }
  }
  else if (negative == True){
    unsigned long long int binary = (unsigned long long int)hex;
    
    char hex_digits[] = "0123456789abcdef"; //array of all the hex characters

    char digits[hexLength + 1];
    digits[hexLength] = '\0'; //add null terminator to the string

    //convert binary to hex
    for (int i = 7; i >= 0; i--){
      digits[i] = hex_digits[binary & 0xF]; //considers the last 4 bits
      binary >>= 4; //shifts binary to the right by 4 bits
    }

    //print hex
    for (int i = 0; i < 8; i++){
      putchar(digits[i]);
      charPrintedCounter++;
    }
  }
  
  //print spaces if left justified
  if (leftJ == True) {
    while (numSpaces != 0){
      putchar(' ');
      numSpaces--;
      charPrintedCounter++;
    }
  } 

  if ((space == True) || (plus == True)) return -1; //if space or plus flags are True, they are ignored and the number is still printed, but it is technically an error, so return -1
  else return charPrintedCounter;
}

int character(va_list args, char * flag, char * width, char * precision, char * length, char *stop){
  int charPrintedCounter = 0; //will be added to numPrintedChar when the function returns to the old function

  // Flags
  // While still in flag field and have not hit the start of another field, process each flag
  _Bool leftJ = False; // This will be set to true if justification is specified (because infomation is needed from the width field before printing)
  _Bool plus = False; // add plus sign before number
  _Bool space = False; // if no sign will be written add space before value
  _Bool zero = False;
  _Bool hashtag = False; //in this data type if present it is an error and will print everything otherwise, but will return the error code of -1
  while ((flag != NULL) && (flag != width) && (flag != precision) && (flag != length) && (flag != stop)){
    if (*flag == '-') leftJ = True;
    else if (*flag == '+') plus = True;
    else if (*flag == ' ') space = True; // only applies if no sign is otherwise printed 
    else if (*flag == '0') zero = True;  
    else if (*flag == '#') hashtag = True;
    flag++; 
  }
  
  //Width (might pop an element from the argument list)
  int w = 0;
  // if the width field has a '*' then pop the width from the args list, if not then iterate through width until you hit the next field and convert the character number into an integer number (don't need to check that points to a number because done in original function)
  if ((width != NULL) && (*width == '*')) {
    w = va_arg(args, int);
    //throw an error if width was indicated with a * and a number after it (which will get past the pointers in the my_printf function)
    if ((*(width+1) >= 48) && (*(width+1) <= 57)) return -1;
  }
    else {
      while ((width != NULL) && (width != precision) && (width != length) && (width != stop)){
        w = (w*10) + (*width - '0'); 
        width++;
      }
    }

  //Precision -- does not work with this data type, so check if pointer set and if so return -1 error, but print the character
  _Bool p = False; 
  if (precision != NULL) p = True; 

  //get the character
  int character = va_arg(args, unsigned int);
  
  // intializing variables for printing
  int numSpaces = 0; //number of spaces to add

  //update numSpaces based on width
  if (w > 1){
    numSpaces = (w-1);
  }

  //print spaces if right justified
  if (leftJ == False) {
    while (numSpaces != 0){
      putchar(' ');
      numSpaces--;
      charPrintedCounter++;
    }
  } 

  //print the character
  putchar(character);
  charPrintedCounter++;
  
  //print spaces if left justified
  if (leftJ == True) {
    while (numSpaces != 0){
      putchar(' ');
      numSpaces--;
      charPrintedCounter++;
    }
  } 

  if ((space == True) || (plus == True) || (hashtag == True) || (zero == True) || (p == True) || (length != NULL)) return -1; //flag issue, char is still printed, but return -1
  else return charPrintedCounter;
}

int strings(va_list args, char * flag, char * width, char * precision, char * length, char *stop){
}




int my_printf(char *input_string, ...){
  int numPrintedChar = 0; //the function will return this value which keeps track of the number of characters written
  
  char * string = input_string; //The variable "string" now points to the first character in the string
  
  // declare a va_list to hold the arguments. va_list is the type used to store the variable arguments.
  va_list args;

  // Initialize the argument list
  va_start(args, input_string); // the last value in the list is 0, indicating the end of the list

  _Bool error = False; //will be set to True if any flags are incorrect
  
  //Loop through the string and write to stdout
  char current; //initializes a variable to store the current character in from the string
  while(*string != '\0'){
    current = *string; //current points to what is stored in the memory location

    // While we are up to a regular character, print the character to the stdout
    if (current != '%'){
      putchar(current);
      numPrintedChar++; 
      string ++; //increments the pointer
    }

    //If %% print one %
    else if ((current == '%') && ((*(string+1) == '%'))){
      putchar('%');
      numPrintedChar++; 
      string = string + 2; //increments the pointer
    }

    else if (current == '%'){
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

      if (*string == '.'){
        precision = string;
        string++;
      } 
      while ((*string == '*')||((*string >= 48) && (*string <= 57))) string++;

      if ((*string == 'h')||(*string == 'l')) length = string;
      while ((*string == 'h')||(*string == 'l')) string++;

      //set the specifier
      if (*string == 'd') specifier = 'd';
      else if (*string == 'x') specifier = 'x';
      else if (*string == 'c') specifier = 'c';
      else if (*string == 's') specifier = 's';

      char * stop = string; // the variable next contains a copy of the pointer in the string to be used in the functions below
      string++; //increment the string pointer -- now pointing to the next character to be printed

      // functions to deal with the data type, argument from args, and specifications 
      int charPrinted = 0;
      if (specifier == 'd') charPrinted = decimal_int(args, flag, width, precision, length, stop);
      else if (specifier == 'x') charPrinted = hexadecimal_int(args, flag, width, precision, length, stop);
      else if (specifier == 'c') charPrinted = character(args, flag, width, precision, length, stop);
      else if (specifier == 's') charPrinted = strings(args, flag, width, precision, length, stop);
      else return -1; //If the specifier is not d, x, c, or s, then return -1 to indicate an error

      // if one of the functions returns with an error (returns -1) then exit the function and return -1 to indicate an error
      if (charPrinted != -1) numPrintedChar += charPrinted;
      else if (charPrinted == -1) error = True;
      
      // when the function returns, string is set to the next character to print and va_list is set so va_arg will retrieve the next argument on the stack
    }
    
  }

  //If the function completed successfully, clean up va_list and return the number of printed characters.
  va_end(args); //clean up the va_list
  if (error == True) return -1;
  return numPrintedChar;
}





int main() {
  
  

  //char ignores the 0,+,space,# flags and only recognizes - flag. It also does not recognize precision or length modifiers
  //hex ignores the + flag and space, but recognizes -,#,0. If 0,- are both indicated, 0 is ignored
  //Furthermore, 0 flag is ignored if precision is set

  //MY_PRINTF PARENT FUNCTION TESTING
  //Regular string
  printf("REGULAR STRING  \n");
  printf("my_printf:");
  my_printf("Hello There!");
  printf("\n   printf:");
  printf("Hello There!");
  printf("\n");
  printf("\n");
  
  // No argument
  printf("NO ARGUMENT \n");
  printf("my_printf:");
  my_printf("");
  printf("\n   printf:");
  printf("");
  printf("\n");

  //%%
  printf("%% TEST \n");
  printf("my_printf:");
  my_printf("Test number %% now");
  printf("\n   printf:");
  printf("Test number %% now");
  printf("\n");
  printf("\n");

  
  //DECIMAL_INT TESTS
  //simple tests
  printf("NEGATIVE NUMBER  \n");
  printf("my_printf:");
  my_printf("Test number %d" , -1);
  printf("\n   printf:");
  printf("Test number %d" , -1);
  printf("\n");
  printf("\n");
  
  printf("WITH TEXT AFTER DECIMAL  \n");
  printf("my_printf:");
  my_printf("Test number %d now" , 1);
  printf("\n   printf:");
  printf("Test number %d now" , 1);
  printf("\n");
  printf("\n");
  
  printf("SIMPLE  \n");
  printf("my_printf:");
  my_printf("Test number %d" , 1);
  printf("\n   printf:");
  printf("Test number %d" , 1);
  printf("\n");
  printf("\n");

  //width tests
  printf("WIDTH SIMPLE \n");
  printf("my_printf:");
  my_printf("Test number %4d now" , 5);
  printf("\n   printf:");
  printf("Test number %4d now" , 5);
  printf("\n");
  printf("\n");
  
  printf("WIDTH SMALLER THAN INT \n");
  printf("my_printf:");
  my_printf("Test number %1d now" , 5234);
  printf("\n   printf:");
  printf("Test number %1d now" , 5234);
  printf("\n");
  printf("\n");
  
  printf("WIDTH WITH * \n");
  printf("my_printf:");
  my_printf("Test number %*d now" , 6, 5234);
  printf("\n   printf:");
  printf("Test number %*d now" , 6, 5234);
  printf("\n");
  printf("\n");

  //precision tests
  printf("PRECISION SIMPLE \n");
  printf("my_printf:");
  my_printf("Test number %.4d now" , 5);
  printf("\n   printf:");
  printf("Test number %.4d now" , 5);
  printf("\n");
  printf("\n");
  
  printf("PRECISION WITH CORRESPONDING LENGTH INT \n");
  printf("my_printf:");
  my_printf("Test number %.1d now" , 5);
  printf("\n   printf:");
  printf("Test number %.1d now" , 5);
  printf("\n");
  printf("\n");
  
  printf("PRECISION WITH * \n");
  printf("my_printf:");
  my_printf("Test number %.*d now" , 2, 5);
  printf("\n   printf:");
  printf("Test number %.*d now" , 2, 5);
  printf("\n");
  printf("\n");
  
  printf("PRECISION OF 0 WITH NON-ZERO INT \n");
  printf("my_printf:");
  my_printf("Test number %.0d now" , 5);
  printf("\n   printf:");
  printf("Test number %.0d now" , 5);
  printf("\n");
  printf("\n");
  
  printf("PRECISION OF 0 WITH ZERO INT \n");
  printf("my_printf:");
  my_printf("Test number %.0d now" , 0);
  printf("\n   printf:");
  printf("Test number %.0d now" , 0);
  printf("\n");
  printf("\n");
  
  printf("PRECISION . WITHOUT number \n");
  printf("my_printf:");
  my_printf("Test number %.d now" , 0);
  printf("\n   printf:");
  printf("Test number %.d now" , 0);
  printf("\n");
  printf("\n");
  
  printf("PRECISION WITH WIDTH (P>W) \n");
  printf("my_printf:");
  my_printf("Test number %3.4d now" , 5);
  printf("\n   printf:");
  printf("Test number %3.4d now" , 5);
  printf("\n");
  printf("\n");
  
  printf("PRECISION WITH WIDTH (W>P) \n");
  printf("my_printf:");
  my_printf("Test number %6.4d now" , 5);
  printf("\n   printf:");
  printf("Test number %6.4d now" , 5);
  printf("\n");
  printf("\n");

  //flag test (#)
  printf("# FLAG \n");
  printf("my_printf:");
  my_printf("Test number %#d now" , 5);
  printf("\n   printf:");
  printf("Test number %#d now" , 5);
  printf("\n");
  printf("\n");
  
  //flag test (0)
  printf("0 FLAG WITH WIDTH AND - FLAG \n");
  printf("my_printf:");
  my_printf("Test number %-04d now" , 5);
  printf("\n   printf:");
  printf("Test number %-04d now" , 5);
  printf("\n");
  printf("\n");
  
  printf("0 FLAG WITH WIDTH \n");
  printf("my_printf:");
  my_printf("Test number %04d now" , 5);
  printf("\n   printf:");
  printf("Test number %04d now" , 5);
  printf("\n");
  printf("\n");
  
  printf("0 FLAG WITH NO WIDTH \n");
  printf("my_printf:");
  my_printf("Test number %0d now" , 5);
  printf("\n   printf:");
  printf("Test number %0d now" , 5);
  printf("\n");
  printf("\n");
  
  //flag test (space)
  printf("SPACE FLAG WITH NEGATIVE NUMBER \n");
  printf("my_printf:");
  my_printf("Test number % d now" , -5);
  printf("\n   printf:");
  printf("Test number % d now" , -5);
  printf("\n");
  printf("\n");
  
  printf("SPACE FLAG WITH POSITIVE NUMBER AND + FLAG \n");
  printf("my_printf:");
  my_printf("Test number %+ d now" , 5);
  printf("\n   printf:");
  printf("Test number %+ d now" , 5);
  printf("\n");
  printf("\n");
  
  printf("SPACE FLAG WITH POSITIVE NUMBER  \n");
  printf("my_printf:");
  my_printf("Test number % d now" , 5);
  printf("\n   printf:");
  printf("Test number % d now" , 5);
  printf("\n");
  printf("\n");
  
  //flag test (+) 
  printf("+ FLAG WITH NEGATIVE NUMBER  \n");
  printf("my_printf:");
  my_printf("Test number %+d now" , -5);
  printf("\n   printf:");
  printf("Test number %+d now" , -5);
  printf("\n");
  printf("\n");

  printf("+ FLAG WITH POSITIVE NUMBER  \n");
  printf("my_printf:");
  my_printf("Test number %+d now" , 5);
  printf("\n   printf:");
  printf("Test number %+d now" , 5);
  printf("\n");
  printf("\n");
  
  printf("+ FLAG WITH PRECISION  \n");
  printf("my_printf:");
  my_printf("Test number %+.4d now" , -5);
  printf("\n   printf:");
  printf("Test number %+.4d now" , -5);
  printf("\n");
  printf("\n");
  
  printf("+ FLAG WITH WIDTH  \n");
  printf("my_printf:");
  my_printf("Test number %+4d now" , -5);
  printf("\n   printf:");
  printf("Test number %+4d now" , -5);
  printf("\n");
  printf("\n");
    
  //flag tests (-)
  printf("- FLAG WITH WIDTH AND PRECISION  \n");
  printf("my_printf:");
  my_printf("Test number %-5.3d now" , 1);
  printf("\n   printf:");
  printf("Test number %-5.3d now" , 1);
  printf("\n");
  printf("\n");
  
  printf("- FLAG WITH WIDTH  \n");
  printf("my_printf:");
  my_printf("Test number %-5d now" , 1);
  printf("\n   printf:");
  printf("Test number %-5d now" , 1);
  printf("\n");
  printf("\n");
  
  printf("- FLAG NO WIDTH  \n");
  printf("my_printf:");
  my_printf("Test number %-d now" , 1);
  printf("\n   printf:");
  printf("Test number %-d now" , 1);
  printf("\n");
  printf("\n");

  //specifier tests
  printf("l SPECIFIER \n");
  printf("my_printf:");
  my_printf("Test number %ld now" , 423432);
  printf("\n   printf:");
  printf("Test number %ld now" , 423432);
  printf("\n");
  printf("\n");
  
  printf("ll SPECIFIER \n");
  printf("my_printf:");
  my_printf("Test number %lld now" , 32423432);
  printf("\n   printf:");
  printf("Test number %lld now" , 32423432);
  printf("\n");
  printf("\n");
  
  printf("h SPECIFIER \n");
  printf("my_printf:");
  my_printf("Test number %hd now" , 12345);
  printf("\n   printf:");
  printf("Test number %hd now" , 12345);
  printf("\n");
  printf("\n");
  
  printf("hh SPECIFIER \n");
  printf("my_printf:");
  my_printf("Test number %hhd now" , 12345);
  printf("\n   printf:");
  printf("Test number %hhd now" , 12345);
  printf("\n");
  printf("\n");


  //HEXADECIMAL_INT TESTS
  //simple tests
  printf("SIMPLE  \n");
  printf("my_printf:");
  my_printf("Test number %x" , 132432);
  printf("\n   printf:");
  printf("Test number %x" , 132432);
  printf("\n");
  printf("\n");
  
  //width tests
  printf("WIDTH SIMPLE \n");
  printf("my_printf:");
  my_printf("Test number %4x now" , 5);
  printf("\n   printf:");
  printf("Test number %4x now" , 5);
  printf("\n");
  printf("\n");
  
  printf("WIDTH WITH * \n");
  printf("my_printf:");
  my_printf("Test number %*x now" , 6, 5234);
  printf("\n   printf:");
  printf("Test number %*x now" , 6, 5234);
  printf("\n");
  printf("\n");
  
  //precision tests
  printf("PRECISION SIMPLE \n");
  printf("my_printf:");
  my_printf("Test number %.4x now" , 50);
  printf("\n   printf:");
  printf("Test number %.4x now" , 50);
  printf("\n");
  printf("\n");
  
  printf("PRECISION WITH CORRESPONDING LENGTH HEX \n");
  printf("my_printf:");
  my_printf("Test number %.1x now" , 1);
  printf("\n   printf:");
  printf("Test number %.1x now" , 1);
  printf("\n");
  printf("\n");
  
  printf("PRECISION WITH * \n");
  printf("my_printf:");
  my_printf("Test number %.*x now" , 2, 5);
  printf("\n   printf:");
  printf("Test number %.*x now" , 2, 5);
  printf("\n");
  printf("\n");
  
  printf("PRECISION OF 0 WITH NON-ZERO HEX \n");
  printf("my_printf:");
  my_printf("Test number %.0x now" , 5);
  printf("\n   printf:");
  printf("Test number %.0x now" , 5);
  printf("\n");
  printf("\n");
  
  printf("PRECISION OF 0 WITH ZERO HEX \n");
  printf("my_printf:");
  my_printf("Test number %.0x now" , 0);
  printf("\n   printf:");
  printf("Test number %.0x now" , 0);
  printf("\n");
  printf("\n");
  
  printf("PRECISION . WITHOUT number \n");
  printf("my_printf:");
  my_printf("Test number %.x now" , 0);
  printf("\n   printf:");
  printf("Test number %.x now" , 0);
  printf("\n");
  printf("\n");
  
  printf("PRECISION WITH WIDTH (P>W) \n");
  printf("my_printf:");
  my_printf("Test number %3.4x now" , 5);
  printf("\n   printf:");
  printf("Test number %3.4x now" , 5);
  printf("\n");
  printf("\n");
  
  printf("PRECISION WITH WIDTH (W>P) \n");
  printf("my_printf:");
  my_printf("Test number %6.4x now" , 526);
  printf("\n   printf:");
  printf("Test number %6.4x now" , 526);
  printf("\n");
  printf("\n");
  
  //flag test (#)
  printf("# FLAG \n");
  printf("my_printf:");
  my_printf("Test number %#x now" , 54354354);
  printf("\n   printf:");
  printf("Test number %#x now" , 54354354);
  printf("\n");
  printf("\n");
  
  //flag test (0)
  printf("0 FLAG WITH WIDTH AND - FLAG \n");
  printf("my_printf:");
  my_printf("Test number %-04x now" , 5);
  printf("\n   printf:");
  printf("Test number %-04x now" , 5);
  printf("\n");
  printf("\n");
  
  printf("0 FLAG WITH WIDTH \n");
  printf("my_printf:");
  my_printf("Test number %04x now" , 5);
  printf("\n   printf:");
  printf("Test number %04x now" , 5);
  printf("\n");
  printf("\n");
  
  printf("0 FLAG WITH NO WIDTH \n");
  printf("my_printf:");
  my_printf("Test number %0x now" , 5);
  printf("\n   printf:");
  printf("Test number %0x now" , 5);
  printf("\n");
  printf("\n");
  
  //flag test (space)
  printf("SPACE FLAG \n");
  printf("my_printf:");
  my_printf("Test number % x now" , 5);
  printf("\n   printf:");
  printf("Test number % x now" , 5);
  printf("\n");
  printf("\n");
  
  //flag test (+) 
  printf("+ FLAG  \n");
  printf("my_printf:");
  my_printf("Test number %+.4d now" , -5);
  printf("\n   printf:");
  printf("Test number %+.4d now" , -5);
  printf("\n");
  printf("\n");
  
  //flag tests (-)
  printf("- FLAG WITH WIDTH AND PRECISION  \n");
  printf("my_printf:");
  my_printf("Test number %-5.3x now" , 42);
  printf("\n   printf:");
  printf("Test number %-5.3x now" , 42);
  printf("\n");
  printf("\n");
  
  printf("- FLAG WITH WIDTH  \n");
  printf("my_printf:");
  my_printf("Test number %-5x now" , 1);
  printf("\n   printf:");
  printf("Test number %-5x now" , 1);
  printf("\n");
  printf("\n");
  
  printf("- FLAG NO WIDTH  \n");
  printf("my_printf:");
  my_printf("Test number %-x now" , 1);
  printf("\n   printf:");
  printf("Test number %-x now" , 1);
  printf("\n");
  printf("\n");
  
  //specifier tests
  printf("l SPECIFIER \n");
  printf("my_printf:");
  my_printf("Test number %lx now" , 423432);
  printf("\n   printf:");
  printf("Test number %lx now" , 423432);
  printf("\n");
  printf("\n");
  
  printf("ll SPECIFIER \n");
  printf("my_printf:");
  my_printf("Test number %llx now" , 32423432);
  printf("\n   printf:");
  printf("Test number %llx now" , 32423432);
  printf("\n");
  printf("\n");
  
  printf("h SPECIFIER \n");
  printf("my_printf:");
  my_printf("Test number %hx now" , 12345);
  printf("\n   printf:");
  printf("Test number %hx now" , 12345);
  printf("\n");
  printf("\n");
  
  printf("hh SPECIFIER \n");
  printf("my_printf:");
  my_printf("Test number %hhx now" , 12345);
  printf("\n   printf:");
  printf("Test number %hhx now" , 12345);
  printf("\n");
  printf("\n");
  
}
