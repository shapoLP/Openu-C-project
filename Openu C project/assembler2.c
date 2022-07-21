#include "assembler.h"
///////////////////////////////////////////second run functions//////////////////////////////////////////////

const char opcodess[16][5] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"}; 
const char registerss[8][3] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

/*this function check cost of line-code and sent to "whrite_three_words" , "whrite_two_words" 
 *or "whrite_one_word" function to complite whrite according to the number of words required.
 *in addition, will check the number of registers and their location in the code.
 *@parameter:char* this_line.
 */
void read_code(char* this_line)
{
     char opcode[SYMBOL_SIZE];
     char first_word[SYMBOL_SIZE];
     char second_word[SYMBOL_SIZE];
     char* comma_ptr = NULL;
     int i;
     comma_ptr = strchr(this_line,',');
     if(comma_ptr != NULL)
     {
          comma_ptr[0] = ' ';
          sscanf(this_line, "%s %s %s", opcode, first_word, second_word);
     }
     else
     {
          sscanf(this_line, "%s %s ", opcode, first_word);
          second_word[0] = '\0';
     }
     for(i=0; i<8; i++)
     {
          if(strcmp(first_word, registerss[i]) == 0)
               register_in_first = 1;
          if(strcmp(second_word, registerss[i]) == 0)
               register_in_second = 1;  
     }
     if(codes[IC-100].cost == 1)
     {
          whrite_one_word(opcode, first_word, second_word);
     }
     else if(codes[IC-100].cost == 2)
     {
          whrite_two_words(opcode, first_word, second_word);
     }
     else if(codes[IC-100].cost == 3)
     {
          whrite_three_words(opcode, first_word, second_word);
     }
}
/*this function gets code_line that costs three words(24bits) and  
 *complite whriting in binary code this words(24 bits).
 *@parameter:char* opcode
 *@parameter:char* first_word
 *@parameter:char* second_word
 */
void whrite_three_words(char* opcode, char* first_word, char* second_word)
{
     int i;
     char first_char_of_first_word, first_char_of_second_word;
     char comper_to_symbol[SYMBOL_SIZE];
     int range = EOF;
     sscanf(first_word, "%c", &first_char_of_first_word);
     sscanf(second_word, "%c", &first_char_of_second_word);
     write_opcode(opcode);
     if(first_char_of_second_word == '&')
     {
          syntax_error_flag = TRUE;
          printf("line number %d:cant puts '%c'\n", current_line_number, first_char_of_second_word);
     }
     if(first_char_of_first_word == '&')
     {
          syntax_error_flag = TRUE;
          printf("line number %d:cant puts '&'\n", current_line_number);
     }
     else if((first_char_of_first_word == '#'))
     {
          strcat(codes[IC-100].bin_code, "0000001000");
          write_funct(opcode);
          strcat(codes[IC-100].bin_code, "100");
          first_word += 1;
          sscanf(first_word, "%d", &range);
          write_dec_bin(range);
          strcat(codes[IC-100].bin_value1, "100");
          for(i=0; i<num_of_symbols; i++)
          {
               strcpy(comper_to_symbol, second_word);
               strcat(comper_to_symbol,":");
               if(strcmp(comper_to_symbol,symbols[i].symbol_name) == 0)
               {
                    if(symbols[i].symbol_extern == TRUE)
                    {
                         codes[IC-100].if_extern = TRUE;
                         strcat(codes[IC-100].extern_name,symbols[i].symbol_name); 
                         strcat(codes[IC-100].bin_value2, "000000000000000000000001");
                    }
                    else
                    {
                         write_dec_bin_v2(symbols[i].symbol_IC);
                         strcat(codes[IC-100].bin_value2, "010");
                    }
                    IC++;
                    return;
               }
          }
          syntax_error_flag = TRUE;
          printf("line number %d:dont exist symbol.\n", current_line_number);
     }
     else
     {
          if(first_char_of_second_word == '#')
               strcat(codes[IC-100].bin_code, "0100000000");
          else
               strcat(codes[IC-100].bin_code, "0100001000");
           write_funct(opcode);
           strcat(codes[IC-100].bin_code, "100");  
           for(i=0; i<num_of_symbols; i++)
           {
               strcpy(comper_to_symbol, first_word);
               strcat(comper_to_symbol,":");
               if(strcmp(comper_to_symbol,symbols[i].symbol_name) == 0)
               {
                    if(symbols[i].symbol_extern == TRUE)
                    {
                         codes[IC-100].if_extern = TRUE;
                         strcat(codes[IC-100].extern_name,symbols[i].symbol_name); 
                         strcat(codes[IC-100].bin_value1, "000000000000000000000001");
                         continue;
                    }
                    else
                    {
                         write_dec_bin(symbols[i].symbol_IC);
                         strcat(codes[IC-100].bin_value1, "010");
                         continue;
                    }
               }
           }
           if(i == 10 && first_char_of_second_word != '#')
           {
               syntax_error_flag = TRUE;
               printf("line number %d:dont exist symbol.\n", current_line_number);  
           }
           for(i=0; i<num_of_symbols; i++)
           {
               strcpy(comper_to_symbol, second_word);
               strcat(comper_to_symbol,":");
               if(strcmp(comper_to_symbol,symbols[i].symbol_name) == 0)
               {
                    if(symbols[i].symbol_extern == TRUE)
                    {  
                         codes[IC-100].if_extern = TRUE;
                         strcat(codes[IC-100].extern_name,symbols[i].symbol_name); 
                         strcat(codes[IC-100].bin_value2, "000000000000000000000001");
                    }
                    else
                    {
                         write_dec_bin_v2(symbols[i].symbol_IC);
                         strcat(codes[IC-100].bin_value2, "010");
                    }
                    IC++;
                    return;
               }
          }
          if(first_char_of_second_word == '#')
          {
               second_word += 1;
               sscanf(second_word, "%d", &range);
               write_dec_bin_v2(range);
               strcat(codes[IC-100].bin_value2, "100");
               IC++;
               return;
          }
           
     }
     printf("line number %d:unknow error :(.\n", current_line_number);
}
/*this function gets code_line that costs two words(24bits) and  
 *complite whriting in binary code this words(24 bits).
 *@parameter:char* opcode
 *@parameter:char* first_word
 *@parameter:char* second_word
 */
