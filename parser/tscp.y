/*
 * This file is part of Torus.
 * Torus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Torus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with Torus. If not, see <http://www.gnu.org/licenses/>.
 */

 %{

#ifdef _MSC_VER
#pragma warning(disable:4702 4065 4244 4081)
#endif
#include <iostream>
#include <parser/ast/ast.h>
#include <library/stack.h>
#include <library/vector.h>
#include <library/utility.h>

#define YY_(x) (char *)(x)

extern int tscperror(char * msj);
extern int tscplex(void);
extern int tscplineno;
extern std::string tscp_current_filename;

%}

%union {
 ast::Node * node;
 std::string * str;
 int INTEGER;
 ttl::dynamicstack<std::string>* string_stack;
 ttl::vector<ast::Node*>* node_vector;
};

%type <node> file file_blocks block
// Blocks
%type <node> block_resources  block_defname   block_typedefs
%type <node> block_dialog     block_function  block_buttonevents
%type <node> block_book
// Resources
%type <string_stack> files_list
// Defname
%type <node> defname_list     defname_entry
// Arithmetic
%type <node> arithmetic_or            arithmetic_and          arithmetic_bitwise_or
%type <node> arithmetic_bitwise_xor   arithmetic_bitwise_and  arithmetic_eq
%type <node> arithmetic_rel           arithmetic_shift        arithmetic_add_sub
%type <node> arithmetic_mult_div_rem  arithmetic_logical_not  arithmetic_bitwise_not
%type <node> arithemtic_unary_minus   arithmetic_unary_increment_decrement
%type <node> arithmetic_value
%type <node> rvalue
%type <node> random_expression random_range
%type <node_vector> random_ranges
%type <node> lvalue  single_lvalue
%type <node> complex_lvalue  sequence_complex_lvalue
%type <node> insequence_complex_lvalue  final_complex_lvalue
%type <node> context_lvalue
///// Keywords
// Block class
%token RESOURCES_BLOCK  TYPEDEFS_BLOCK
%token EOF_BLOCK
%token DIALOG_LOCATION      DIALOG_PAGE          DIALOG_RESIZEPIC
%token DIALOG_GUMPPICTILED  DIALOG_CHECKERTRANS  DIALOG_BUTTON
%token DIALOG_DTEXT         DIALOG_CHECKBOX
%token ON_BUTTON  ON_BUTTONS
%token LOCAL  VAR  TAG   CTAG
%token SRC    ACT  ARGV  SELF
%token SERV   ACTARGV
%token LESS_OR_EQUAL GREATER_OR_EQUAL EQUAL EXACT NOT_EXACT NOT_EQUAL AND OR
%token SUM_ASIG SUB_ASIG MUL_ASIG DIV_ASIG REM_ASIG AND_ASIG XOR_ASIG OR_ASIG
%token SHIFT_LEFT SHIFT_RIGHT UNARY_INCREMENT UNARY_DECREMENT
%token IF ELSE ELIF FOR RETURN
%token ARRAY_CLEAR ARRAY_PUSH ARRAY_INSERT ARRAY_REMOVE ARRAY_LENGTH
%token BOOK_PAGES BOOK_TITLE BOOK_AUTHOR BOOK_PAGE
%token <str> DEFNAME_BLOCK  DIALOG_BLOCK  FUNCTION_BLOCK BUTTONEVENTS_BLOCK
%token <str> BOOK_BLOCK
%token <str> ID PATH STRING
%token <INTEGER> INTEGER

%%

/* AST root is 'file' */

file : file_blocks EOF_BLOCK { $$ = $1; ast::root = $1; }

file_blocks : block file_blocks { $$ = new ast::BiNode($1, $2); }
            | { $$ = NULL; }

block : block_resources    { $$ = $1; }
      | block_defname      { $$ = $1; }
      | block_typedefs     { $$ = $1; }
      | block_dialog       { $$ = $1; }
      | block_function     { $$ = $1; }
      | block_buttonevents { $$ = $1; }
      | block_book         { $$ = $1; }

