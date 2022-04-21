#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include "kirbparse.h"

std::string usage = "Jazzy: Usage: jazzy program.jz [ -h | --help ] [ -p | --preserve-intermediates ] [ -o | --output out ]\n"
                    "\tprogram.jz: your input file\n"
                    "\t-h | --help: display this message\n"
                    "\t-p | --preserve-intermediates: do not delete the temp files after compilation\n"
                    "\t-o | --output: the name of the final binary, default is \'output\'\n";

// Change a file name string from name.originalextension to name_originalextension.newextension
char* format_output(char* input, char *extension);

using std::endl;
int main(int argc, char **argv)
{
  int ret;

  // Setup directory structure
  ret = system("mkdir -p syntan_blocks_temp");
  if (ret != 0)
    exit(1);

  ret = system("mkdir -p jazzy_temp");
  if (ret != 0)
    exit(1);

  // Parse arguments with my library I developed over winter break
  // Scalable and easy to modify
  FILE *argparse_log = fopen("jazzy_temp/argparse_output.txt", "w");
  int num_flags = 2;
  char flags_short[] = "ph";
  char *flags_long[num_flags];
  flags_long[0] = "preserve-intermediates";
  flags_long[1] = "help";

  int num_value_opts = 1;
  char value_opts_short[] = "o";
  char *value_opts_long[1];
  value_opts_long[0] = "output";

  int num_anon;
  char ***anon_args = (char ***) malloc(sizeof(char**));
  int flags_out[num_flags];
  char *value_opts_out[num_value_opts];

  kirbparse_info = argparse_log;  // Log to console
  kirbparse_err = argparse_log; // Print errors on console as well
  kirbparse_debug = 1;      // Show debug output
  kirbparse_werror = 1;     // Treat argparse warnings as errors

  // Pick up command line arguments
  ret = Kirb_parse_all(argc, argv, num_flags, flags_short, flags_long,
                       0, 0,
                       num_value_opts,value_opts_short, value_opts_long,
                       flags_out, value_opts_out, &num_anon, anon_args);

  if (ret != 0)
  {
    std::cout << "Jazzy: Error encountered when parsing arguments, code " << ret << "." << endl;
    if (ret == 1)
      std::cout << usage << endl;
    
    exit(1);
  }

  fclose(argparse_log);

  // Get outputs from arg parse
  if (num_anon < 1)
  {
    std::cout << "Jazzy: no input file specified" << endl;
    exit(1);
  }
  else if (num_anon > 1)
  {
    std::cout << "Jazzy: too many input files" << endl;
    exit(1);
  }

  char *input_name = *anon_args[0];
  int preserve_temporaries = flags_out[0];
  int help = flags_out[1];
  char *output_name;
  if (value_opts_out[0] == NULL)
    output_name = "jazzy_output";
  else
    output_name = value_opts_out[0];
  
  if (help)
  {
    std::cout << usage << endl;
    exit(0);
  }

  std::cout << "Jazzy: Starting Compilation" << endl;

  // Create temp file names and commands
  char *lex_output = format_output(input_name, ".lex");
  char *sym_output = format_output(input_name, ".sym");
  char *asm_output = format_output(input_name, ".asm");

  char lex_command[strlen(input_name) + 11]; // 11
  strcpy(lex_command, "bin/calli ");
  strcat(lex_command, input_name);

  char syn_command[strlen(lex_output) + strlen(sym_output) + strlen(asm_output) + 14]; // 14
  strcpy(syn_command, "bin/syntan ");
  strcat(syn_command, lex_output);
  strcat(syn_command, " ");
  strcat(syn_command, sym_output);
  strcat(syn_command, " ");
  strcat(syn_command, asm_output);

  char asm_command[strlen(asm_output) + strlen(output_name) + 77];
  strcpy(asm_command, "nasm -felf64 ");
  strcat(asm_command, asm_output);
  strcat(asm_command, " -o jazzy_temp/intermediate.o && ld jazzy_temp/intermediate.o -o ");
  strcat(asm_command, output_name);

  // bin/calli input.jz
  ret = system(lex_command);

  if (ret != 0)
    exit(1);

  // bin/syntan input_jz.lex input_jz.sym input_jz.asm
  ret = system(syn_command);
  
  // nasm -felf64 input_jz.asm -o intermediate.o && ld intermediate.o -o output_name
  ret = system(asm_command);

  // Get rid of temporary files
  if(!preserve_temporaries)
  {
    char rm_lexical[strlen(lex_output) + 7];
    strcpy(rm_lexical, "rm -f ");
    strcat(rm_lexical, lex_output);

    char rm_symbol[strlen(sym_output) + 7];
    strcpy(rm_symbol, "rm -f ");
    strcat(rm_symbol, sym_output);

    char rm_asm[strlen(asm_output) + 7];
    strcpy(rm_asm, "rm -f ");
    strcat(rm_asm, asm_output);

    system("rm -rf syntan_blocks_temp");
    system("rm -rf jazzy_temp");
    
    system(rm_lexical);
    system(rm_symbol);
    system(rm_asm);
  }

  free(lex_output);
  free(sym_output);
  free(asm_output);
  free(anon_args);

  std::cout << "Jazzy: Exit" << endl;

  return 0;
}

char* format_output(char* input, char *extension)
{
  char *out = (char*) malloc(strlen(input) + 5); // length of inputname_inputextension.lex\0
  strcpy(out, input);
  char *dot = strchr(out, '.');
  *dot = '_';
  strcat(out, extension);
  return out;
}