void whrite_two_words(char* opcode, char* first_word, char* second_word)
{
     int i, k, counter = 0;
     int flag = FALSE;
     char first_char_of_first_word;
     char comper_to_symbol[SYMBOL_SIZE];
     int range = EOF; 
     sscanf(first_word, "%c", &first_char_of_first_word);
     write_opcode(opcode);
     if(register_in_first == 1)//if register
     {
          strcat(codes[IC-100].bin_code, "11");
          for(i=0; i<8; i++)
          {
               if(strcmp(first_word, registerss[i]) == 0)
                    write_regtister(registerss[i]);
          }
          strcpy(comper_to_symbol, second_word);
          strcat(comper_to_symbol,":");
          strcat(codes[IC-100].bin_code, "01000");
          write_funct(opcode);
          strcat(codes[IC-100].bin_code, "100");
          for(i=0; i<num_of_symbols; i++)
          {
               if(strcmp(comper_to_symbol,symbols[i].symbol_name) == 0)
               {
                    if(symbols[i].symbol_extern == TRUE)
                    {
                         codes[IC-100].if_extern = TRUE;
                         strcat(codes[IC-100].extern_name,symbols[i].symbol_name); 
                         strcat(codes[IC-100].bin_value1, "000000000000000000000001");
                    }
                    else
                    {
                         write_dec_bin(symbols[i].symbol_IC);
                         strcat(codes[IC-100].bin_value1, "010");
                    }
                    IC++;
                    return;
               }
          }
          syntax_error_flag = TRUE;
          printf("line number %d:dont exist symbol.\n", current_line_number);
          
          
     }//if register - end
     else if(first_char_of_first_word == '&')//if &
     {
          if(strcmp(codes[IC-100].bin_code,"001001") != 0)
          {
               syntax_error_flag = TRUE;
               printf("line number %d:cant puts '&' on %s opcode\n", current_line_number, opcode);
          }
          strcat(codes[IC-100].bin_code, "0000010000");
          write_funct(opcode); 
          strcat(codes[IC-100].bin_code, "100");
          strcpy(comper_to_symbol, first_word+1);
          strcat(comper_to_symbol,":");
          for(i=0; i<num_of_symbols; i++)
          {
               if(strcmp(comper_to_symbol,symbols[i].symbol_name) == 0 && symbols[i].symbol_extern != TRUE)
               {
                    for(k=0; k<(IC-100); k++)
                         counter += codes[k].cost;
                    range = (symbols[i].symbol_IC - (counter+100));
                    
               }
          }
          if(range == EOF)
          {
               syntax_error_flag = TRUE;
               printf("line number %d:dont exist symbol or symbol exterm.\n", current_line_number);
          }
          write_dec_bin(range);
          strcat(codes[IC-100].bin_value1, "100");
          IC++;
          return;
     }//if & - end
     else if((first_char_of_first_word == '#'))//if #
     {
          strcat(codes[IC-100].bin_code, "00000");
          for(i=0; i<8; i++)
          {
               
               if(strcmp(first_word, registerss[i]) == 0)
               {     
                    strcat(codes[IC-100].bin_code, "11");
                    write_regtister(registerss[i]);
                    flag = TRUE;
               }
          }
          if(flag == FALSE)
               strcat(codes[IC-100].bin_code, "00000");
          write_funct(opcode);
          strcat(codes[IC-100].bin_code, "100");
          first_word += 1;
          sscanf(first_word, "%d", &range);
          write_dec_bin(range);
          strcat(codes[IC-100].bin_value1, "100");
          IC++;
          return;
     }//if # - end
     else//if register or symbol 
     {
          if(register_in_second == TRUE)
          {
               strcat(codes[IC-100].bin_code, "01000");
               strcat(codes[IC-100].bin_code, "11");
               for(i=0; i<8; i++)
               {
                    if(strcmp(second_word, registerss[i]) == 0)
                         write_regtister(registerss[i]);
               }
               write_funct(opcode);
               strcat(codes[IC-100].bin_code, "100");   
          }
          else
          {
               strcat(codes[IC-100].bin_code, "0000001000");
               write_funct(opcode);
               strcat(codes[IC-100].bin_code, "100");
          }
          strcat(first_word,":");
          for(i=0; i<num_of_symbols; i++)
          {
               if(strcmp(first_word,symbols[i].symbol_name) == 0)
               {
                    if(symbols[i].symbol_extern == TRUE)
                    {
                         codes[IC-100].if_extern = TRUE;
                         strcat(codes[IC-100].extern_name,symbols[i].symbol_name); 
                         strcat(codes[IC-100].bin_value1, "000000000000000000000001");
                    }
                    else
                    {
                         write_dec_bin(symbols[i].symbol_IC);
                         strcat(codes[IC-100].bin_value1, "010");
                    }
                    IC++;
                    return;
               }
          }
          IC++;
          syntax_error_flag = TRUE;
          printf("line number %d:dont exist symbol.\n", current_line_number);
     }//if register or symbol - end
     printf("line number %d:unknow error :(.\n", current_line_number);    
}
/*this function gets code_line that costs one word(24bits) and  
 *complite whriting in binary code this word(24 bits).
 *@parameter:char* opcode
 *@parameter:char* first_word
 *@parameter:char* second_word
 */
