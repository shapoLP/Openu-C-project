#include "assembler.h"
///////////////////////////////////////////main function//////////////////////////////////////////////


int main(int arg, char* vargs[])
{                                                      
                                              /*preper to run on all files*/
                                     
     char file_to_open[SYMBOL_SIZE];
     char new_file_ext[SYMBOL_SIZE];
     char new_file_ent[SYMBOL_SIZE];
     char new_file_ob[SYMBOL_SIZE];
     char command[SYMBOL_SIZE];
     char first_char, space = ' ';
     char* comma_ptr;
     char* use;
     int i,j,k;
     
     FILE* file;
     FILE* new;
     
                                                  /*run on all files*/
                                                  
     for(i=1; i<arg; i++)
     {
          //opening new file                                       
          file_to_open[0] = '\0';
          max_of_symbols = 10;
          num_of_symbols = 0;
          symbols = (symbol*) calloc(max_of_symbols,sizeof(symbol));
          codes = (code*) calloc(FIC-99,sizeof(code));
          symbol_flag = FALSE;
          syntax_error_flag = FALSE;
          current_line_number = 0;
          IC = 100;
          DC = 0;
          strcat(file_to_open, vargs[i]);
          strcpy(new_file_ext, file_to_open);
          strcpy(new_file_ent, file_to_open);
          strcpy(new_file_ob, file_to_open);
          strcat(file_to_open, ".as");
          file = fopen(file_to_open,"r");
          if(file == NULL)
               continue;
               
                                             /*run on file - first run*/
                                             
          while(fgets(this_line, LINE_OF_ASSEMBLI_CODE, file) != NULL)
          {    
               //preper new line
               symbol_flag = FALSE;
               current_line_number++;
               first_char = '\0';
               sscanf(this_line, "%c",&first_char);
               //check type of line
               if(first_char == ';')
                    continue;
               if(first_char == '\n')
                    continue;
               for(j=0; j<strlen(this_line)-1; j++)
               {
                    first_char = this_line[j];
                    if(first_char != space)
                         break;
               }
               if(j == strlen(this_line)-1)
                    continue;
               if(if_symbol_check(this_line) == TRUE)
               {
                    symbols_limit();
                    symbol_flag = TRUE;
                    add_symbol(this_line);
               }
               else if(if_data_command(this_line) == TRUE)
               {
                    symbols_limit();
                    add_data(this_line);
               }
               else
               {
                    command_cost(this_line);
               }
          }
          
                                                /*finishing - first run*/
                                                
          FIC = IC-100;
          FDC = DC;
          for(j=0; j<num_of_symbols; j++)
          {
               if(symbols[j].symbol_feature == 1 || symbols[j].symbol_feature == 2)
                    symbols[j].symbol_IC = symbols[j].symbol_DC + IC;
          }
          
                                               /*prepering - second run*/
                                               
          rewind(file);
          current_line_number = 0;
          IC = 100;
          codes = (code*) calloc((FIC),sizeof(code));
          
                                             /*run on file - second run*/
                                             
          while(fgets(this_line, LINE_OF_ASSEMBLI_CODE, file) != NULL)
          {
               register_in_first = 0;
               register_in_second = 0;
               current_line_number++;
               first_char = '\0';
               sscanf(this_line, "%c",&first_char);
               if(first_char == ';')
                    continue;
               if(first_char == '\n')
                    continue;
               for(j=0; j<strlen(this_line)-1; j++)
               {
                    first_char = this_line[j];
                    if(first_char != space)
                         break;
               }
               if(j == strlen(this_line)-1)
                    continue;
               if(if_symbol_check(this_line) == TRUE)
               {
                    strcpy(this_line, delete_simbol(this_line));
               }
               if(if_data_command(this_line) == TRUE)
               {
                    sscanf(this_line, "%s", command);
                    if(check_symbol_feature(command) == 3)
                    {
                         add_entry(this_line);
                         command[0] = '\0';
                         continue;
                    }
                    else
                         continue;
               }
               codes[IC-100].cost = command_cost_int(this_line);
               codes[IC-100].IC = IC;
               read_code(this_line);
          } 
          
                                        /*finishing - second run*/
          
            
          if(syntax_error_flag == TRUE)
              continue;
              
                                        /*prepering to make files*/
                   
          
          rewind(file);
          IC = 100;
                  

                                        /*create file - XXXX.ent*/
           strcat(new_file_ent,".ent");          
           new = fopen(new_file_ent,"w+");
           for(j=0; j<num_of_symbols; j++)
          {
               if(symbols[j].symbol_entry == TRUE)
               {
                    comma_ptr = strchr(symbols[j].symbol_name,':');
                    if(comma_ptr != NULL)
                          comma_ptr[0] = ' ';
                    fprintf(new,"%s%07d\n", symbols[j].symbol_name, symbols[j].symbol_IC);
               }
          }
             
                                        /*rcreate file - XXXX.ext*/
                                        
          strcat(new_file_ext,".ext");          
          new = fopen(new_file_ext,"w+"); 
          for(j=0; j<FIC; j++)
          {
               for(k=0; k<codes[j].cost; k++)
               {
                    if(codes[j].if_extern == TRUE && k != 0)
                    {
                         comma_ptr = strchr(codes[j].extern_name,':');
                         if(comma_ptr != NULL)
                              comma_ptr[0] = ' ';
                         sscanf(codes[j].extern_name, "%s %s", use, comma_ptr);
                         if(comma_ptr != NULL && strlen(comma_ptr) > 0)
                              strcpy(codes[j].extern_name, comma_ptr);
                         fprintf(new, "%s %07d\n", use, IC);
                    }
                    IC++;
               }
          }
                                         
                                        /*create file - XXXX.ob*/
                                        
          IC = 100;                              
          strcat(new_file_ob,".ob");          
          new = fopen(new_file_ob,"w+");
          fprintf(new, "%7d %d\n", FIC, FDC);
          for(j=0; j<FIC; j++)
          {
               for(k=0; k<codes[j].cost; k++)
               {
                    if(k == 0)
                         fprintf(new, "%07d %06x\n", IC, dec_bin_converter(codes[j].bin_code));
                    if(k == 1)
                         fprintf(new, "%07d %06x\n", IC, dec_bin_converter(codes[j].bin_value1));
                    if(k == 2)
                         fprintf(new, "%07d %06x\n", IC, dec_bin_converter(codes[j].bin_value2));
                    IC++;
               }
          }
          for(j=0; j<num_of_symbols; j++)
          {
               if(symbols[j].symbol_feature == 1 || symbols[j].symbol_feature == 2)
               {
                    fprintf(new, "%07d %06x\n", IC, dec_bin_converter(symbols[j].bin_value));
                    IC++;
               }
          }     
     }//finish "for" loop - for file
     free(codes);
     free(symbols);
     return 1; 
}//finish main

/*this function gets string that represents word(24 bits) 
 *and coverte to decimal number.  
 *@parameter:char* word
 *@return:int decimal
*/ 
int dec_bin_converter(char* bin_char)
{
     char this;
     int i, j, one_zero;
     int dec = 0;
     for(i=23,j=0; 0<=i; i--, j++)
     {
          one_zero = (bin_char[i] == 49 ?1:0);
          dec += (int) pow(2,j)*one_zero;
     }
     return dec;


}







