                                          /*includes*/ 
                                                                 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

                                          /*defines*/
                                          
#define TRUE 1
#define FALSE 0
#define CODE 0
#define DATA_INT 1
#define DATA_STRING 2
#define ENTRYOREXTERN 34
#define LINE_OF_ASSEMBLI_CODE 81
#define WORD_SIZE 25
#define WORD_SIZE_X 7
#define SYMBOL_SIZE 31

                                           /*structs*/
                                           
typedef struct{
     int symbol_IC;
     int symbol_DC;
     char bin_value[WORD_SIZE];
     char symbol_name[SYMBOL_SIZE];
     int symbol_feature;/*0 = code, 1 = data_int, 2 = data_string, 34 = entry or extern*/
     int symbol_extern; /*0 = FALSE, 1 = TRUE*/
     int symbol_entry; /*0 = FALSE, 1 = TRUE*/
}symbol;
typedef struct{
     char bin_code[WORD_SIZE];
     char bin_value1[WORD_SIZE];
     char bin_value2[WORD_SIZE];
     int IC;
     int cost;
     int if_extern;
     char extern_name[SYMBOL_SIZE];
}code;

                                         /*global parameters*/
                                         
char this_line[LINE_OF_ASSEMBLI_CODE];
int current_line_number;
int IC;
int DC;
int FIC;
int FDC;
int symbol_flag;
int syntax_error_flag;
int max_of_symbols;
int num_of_symbols;
char* bin_value;
char* bin_value2;
int register_in_first;
int register_in_second;
symbol* symbols;
code* codes;


                                       /*function prototype - main*/
                                   
int main(int arg, char* vargs[]);
int dec_bin_converter(char* bin_char);

                                     /*function prototype - first run*/

int if_symbol_check(char* this_line);
void add_symbol(char* this_line);
void symbols_limit();
void add_data(char* this_line);
void add_extern(char* this_line);
void if_symbol_name_free(char* symbol_name);
int check_symbol_feature(char* command_name);
void add_data_int(char* this_line);
int if_data_command(char* this_line);
void add_data_string(char* this_line);
void cmp_int_data(int num);
void cmp_char_data(char ch);
void command_cost(char* this_line);
                                   
                                    /*function prototype - second run*/

char* delete_simbol(char* this_line);
int command_cost_int(char* this_line);
void add_entry(char* this_line);
void read_code(char* this_line);
void write_opcode(char* this_line);
void whrite_one_word(char* opcode, char* first_word, char* second_word);
void whrite_two_words(char* opcode, char* first_word, char* second_word);
void whrite_three_words(char* opcode, char* first_word, char* second_word);
void write_funct(char* opcode);
void write_regtister(const char* reg);
void write_dec_bin(int range);
void write_dec_bin_v2(int range);