void whrite_one_word(char* opcode, char* first_word, char* second_word)
{
     int i;
     write_opcode(opcode);
     if(register_in_first == 1 && register_in_second == 0)//if register ony in first
     {
          strcat(codes[IC-100].bin_code, "0000011");
          for(i=0; i<8; i++)
          {
               if(strcmp(first_word, registerss[i]) == 0)
                    write_regtister(registerss[i]);
          }
          write_funct(opcode);  
          strcat(codes[IC-100].bin_code, "100");
          IC++;
          return;
     }//if register ony in first - end
     if(register_in_first == 1)//if register in first
     {
          strcat(codes[IC-100].bin_code, "11");
          for(i=0; i<8; i++)
          {
               if(strcmp(first_word, registerss[i]) == 0)
                    write_regtister(registerss[i]);
          }
     }//if register in first - end
     else//if register not in first
          strcat(codes[IC-100].bin_code, "00000");
     if(register_in_second == 1)//if register in second
     {
          strcat(codes[IC-100].bin_code, "11");
          for(i=0; i<8; i++)
          {
               if(strcmp(second_word, registerss[i]) == 0)
                    write_regtister(registerss[i]);
          }
     }
     else//if register in second - end
          strcat(codes[IC-100].bin_code, "00000");
     write_funct(opcode);  
     strcat(codes[IC-100].bin_code, "100");
     IC++;
}
/*this function gets int value and convert to binari 
 *and save in third word of code(23-3 bits)
 *@parametr:int range.
 */