/*******************************************************************************************
* RESOURCES BLOCK
*******************************************************************************************/

block_resources : RESOURCES_BLOCK files_list { $$ = new ast::BlockResourcesNode(*$2); delete $2; }

files_list : PATH files_list { $$ = $2; $$->push(*$1); delete $1; }
           |                 { $$ = new ttl::dynamicstack<std::string>(); }

/*******************************************************************************************
* DEFNAME BLOCK
*******************************************************************************************/

block_defname : DEFNAME_BLOCK defname_list { $$ = new ast::DefnameBlockNode(*$1, tscp_current_filename, tscplineno, $2); delete $1; }

defname_list : defname_entry defname_list { $$ = new ast::BiNode($1, $2); }
             |                            { $$ = nullptr; }

defname_entry : ID '=' arithmetic_or ';'                 { $$ = new ast::DefnameEntryNode(*$1, tscp_current_filename, tscplineno, $3); delete $1; }
              | ID '[' INTEGER ']' '=' arithmetic_or ';' { $$ = new ast::DefnameArrayEntryNode(*$1, tscp_current_filename, tscplineno, $3, $6); delete $1; }

/*******************************************************************************************
* TYPEDEF BLOCK
*******************************************************************************************/

block_typedefs : TYPEDEFS_BLOCK defname_list { $$ = nullptr; }

/*******************************************************************************************
* BOOK BLOCK
*******************************************************************************************/

block_book : BOOK_BLOCK book_statements { $$ = nullptr; }

book_statements : book_statements book_statement
                |

book_statement : BOOK_PAGES '=' INTEGER ';'
               | BOOK_TITLE '=' STRING ';'
               | BOOK_AUTHOR '=' STRING ';'
               | BOOK_PAGE INTEGER '=' '{' book_lines '}'

book_lines : book_lines STRING
           |

/*******************************************************************************************
* DIALOG BLOCK
*******************************************************************************************/

block_dialog : DIALOG_BLOCK statement_sequence { $$ = nullptr; }

/*******************************************************************************************
* BUTTONEVENTS BLOCK
*******************************************************************************************/

block_buttonevents : BUTTONEVENTS_BLOCK buttonevents { $$ = nullptr; }

buttonevents : buttonevent buttonevents
	     | buttonevent

buttonevent : ON_BUTTON '(' INTEGER ')' '{' statement_sequence '}'
            | ON_BUTTONS '(' INTEGER ',' INTEGER ')' '{' statement_sequence '}'

/*******************************************************************************************
* FUNCTION BLOCK
*******************************************************************************************/

block_function : FUNCTION_BLOCK statement_sequence { $$ = nullptr; }

statement_sequence : statement_sequence statement
                   | statement

statement : DIALOG_LOCATION '(' INTEGER ',' INTEGER ')' ';'
          | DIALOG_PAGE '(' arithmetic_or ')' ';'
          | DIALOG_RESIZEPIC '(' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ')' ';'
          | DIALOG_GUMPPICTILED '(' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ')' ';'
          | DIALOG_CHECKERTRANS '(' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ')' ';'
          | DIALOG_BUTTON '(' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ')' ';'
          | DIALOG_CHECKBOX '(' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ')' ';'
          | DIALOG_DTEXT '(' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ')' ';'
          | statement_array_operation
          | statement_asig
          | statement_functioncall
          | statement_conditional
          | statement_loop
          | statement_return

statement_array_operation : ARRAY_CLEAR '(' lvalue ')' ';'
                          | ARRAY_PUSH '(' lvalue ',' arithmetic_or ')' ';'
                          | ARRAY_INSERT '(' lvalue ',' arithmetic_or ',' arithmetic_or ')' ';'
                          | ARRAY_REMOVE '(' lvalue ',' arithmetic_or ')' ';'
                          | ARRAY_LENGTH '(' lvalue ')' ';'

