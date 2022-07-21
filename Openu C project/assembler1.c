#include "assembler.h"
///////////////////////////////////////////first run functions//////////////////////////////////////////////

const char *opcodes[16] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"}; 
const char *registers[8] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

/*this function checks if symbols array dont out of rang
 *if so then increases dynamic array
 */
void symbols_limit()
{
      if(num_of_symbols >= max_of_symbols)
      {
           max_of_symbols *= 2;
           symbols = (symbol*) realloc(symbols,sizeof(symbol)*max_of_symbols);
      }
}
/*this function gets line and check if first word is symbol if do return TRUE
 *@parametr:char* line
 *@return:int 1/0
 */
int if_symbol_check(char* this_line)
{
     int size_of_beginning_of_command;
     char beginning_of_command[SYMBOL_SIZE]; 
     sscanf(this_line, "%s", beginning_of_command);
     size_of_beginning_of_command = (strlen(beginning_of_command)-1);
     if(strcmp(beginning_of_command + size_of_beginning_of_command,":") == 0)
          return TRUE;
     else
          return FALSE;
}

/*this function gets line and check if first word is data command if do return TRUE
 *@parametr:char* line
 *@return:int 1/0
 */
int if_data_command(char* this_line){
     int type_feature;
     char beginning_of_command[SYMBOL_SIZE]; 
     sscanf(this_line, "%s", beginning_of_command);
     type_feature = check_symbol_feature(beginning_of_command);
     if(type_feature != 0)
          return TRUE;
     else
          return FALSE;
}
/*this function gets command line when the first word is data and 
 *add data to symbols arrey with all patameters then needs.
 *paraneters:char* this_line
 */
void add_data(char* this_line){
     int type_feature;
     char feature_name[SYMBOL_SIZE];
     char external_symbol[SYMBOL_SIZE];
     sscanf(this_line, "%s", feature_name);
     type_feature = check_symbol_feature(feature_name);
     if(type_feature == 4)
     { 
          sscanf(this_line, "%*s %s", external_symbol);
          symbols[num_of_symbols].symbol_extern = TRUE;
          symbols[num_of_symbols].symbol_entry = FALSE;
          sprintf(symbols[num_of_symbols].bin_value, "%s", "000000000000000000000001");
          symbols[num_of_symbols].symbol_feature = ENTRYOREXTERN;
          symbols[num_of_symbols].symbol_IC = 0;
          symbols[num_of_symbols].symbol_DC = 0;
          sprintf(symbols[num_of_symbols].symbol_name, "%s:", external_symbol);
          num_of_symbols++;
          return;
     }
     else if(type_feature == 3)
     {
          return;
     }   
     else if(type_feature == 1)
     {
          add_data_int(this_line);
          return;
     }
     else if(type_feature == 2)
     {
          add_data_string(this_line);
          return;
     }
}
/*this function gets command line when the first word is symbol and 
 *add symbol to symbols arrey with all symbol's patameters then needs.
 *and chack if name of symbol is free. if not its syntax error.(call to other function)
 *and check the command cost and update.(call to other function)
 *paraneters:char* this_line
 */
void add_symbol(char* this_line)
{    
     int type_feature;
     char symbol_name[SYMBOL_SIZE];
     char feature_name[SYMBOL_SIZE];
     sscanf(this_line, "%s", symbol_name);
     sscanf(this_line, "%*s %s", feature_name);
     if_symbol_name_free(symbol_name);
     type_feature = check_symbol_feature(feature_name);
     sprintf(symbols[num_of_symbols].symbol_name, "%s", symbol_name);
     if(type_feature == 4 || type_feature == 3)
     {
          syntax_error_flag = TRUE;
          printf("line number %d:.entry or .extern came befor symbol\n", current_line_number);  
     }     
     if(type_feature == 0)
     {
          symbols[num_of_symbols].symbol_feature = CODE;
          symbols[num_of_symbols].symbol_extern = FALSE;
          symbols[num_of_symbols].symbol_entry = FALSE;
          symbols[num_of_symbols].symbol_IC = IC;
          num_of_symbols++;
          command_cost(this_line);
          return;
     }     
     else if(type_feature == 1)
     {  
          add_data_int(this_line);
          return;
     }
     else if(type_feature == 2)
     {
          add_data_string(this_line);
          return;
     }
}
/*this function gets symbol name and check if its free if not on the "syntax error flag"
 *and print a warning;
 *@parametr:char* symbol_name
 */
void if_symbol_name_free(char* symbol_name)
{
     char check[SYMBOL_SIZE];
     int i;
     for(i=0; i<=num_of_symbols; i++)
     {    
          if(strcmp(symbol_name, symbols[i].symbol_name) == 0)
          {
               syntax_error_flag = TRUE;
               printf("line number %d:multiply symbols\n",current_line_number);
          }
     }
     for(i=0; i<8; i++)
     {
          sprintf(check, "%s:", registers[i]);
          if(strcmp(symbol_name, check) == 0)
          {
               syntax_error_flag = TRUE;
               printf("line number %d:%s is saved name in assmbly\n", current_line_number, symbol_name);
          }
     }
     for(i=0; i<16; i++)
     {
          sprintf(check, "%s:", opcodes[i]);
          if(strcmp(symbol_name, check) == 0)
          {
               syntax_error_flag = TRUE;
               printf("line number %d:%s is saved name in assmbly\n", current_line_number, symbol_name);
          }
     }
     
}
/*this function gets command name(the word after symbol) and checks 
 *his feature if code or data, returns:
 *1 if .data
 *2 if .string
 *3 if .entry
 *4 if .extern
 *0 if .code
 *@parametr:char* symbol_name
 *@returns:int symbol_feature
 */