void write_dec_bin_v2(int range)
{
     unsigned int mask = 1;
     mask <<= (20);
     while(mask != 0)
     {
          if(range&mask)
               strcat(codes[IC-100].bin_value2, "1");
          else
               strcat(codes[IC-100].bin_value2, "0");
          mask >>=1;
     }
}
/*this function gets int value and convert to binari 
 *and save in second word of code(23-3 bits)
 *@parametr:int range.
 */
void write_dec_bin(int range)
{
     unsigned int mask = 1;
     mask <<= (20);
     while(mask != 0)
     {
          if(range&mask)
               strcat(codes[IC-100].bin_value1, "1");
          else
               strcat(codes[IC-100].bin_value1, "0");
          mask >>=1;
     }
}
/*this function gets string that represend register and write in 
 *first word the addres of this register.
 *@parameter:char* reg
 */
void write_regtister(const char* reg)
{
     int i;
     for(i=0; i<8; i++)
     {
          if(strcmp(reg, registerss[i]) == 0)
          {
               switch(i)
               {
                    case 0:
                         strcat(codes[IC-100].bin_code, "000");
                         return;
                    case 1:
                         strcat(codes[IC-100].bin_code, "001");
                         return;
                    case 2:
                         strcat(codes[IC-100].bin_code, "010");
                         return;
                    case 3:
                         strcat(codes[IC-100].bin_code, "011");
                         return;
                    case 4:
                         strcat(codes[IC-100].bin_code, "100");
                         return;
                    case 5:
                         strcat(codes[IC-100].bin_code, "101");
                         return;
                    case 6:
                         strcat(codes[IC-100].bin_code, "110");
                         return;
                    case 7:
                         strcat(codes[IC-100].bin_code, "111");
                         return;
                   }
            }
      }
}
/*this function gets string that represend opcode and write in 
 *first word the funct of this opcode.
 *@parameter:char* opcode
 */
void write_funct(char* opcode)
{
     int i;
     for(i=0; i<16; i++)
     {
          if(strcmp(opcode, opcodess[i]) == 0)
          {
               switch(i)
               {
                    case 0:
                         strcat(codes[IC-100].bin_code, "00000");
                         return;
                    case 1:
                         strcat(codes[IC-100].bin_code, "00000");
                         return;
                    case 2:
                         strcat(codes[IC-100].bin_code, "00001");
                         return;
                    case 3:
                         strcat(codes[IC-100].bin_code, "00010");
                         return;
                    case 4:
                         strcat(codes[IC-100].bin_code, "00000");
                         return;
                    case 5:
                         strcat(codes[IC-100].bin_code, "00001");
                         return;
                    case 6:
                         strcat(codes[IC-100].bin_code, "00010");
                         return;
                    case 7:
                         strcat(codes[IC-100].bin_code, "00011");
                         return;
                    case 8:
                         strcat(codes[IC-100].bin_code, "00100");
                         return;
                    case 9:
                         strcat(codes[IC-100].bin_code, "00001");
                         return;
                    case 10:
                         strcat(codes[IC-100].bin_code, "00010");
                         return;
                    case 11:
                         strcat(codes[IC-100].bin_code, "00011");
                         return;
                    case 12:
                         strcat(codes[IC-100].bin_code, "00000");
                         return;
                    case 13:
                         strcat(codes[IC-100].bin_code, "00000");
                         return;
                    case 14:
                         strcat(codes[IC-100].bin_code, "00000");
                         return;
                    case 15:
                         strcat(codes[IC-100].bin_code, "00000");
                         return;
               }
             
          }
     }
}
/*this function gets string that represend opcode and write in 
 *first word the funct of this opcode.
 *@parameter:char* opcode
 */