statement_conditional : IF '(' arithmetic_or ')' '{' statement_sequence '}' statement_conditional_if_extra

statement_conditional_if_extra : statement_conditional_if_extra statement_conditional_if_end
                               | statement_conditional_if_extra ELIF '(' arithmetic_or ')' '{' statement_sequence '}'
                               |

statement_conditional_if_end : ELSE '{' statement_sequence '}'

statement_loop : FOR '(' asig ';' arithmetic_or ';' asig ')' '{' statement_sequence '}'

statement_return : RETURN ';'
                 | RETURN arithmetic_or ';'

/*******************************************************************************************
* COMMON
*******************************************************************************************/

statement_asig : asig ';'

asig : lvalue asig_op arithmetic_or
     | lvalue UNARY_INCREMENT
     | lvalue UNARY_DECREMENT

asig_op : '='
        | SUM_ASIG
        | SUB_ASIG
        | MUL_ASIG
        | DIV_ASIG
        | REM_ASIG
        | AND_ASIG
        | XOR_ASIG
        | OR_ASIG

arithmetic_or : arithmetic_or OR arithmetic_and  { $$ = nullptr; }
              | arithmetic_and                   { $$ = $1; }

arithmetic_and : arithmetic_and AND arithmetic_bitwise_or  { $$ = nullptr; }
               | arithmetic_bitwise_or                     { $$ = $1; }

arithmetic_bitwise_or : arithmetic_bitwise_or '|' arithmetic_bitwise_xor  { $$ = new ast::BitwiseOrNode($1, $3); }
                     | arithmetic_bitwise_xor                             { $$ = $1; }

arithmetic_bitwise_xor : arithmetic_bitwise_xor '^' arithmetic_bitwise_and  { $$ = nullptr; }
                       | arithmetic_bitwise_and                             { $$ = $1; }

arithmetic_bitwise_and : arithmetic_bitwise_and '&' arithmetic_eq  { $$ = nullptr; }
                       | arithmetic_eq                             { $$ = $1; }

arithmetic_eq : arithmetic_eq EQUAL arithmetic_rel      { $$ = nullptr; }
              | arithmetic_eq NOT_EQUAL arithmetic_rel  { $$ = nullptr; }
              | arithmetic_eq EXACT arithmetic_rel      { $$ = nullptr; }
              | arithmetic_eq NOT_EXACT arithmetic_rel  { $$ = nullptr; }
              | arithmetic_rel                          { $$ = $1; }

arithmetic_rel : arithmetic_rel '<' arithmetic_shift               { $$ = nullptr; }
               | arithmetic_rel LESS_OR_EQUAL arithmetic_shift     { $$ = nullptr; }
               | arithmetic_rel '>' arithmetic_shift               { $$ = nullptr; }
               | arithmetic_rel GREATER_OR_EQUAL arithmetic_shift  { $$ = nullptr; }
               | arithmetic_shift                                  { $$ = $1; }

arithmetic_shift : arithmetic_shift SHIFT_LEFT arithmetic_add_sub   { $$ = nullptr; }
                 | arithmetic_shift SHIFT_RIGHT arithmetic_add_sub  { $$ = nullptr; }
                 | arithmetic_add_sub                               { $$ = $1; }

arithmetic_add_sub : arithmetic_add_sub '+' arithmetic_mult_div_rem  { $$ = nullptr; }
                   | arithmetic_add_sub '-' arithmetic_mult_div_rem  { $$ = nullptr; }
                   | arithmetic_mult_div_rem                         { $$ = $1; }

arithmetic_mult_div_rem : arithmetic_mult_div_rem '/' arithmetic_logical_not  { $$ = nullptr; }
                        | arithmetic_mult_div_rem '*' arithmetic_logical_not  { $$ = nullptr; }
                        | arithmetic_mult_div_rem '%' arithmetic_logical_not  { $$ = nullptr; }
                        | arithmetic_logical_not                              { $$ = $1; }