int check_symbol_feature(char* command_name)
{ 
     if(strcmp(command_name,".data") == 0)
          return 1;
     else if(strcmp(command_name,".string") == 0)
          return 2;
     else if(strcmp(command_name,".entry") == 0)
          return 3;
     else if(strcmp(command_name,".extern") == 0)
          return 4;
     else
          return 0;        
}
/*this function get a line like: .extern SYMBOL
 *and add to memory.
 *@paramet:char* this_line
 */ 
void add_extern(char* this_line)
{
     char external_symbol[SYMBOL_SIZE];
     if(sscanf(this_line, "%*s %s", external_symbol) == EOF)
     {
          syntax_error_flag = TRUE;
          printf("line number %d:needs symbol after .extern\n", current_line_number);  
     }
     symbols[num_of_symbols].symbol_extern = TRUE;
     symbols[num_of_symbols].symbol_entry = FALSE;
     symbols[num_of_symbols].symbol_feature = ENTRYOREXTERN;
     symbols[num_of_symbols].symbol_IC = 0;
     symbols[num_of_symbols].symbol_DC = 0;
     sprintf(symbols[num_of_symbols].symbol_name, "%s:", external_symbol);
     num_of_symbols++;
}
/*this function get a line like: SYMBOL: .data "abcd"
 *or like: .data "abcd"
 *and add to memory.
 *@paramet:char* this_line
 */ 
void add_data_int(char* this_line)
{
     char value[7];
     int count_nambers, i;
     char co1='\0', co2='\0', co3='\0', co4='\0' , co5='\0', co6='\0', co7='\0';
     int n[8] = {0};
     if(symbol_flag == TRUE)    
          count_nambers = sscanf(this_line,"%*s %*s %d %c %d %c %d %c %d %c %d %c %d %c %d %c %d", &n[0],&co1,&n[1],&co2,&n[2],&co3,&n[3],&co4,&n[4],&co5,&n[5],&co6,&n[6],&co7,&n[7]);
     else
          count_nambers = sscanf(this_line,"%*s %d %c %d %c %d %c %d %c %d %c %d %c %d %c %d", &n[0],&co1,&n[1],&co2,&n[2],&co3,&n[3],&co4,&n[4],&co5,&n[5],&co6,&n[6],&co7,&n[7]);
      count_nambers = count_nambers/2+1;
      if(count_nambers == 0)
      {
               syntax_error_flag = TRUE;
               printf("line number %d:no argoments after .data", current_line_number);
      }
      for(i=0; i<count_nambers; i++)
      {        
           symbols_limit();          
           symbols[num_of_symbols].symbol_DC = DC;
           cmp_int_data(n[i]);
           symbols[num_of_symbols].symbol_extern = FALSE;
           symbols[num_of_symbols].symbol_entry = FALSE; 
           symbols[num_of_symbols].symbol_feature = DATA_INT;
           num_of_symbols++;
           DC++;
      }     
}

/*this function get a line like: SYMBOL: .string "abcd"
 *or like: .string "abcd"
 *and add to memory.
 *@paramet:char* this_line
 */ 
void add_data_string(char* this_line)
{
     int num_of_chars, i;
     char string_to_add[SYMBOL_SIZE];
     if(symbol_flag == TRUE)
     {
          sscanf(this_line,"%*s %*s %s",string_to_add);
     }
     else
     {
          sscanf(this_line,"%*s %s",string_to_add);
     }
     num_of_chars = strlen(string_to_add);
     if(num_of_chars < 2)
     {
               syntax_error_flag = TRUE;
               printf("line number %d:missing argoments after .string", current_line_number);
     }
     for(i=1; i<=num_of_chars-1; i++)
     {
          if(i == num_of_chars-1)
          {
               symbols_limit();
               sprintf(symbols[num_of_symbols].bin_value, "%s", "000000000000000000000000");
               symbols[num_of_symbols].symbol_feature = DATA_STRING;
               symbols[num_of_symbols].symbol_extern = FALSE;
               symbols[num_of_symbols].symbol_entry = FALSE;
               symbols[num_of_symbols].symbol_DC = DC;
               num_of_symbols++;
               DC++;
               continue;
          }
          symbols_limit();
          cmp_char_data(string_to_add[i]);
          symbols[num_of_symbols].symbol_feature = DATA_STRING;
          symbols[num_of_symbols].symbol_extern = FALSE;
          symbols[num_of_symbols].symbol_entry = FALSE;
          symbols[num_of_symbols].symbol_DC = DC;
          num_of_symbols++;
          DC++;
     } 
}
/*this function get a int and comper to X16
 *and returns in char* modal:"xxxxxx"
 *@parameter:int
 *@returns:char* 
 */ 
