#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define add(x, y) ((x) + (y))
#define and(x, y) ((x) && (y))
#define cast(type, x) ((type) x)
#define ccstring const char *
#define cstring char *
#define delete(x) { (free((void*)x)); x = NULL; }
#define deref(x) *(x)
#define div(x, y) ((x) / (y))
#define eq(x, y) ((x) == (y))
#define equals(x, y) ((x) == (y))
#define falsey(x) (!(x))
#define gt(x, y) ((x) > (y))
#define gte(x, y) ((x) >= (y))
#define interpolate_cstring(x, y) x y
#define lt(x, y) ((x) < (y))
#define lte(x, y) ((x) <= (y))
#define mod(x, y) ((x) % (y))
#define mul(x, y) ((x) * (y))
#define nand(x, y) (!((x) && (y)))
#define neg(x) (-(x))
#define nor(x, y) (!((x) || (y)))
#define not(x) (!(x))
#define or(x, y) ((x) || (y))
#define point(x) &(x)
#define sub(x, y) ((x) - (y))
#define truthy(x) (x)
#define xor(x, y) ((x) ^ (y))
#define xnor(x, y) (!((x) ^ (y)))
#define unsafe_index(arr, index) (arr)[(index)]
#define unused(x) (void)(x)

#include "stdio.h"

#include "string.h"

#include "generation/prelude.h"

#include "helpers/system.h"

#include "lex/prelude.h"

#include "macros/helpers.h"

typedef struct {
ccstring source_file_path;
ccstring output_file_path;
bool should_run;
} DaybreakArgs;

typedef struct {
bool is_success;
DaybreakArgs args;
} DaybreakArgParseResult;

typedef struct {
int error;
Token* tokens;
} DaybreakTokenifyResult;

typedef struct {
int error;
} DaybreakEmissionResult;

typedef struct {
int error;
} DaybreakRunResult;