arithmetic_logical_not : '!' arithmetic_logical_not  { $$ = nullptr; }
                       | arithmetic_bitwise_not      { $$ = $1; }

arithmetic_bitwise_not : '~' arithmetic_bitwise_not  { $$ = nullptr; }
                       | arithemtic_unary_minus      { $$ = $1; }

arithemtic_unary_minus : '-' arithemtic_unary_minus            { $$ = nullptr; }
                       | arithmetic_unary_increment_decrement  { $$ = $1; }

arithmetic_unary_increment_decrement : arithmetic_unary_increment_decrement UNARY_INCREMENT  { $$ = nullptr; }
                                     | arithmetic_unary_increment_decrement UNARY_DECREMENT  { $$ = nullptr; }
                                     | arithmetic_value                                      { $$ = $1; }

arithmetic_value : '(' arithmetic_or ')'                                      { $$ = $2; }
                 | '(' arithmetic_or '?' arithmetic_or ':' arithmetic_or ')'  { $$ = nullptr; }
                 | rvalue                                                     { $$ = $1; }

rvalue : lvalue             { $$ = $1; }
       | random_expression  { $$ = $1; }
       | INTEGER            { $$ = new ast::IntegerValueNode($1); }
       | STRING             { $$ = new ast::StringValueNode(*$1); delete $1; }

lvalue : single_lvalue   { $$ = $1; }
       | complex_lvalue  { $$ = $1; }

single_lvalue : SRC                            { $$ = nullptr; }
              | SELF                           { $$ = nullptr; }
              | SERV                           { $$ = nullptr; }
              | ACT                            { $$ = nullptr; }
              | ACTARGV '[' arithmetic_or ']'  { $$ = nullptr; }
              | ARGV '[' arithmetic_or ']'     { $$ = nullptr; }

complex_lvalue : final_complex_lvalue                              { $$ = $1; }
               | sequence_complex_lvalue '.' final_complex_lvalue  { $$ = nullptr; }

sequence_complex_lvalue : sequence_complex_lvalue '.' insequence_complex_lvalue  { $$ = nullptr; }
                        | insequence_complex_lvalue                              { $$ = $1; }
                        | single_lvalue                                          { $$ = $1; }
                        | context_lvalue                                         { $$ = $1; }

insequence_complex_lvalue : final_complex_lvalue  { $$ = nullptr; }
                          | TAG                   { $$ = nullptr; }
                          | CTAG                  { $$ = nullptr; }

final_complex_lvalue : ID                        { $$ = new ast::IdentifierNode(*$1, tscp_current_filename, tscplineno); delete $1; }
                     | ID '(' argument_list ')'  { $$ = nullptr; }
                     | ID '[' arithmetic_or ']'  { $$ = new ast::ArrayAccessNode(*$1, tscp_current_filename, tscplineno, $3); delete $1; }

context_lvalue : LOCAL  { $$ = nullptr; }
               | VAR    { $$ = nullptr; }

random_expression : '{' random_ranges '}'   { $$ = new ast::RandomExpressionNode(*$2); delete $2; }

random_ranges : random_ranges random_range  { $$ = $1; $$->push_back($2); }
              | random_range                { $$ = new ttl::vector<ast::Node*>(); $$->push_back($1); }

random_range : rvalue INTEGER  { $$ = new ast::SingleRandomRangeNode($2, $1); }

statement_functioncall : lvalue ';'

argument_list : populated_argument_list
              |

populated_argument_list : populated_argument_list ',' arithmetic_or
                        | arithmetic_or

%%


extern void tscp_init_parser(const char * buffer);

extern std::string tscp_current_filename;

ast::Node * tscp_parse(const char * buffer, const std::string& filename)
{
    tscp_current_filename = filename;
    tscp_init_parser(buffer);
    tscpparse();
    return ast::root;
}


int tscperror(char * msj) {
  throw ttl::SyntaxError(tscplineno);
  return 1;
}

#ifdef _MSC_VER
#pragma warning(default:4702 4065 4244 4081)
#endif