void cmp_int_data(int num)
{    
     unsigned int mask = 1;
     mask <<= (23);
     while(mask != 0)
     {
          if(num&mask)
               strcat(symbols[num_of_symbols].bin_value, "1");
          else
               strcat(symbols[num_of_symbols].bin_value, "0");
          mask >>=1;
     }
}
/*this function get a charcter and comper to X16
 *and returns in char* modal:"xxxxxx"
 *@parameter:char
 *@returns:char* 
 */ 
void cmp_char_data(char ch)
{    
     int num = ch;
     unsigned int mask = 1;
     mask <<= (23);
     while(mask != 0)
     {
          if(num&mask)
               strcat(symbols[num_of_symbols].bin_value, "1");
          else
               strcat(symbols[num_of_symbols].bin_value, "0");
          mask >>=1;
     }
}
/*this function gets command line and chack how mach words(memory) its cost
 *and update IC(IC += the cost).
 *active sintax_error_flag:missing argoments, missing comma, illigal comma, to many argoments.
 *@parameter:char* this_line
 */
void command_cost(char* this_line)
{
     int how_many_registers = 0;
     char* if_molti_registers;
     char symbol_in_line[SYMBOL_SIZE];
     int size_of_remove=0;
     int i;
     char command[SYMBOL_SIZE], after_command[SYMBOL_SIZE];
     char first_word[SYMBOL_SIZE];
     char second_word[SYMBOL_SIZE];
     char* comma_ptr;
     char first_char, space = ' ';
     first_word[0] = '\0';
     second_word[0] = '\0';
     for(i=0; i<strlen(this_line); i++)
     {
          first_char = this_line[i];
          if(first_char == space)
               size_of_remove++;
          else
               break;
     }
     if(symbol_flag == TRUE)
     {
          sscanf(this_line,"%s %s", symbol_in_line, command);
          size_of_remove += (strlen(symbol_in_line))+1;
          size_of_remove += strlen(command);
          strcpy(after_command, this_line+size_of_remove+1);
          
     }     
     else
     {
          sscanf(this_line, "%s", command);
          size_of_remove += strlen(command);
          strcpy(after_command, this_line+size_of_remove);
     }     
     for(i=0; i<=4; i++)
     {
          if(strcmp(command, opcodes[i]) == 0)
          {
               if(strchr(this_line, ',') == NULL)
               {
                     syntax_error_flag = TRUE;
                     printf("line number %d:comma don't exist\n", current_line_number);
               }
               for(i=0; i<8; i++)
               {
                    if_molti_registers = strstr(this_line, registers[i]);
                    if(if_molti_registers != NULL)
                    {
                         how_many_registers++;
                         strcpy(if_molti_registers, if_molti_registers+2);
                         if(strstr(this_line, registers[i]) != NULL)
                         {
                              syntax_error_flag = TRUE;
                              printf("line number %d:multipy registers\n", current_line_number);
                         }
                    }
               }
               if(after_command[0] == ',' || after_command[1] == ',' || after_command[strlen(after_command)-1] == ',')
               {
                     syntax_error_flag = TRUE;
                     printf("line number %d:illigal comma\n", current_line_number);               
               }
               comma_ptr = strchr(after_command,',');
               if(comma_ptr != NULL)
               {
                    comma_ptr[0] = ' ';
               }
               else
               {
                    syntax_error_flag = TRUE;
                    printf("line number %d:missing comma\n", current_line_number);  
               }
               sscanf(after_command, "%s %s", first_word, second_word);
               if(strlen(first_word) < 1 || strlen(second_word) < 1)
               {
                     
                     syntax_error_flag = TRUE;
                     printf("line number %d:missing parameters\n", current_line_number);               
               }
               how_many_registers = (how_many_registers>2?2:how_many_registers);
               IC += 3-how_many_registers;
               return;
          }
     }
     for(i=5; i<=13; i++)
     {
          if(strcmp(command, opcodes[i]) == 0)
          {
               for(i=0; i<8; i++)
               {
                    if(strstr(this_line, registers[i]) != NULL)
                    how_many_registers++;
               }
               if(strchr(this_line, ',') != NULL)
               {
                     syntax_error_flag = TRUE;
                     printf("line number %d:illegal comma\n", current_line_number);
               }
               else if(strlen(after_command) < 1)
               {
                     syntax_error_flag = TRUE;
                     printf("line number %d:missing parameters\n", current_line_number); 
               }
               how_many_registers = (how_many_registers>1?1:how_many_registers);
               IC += 2-how_many_registers;
               return;
          }
     }
     for(i=14; i<=16; i++)
     {
          if(strcmp(command, opcodes[i]) == 0)
          {
               if(strlen(after_command) > 0)
               {
                    printf("%s",after_command);
                     syntax_error_flag = TRUE;
                     printf("line number %d:illegal parameter\n", current_line_number); 
               }
               IC += 1;
               return;
          }
     }
     syntax_error_flag = TRUE;
     printf("line number %d:command don't exist\n", current_line_number);        
}