void write_opcode(char* opcode)
{
     int i;
     for(i=0; i<16; i++)
     {
          if(strcmp(opcode, opcodess[i]) == 0)
          {
               switch(i)
               {
                    case 0:
                         strcat(codes[IC-100].bin_code, "000000");
                         return;
                    case 1:
                         strcat(codes[IC-100].bin_code, "000001");
                         return;
                    case 2:
                         strcat(codes[IC-100].bin_code, "000010");
                         return;
                    case 3:
                         strcat(codes[IC-100].bin_code, "000010");
                         return;
                    case 4:
                         strcat(codes[IC-100].bin_code, "000100");
                         return;
                    case 5:
                         strcat(codes[IC-100].bin_code, "000101");
                         return;
                    case 6:
                         strcat(codes[IC-100].bin_code, "000101");
                         return;
                    case 7:
                         strcat(codes[IC-100].bin_code, "000101");
                         return;
                    case 8:
                         strcat(codes[IC-100].bin_code, "000101");
                         return;
                    case 9:
                         strcat(codes[IC-100].bin_code, "001001");
                         return;
                    case 10:
                         strcat(codes[IC-100].bin_code, "001001");
                         return;
                    case 11:
                         strcat(codes[IC-100].bin_code, "001001");
                         return;
                    case 12:
                         strcat(codes[IC-100].bin_code, "001100");
                         return;
                    case 13:
                         strcat(codes[IC-100].bin_code, "001101");
                         return;
                    case 14:
                         strcat(codes[IC-100].bin_code, "001110");
                         return;
                    case 15:
                         strcat(codes[IC-100].bin_code, "001111");
                         return;
               }
             
          }
     }
}
/*this function gets line when first word is .entry
 *and change symbols[this symbol].if_entry to TRUE
 *if symbol dont exist its syntax error, print warning.
 *@parameter:char* this_line
 */
void add_entry(char* this_line)
{
     int i;
     char after_command[SYMBOL_SIZE];
     sscanf(this_line,"%*s %s", after_command);
     strcat(after_command, ":");
     for(i=0; i<=num_of_symbols; i++)
     {    
          if(strcmp(after_command, symbols[i].symbol_name) == 0)
          {
               symbols[i].symbol_entry = TRUE;
               return;
          }
     }
          syntax_error_flag = TRUE;
          printf("line number %d:symbol do'not exist\n",current_line_number);
}
/*this function gets line whith symbol and delete the symbol from line and return with
 *out symbol.
 *@parameter:char* this_line
 *@return char* new_line
 */
char* delete_simbol(char* this_line)
{
     char* ptr;
     for(ptr = this_line; ptr-this_line<strlen(this_line); ptr++)
     {
          if((*ptr) == ':')
               return ptr+1;
     }
     return NULL;
}
/*this function check how many words the command need
 *or 1 or 2 or 3
 *@parameter:char* this_line
 *@return int cost
 */
int command_cost_int(char* this_line)
{
     int how_many_registerss = 0;
     int i;
     char command[SYMBOL_SIZE], after_command[SYMBOL_SIZE];
     sscanf(this_line,"%s %s", command, after_command);
     for(i=0; i<=4; i++)
     {
          if(strcmp(command, opcodess[i]) == 0)
          {
               for(i=0; i<8; i++)
               {
                    if(strstr(this_line, registerss[i]) != NULL)
                    how_many_registerss++;
               }
               how_many_registerss = (how_many_registerss>2?2:how_many_registerss);
               return 3-how_many_registerss;
          }
     }
     for(i=5; i<=13; i++)
     {
          if(strcmp(command, opcodess[i]) == 0)
          {
               for(i=0; i<8; i++)
               {
                    if(strstr(this_line, registerss[i]) != NULL)
                    how_many_registerss++;
               }
               how_many_registerss = (how_many_registerss>1?1:how_many_registerss);
               return 2-how_many_registerss;
          }
     }
     for(i=14; i<=16; i++)
     {
          if(strcmp(command, opcodess[i]) == 0)
          {
               return 1;
          }
     }       
}