inline int daybreak_compile(const DaybreakArgs args);
inline int _daybreak_compile_program(const cstring output_c_file_path, const DaybreakArgs args);
inline DaybreakEmissionResult _daybreak_emit_program(const DaybreakArgs args, ProgramNode* program);
inline bool daybreak_greeting();
inline DaybreakArgParseResult daybreak_parse_args(const int argc, ccstring* argv);
inline DaybreakArgParseResult _daybreak_parse_arg(ccstring* argv, const int argc, const DaybreakArgs acc);
inline DaybreakRunResult _daybreak_run_program(const DaybreakArgs args);
inline DaybreakTokenifyResult _daybreak_tokenify_file(const DaybreakArgs args);
int daybreak_compile(const DaybreakArgs args) {
{
DaybreakTokenifyResult tokenify_result=_daybreak_tokenify_file(args);
;
if (not(tokenify_result.error))
{
{
ProgramNode* program=ast_parse_program(point(tokenify_result.tokens));
;
if (truthy(program))
{
{
const DaybreakEmissionResult emit_result=_daybreak_emit_program(args, program);
;
if (not(emit_result.error))
{
if (not(args.should_run))
{
return 0;
}
else 
{
{
const DaybreakRunResult run_result=_daybreak_run_program(args);
;
if (not(run_result.error))
{
return 0;
}
else 
{
return run_result.error;
}
}
;
}
;
}
else 
{
return emit_result.error;
}
}
;
}
else 
{
{
LOG_ERROR("Failed to parse program");
return 2;
}
;
}
}
;
}
else 
{
return tokenify_result.error;
}
}
}
int _daybreak_compile_program(const cstring output_c_file_path, const DaybreakArgs args) {
{
if (truthy(args.output_file_path))
{
{
const cstring output_file_path=args.output_file_path;
;
LOG_INFO("Output file: %s", output_file_path);
return compile_c_code(output_c_file_path, output_file_path);
}
;
}
else 
{
{
cstring output_file_path=get_default_output_file();
;
LOG_INFO("Output file: %s", output_file_path);
const int result=compile_c_code(output_c_file_path, output_file_path);
;
delete(output_file_path);
return result;
}
;
}
}
}
DaybreakEmissionResult _daybreak_emit_program(const DaybreakArgs args, ProgramNode* program) {
{
cstring standard_library_directory=get_standard_library_directory();
;
const size_t output_c_file_length=add(1, add(strlen(standard_library_directory), add(strlen(OUTPUT_DIRECTORY), strlen(DEFAULT_SOURCE_OUTPUT))));
;
cstring output_c_file_path=malloc(mul(sizeof(char), output_c_file_length));
;
sprintf(output_c_file_path, interpolate_cstring("%s", interpolate_cstring(OUTPUT_DIRECTORY, DEFAULT_SOURCE_OUTPUT)), standard_library_directory);
delete(standard_library_directory);
const int generation_error=generate_c_code(program, output_c_file_path);
;
ast_free_program_node(program);
if (not(generation_error))
{
{
const int compiler_error=_daybreak_compile_program(output_c_file_path, args);
;
delete(output_c_file_path);
return (DaybreakEmissionResult){.error=compiler_error};
}
;
}
else 
{
{
LOG_ERROR("Failed to generate C code. Received error: %d", generation_error);
return (DaybreakEmissionResult){.error=generation_error};
}
;
}
}
}
bool daybreak_greeting() {
{
printf("%s#####################################%s\n", ANSI_FOREGROUND_YELLOW, ANSI_RESET);
printf("%s# Welcome to the Daybreak compiler! #%s\n", ANSI_FOREGROUND_YELLOW, ANSI_RESET);
printf("%s#####################################%s\n", ANSI_FOREGROUND_YELLOW, ANSI_RESET);
return true;
}
}
DaybreakArgParseResult daybreak_parse_args(const int argc, ccstring* argv) {
{
if (lt(argc, 2))
{
return (DaybreakArgParseResult){.is_success=false};
}
return _daybreak_parse_arg(add(argv, 1), sub(argc, 1), (DaybreakArgs){.source_file_path=NULL, .output_file_path=NULL, .should_run=false});
}
}
DaybreakArgParseResult _daybreak_parse_arg(ccstring* argv, const int argc, const DaybreakArgs acc) {
{
if (lte(argc, 0))
{
return (DaybreakArgParseResult){.is_success=true, .args=acc};
}
else 
{
if (or(eq(0, strcmp(deref(argv), "--output")), eq(0, strcmp(deref(argv), "-o"))))
{
if (lt(argc, 2))
{
return (DaybreakArgParseResult){.is_success=false, .args=acc};
}
else 
{
return _daybreak_parse_arg(add(argv, 2), sub(argc, 2), (DaybreakArgs){.source_file_path=acc.source_file_path, .output_file_path=unsafe_index(argv, 1), .should_run=acc.should_run});
}
;
}
else if (or(eq(0, strcmp(deref(argv), "--run")), eq(0, strcmp(deref(argv), "-r"))))
{
return _daybreak_parse_arg(add(argv, 1), sub(argc, 1), (DaybreakArgs){.source_file_path=acc.source_file_path, .output_file_path=acc.output_file_path, .should_run=true});
}
else 
{
return _daybreak_parse_arg(add(argv, 1), sub(argc, 1), (DaybreakArgs){.source_file_path=deref(argv), .output_file_path=acc.output_file_path, .should_run=acc.should_run});
}
;
}
}
}
DaybreakRunResult _daybreak_run_program(const DaybreakArgs args) {
{
if (truthy(args.output_file_path))
{
return (DaybreakRunResult){.error=run_command(args.output_file_path, stdout)};
}
else 
{
{
const cstring output_file_path=get_default_output_file();
;
const DaybreakRunResult result=(DaybreakRunResult){.error=run_command(output_file_path, stdout)};
;
delete(output_file_path);
return result;
}
;
}
}
}
DaybreakTokenifyResult _daybreak_tokenify_file(const DaybreakArgs args) {
{
const int directory_setup_error=setup_language_directories();
;
if (truthy(directory_setup_error))
{
return (DaybreakTokenifyResult){.error=directory_setup_error, .tokens=NULL};
}
FILE* source_file=lex_open_file(args.source_file_path, DAYBREAK_NEXT_IMPORT_POINTER);
;
if (falsey(source_file))
{
return (DaybreakTokenifyResult){.error=1, .tokens=NULL};
}
Token* tokens=lex_file(args.source_file_path, source_file);
;
fclose(source_file);
return (DaybreakTokenifyResult){.error=0, .tokens=tokens};
}
}



int main(const int argc, ccstring* argv) {
{
daybreak_greeting();
const DaybreakArgParseResult parsed=daybreak_parse_args(argc, argv);
;
if (truthy(parsed.is_success))
{
return daybreak_compile(parsed.args);
}
else 
{
{
LOG_ERROR("Failed to parse CLI args");
return 1;
}
;
}
}
}

