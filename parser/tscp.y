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
#include <parser/ast.h>
#include <library/stack.h>
#include <library/vector.h>
#include <library/utility.h>

#define YY_(x) (char *)(x)

extern int tscperror(char * msj);
extern int tscplex(void);
extern int tscplineno;

%}

%union {
 ast::Node * node;
 std::string * str;
 int INTEGER;
 ttl::dynamicstack<std::string>* string_stack;
};

%type <node> file file_blocks block
// Blocks
%type <node> block_resources  block_defname   block_typedefs
%type <node> block_dialog     block_function  block_buttonevents
// Resources
%type <string_stack> files_list
///// Keywords
// Block class
%token RESOURCES_BLOCK  TYPEDEFS_BLOCK
%token EOF_BLOCK
%token DIALOG_LOCATION      DIALOG_PAGE          DIALOG_RESIZEPIC
%token DIALOG_GUMPPICTILED  DIALOG_CHECKERTRANS  DIALOG_BUTTON
%token DIALOG_DTEXT
%token ON_BUTTON  ON_BUTTONS
%token LOCAL  VAR  TAG   TAG0  CTAG  CTAG0
%token SRC    ACT  ARGV  SELF
%token LESS_OR_EQUAL GREATER_OR_EQUAL EQUAL NOT_EQUAL AND OR
%token SUM_ASIG SUB_ASIG MUL_ASIG DIV_ASIG REM_ASIG AND_ASIG XOR_ASIG OR_ASIG
%token SHIFT_LEFT SHIFT_RIGHT UNARY_INCREMENT UNARY_DECREMENT
%token IF ELSE ELIF FOR RETURN
%token <str> DEFNAME_BLOCK  DIALOG_BLOCK  FUNCTION_BLOCK BUTTONEVENTS_BLOCK
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

/*******************************************************************************************
* RESOURCES BLOCK
*******************************************************************************************/

block_resources : RESOURCES_BLOCK files_list { $$ = new ast::BlockResourcesNode(*$2); delete $2; }

files_list : PATH files_list { $$ = $2; $$->push(*$1); delete $1; }
           |                 { $$ = new ttl::dynamicstack<std::string>(); }

/*******************************************************************************************
* DEFNAME BLOCK
*******************************************************************************************/

block_defname : DEFNAME_BLOCK defname_list { $$ = nullptr; }

defname_list : ID '=' arithmetic_or ';' defname_list
             | ID '[' INTEGER ']' '=' arithmetic_or ';' defname_list
             |

/*******************************************************************************************
* TYPEDEF BLOCK
*******************************************************************************************/

block_typedefs : TYPEDEFS_BLOCK defname_list { $$ = nullptr; }

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
          | DIALOG_PAGE '(' INTEGER ')' ';'
          | DIALOG_RESIZEPIC '(' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ')' ';'
          | DIALOG_GUMPPICTILED '(' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ')' ';'
          | DIALOG_CHECKERTRANS '(' INTEGER ',' INTEGER ',' INTEGER ',' INTEGER ')' ';'
          | DIALOG_BUTTON '(' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ')' ';'
          | DIALOG_DTEXT '(' arithmetic_or ',' arithmetic_or ',' arithmetic_or ',' arithmetic_or ')' ';'
          | statement_asig
          | statement_functioncall
          | statement_conditional
          | statement_loop
          | statement_return

statement_conditional : IF '(' arithmetic_or ')' '{' statement_sequence '}' statement_conditional_if_extra

statement_conditional_if_extra : statement_conditional_if_extra statement_conditional_if_end
                               | ELIF '(' arithmetic_or ')' '{' statement_sequence '}'
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

arithmetic_or : arithmetic_or OR arithmetic_and
              | arithmetic_and

arithmetic_and : arithmetic_and AND arithmetic_bitwise_or
               | arithmetic_bitwise_or

arithmetic_bitwise_or : arithmetic_bitwise_or '|' arithmetic_bitwise_xor
                     | arithmetic_bitwise_xor

arithmetic_bitwise_xor : arithmetic_bitwise_xor '^' arithmetic_bitwise_and
                       | arithmetic_bitwise_and

arithmetic_bitwise_and : arithmetic_bitwise_and '&' arithmetic_eq
                       | arithmetic_eq

arithmetic_eq : arithmetic_eq EQUAL arithmetic_rel
              | arithmetic_eq NOT_EQUAL arithmetic_rel
              | arithmetic_rel

arithmetic_rel : arithmetic_rel '<' arithmetic_shift
               | arithmetic_rel LESS_OR_EQUAL arithmetic_shift
               | arithmetic_rel '>' arithmetic_shift
               | arithmetic_rel GREATER_OR_EQUAL arithmetic_shift
               | arithmetic_shift

arithmetic_shift : arithmetic_shift SHIFT_LEFT arithmetic_add_sub
                 | arithmetic_shift SHIFT_RIGHT arithmetic_add_sub
                 | arithmetic_add_sub

arithmetic_add_sub : arithmetic_add_sub '+' arithmetic_mult_div_rem
                   | arithmetic_add_sub '-' arithmetic_mult_div_rem
                   | arithmetic_mult_div_rem

arithmetic_mult_div_rem : arithmetic_mult_div_rem '/' arithmetic_logical_not
                        | arithmetic_mult_div_rem '*' arithmetic_logical_not
                        | arithmetic_mult_div_rem '%' arithmetic_logical_not
                        | arithmetic_logical_not

arithmetic_logical_not : '!' arithmetic_logical_not
                       | arithmetic_bitwise_not

arithmetic_bitwise_not : '~' arithmetic_bitwise_not
                       | arithemtic_unary_minus

arithemtic_unary_minus : '-' arithemtic_unary_minus
                       | arithmetic_unary_increment_decrement

arithmetic_unary_increment_decrement : arithmetic_unary_increment_decrement UNARY_INCREMENT
                                     | arithmetic_unary_increment_decrement UNARY_DECREMENT
                                     | arithmetic_value

arithmetic_value : '(' arithmetic_or ')'
                 | rvalue

rvalue : lvalue
       | random_expression
       | INTEGER
       | STRING

lvalue : lvalue '.' alvalue
       | lvalue '.' ID '(' argument_list ')'
       | final_lvalue

alvalue : TAG
        | TAG0
        | CTAG
        | CTAG0
        | ID
        | ID '[' arithmetic_or ']'

final_lvalue : LOCAL
             | VAR
             | SRC
             | ACT
             | ARGV '[' arithmetic_mult_div_rem ']'
             | SELF
             | alvalue

random_expression : '{' random_ranges '}'

random_ranges : random_ranges random_range
              | random_range

random_range : rvalue INTEGER

statement_functioncall : lvalue ';'
                       | ID '(' argument_list ')' ';'

argument_list : argument_list rvalue
              |

%%


extern void tscp_init_parser(const char * buffer);


ast::Node * tscp_parse(const char * buffer)
{
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