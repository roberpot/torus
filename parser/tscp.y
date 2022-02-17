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
 ttl::dynamicstack<int> * int_stack;
 ttl::dynamicstack<std::string> * string_stack;
 ttl::vector<int> * int_vector;
 ttl::vector<std::string> * string_vector;
};

%type <node> file file_blocks block
// Blocks
%type <node> block_resources block_obscene    block_fame
%type <node> block_karma     block_nototitles block_runes
%type <node> block_plevel    block_defname    block_function
%type <node> block_typedefs
// Resources
%type <string_stack> list_strings_more list_ids_more list_files_more
%type <string_vector> list_strings list_ids list_files
%type <int_stack> list_integers_more
%type <int_vector> list_integers
///// Keywords
// Block class
%token RESOURCES_BLOCK OBSCENE_BLOCK    FAME_BLOCK
%token KARMA_BLOCK     NOTOTITLES_BLOCK RUNES_BLOCK
%token TYPEDEFS_BLOCK
%token FEOF_BLOCK
%token IF ELIF ELSE ENDIF
%token <INTEGER>       PLEVEL_BLOCK
%token <str>           DEFNAME_BLOCK    FUNCTION_BLOCK
%token <str> ID PATH STRING
%token <INTEGER> INTEGER

%%

/* AST root is 'file' */

file : file_blocks FEOF_BLOCK { $$ = $1; ast::root = $1; }

file_blocks : block file_blocks { $$ = new ast::BiNode($1, $2); }
            | { $$ = NULL; }

block : block_resources { $$ = $1; }
      | block_obscene { $$ = $1; }
      | block_fame { $$ = $1; }
      | block_karma { $$ = $1; }
      | block_nototitles { $$ = $1; }
      | block_runes { $$ = $1; }
      | block_plevel { $$ = $1; }
      | block_defname { $$ = $1; }
      | block_function { $$ = $1; }
      | block_typedefs { $$ = $1; }

block_resources : RESOURCES_BLOCK list_files { $$ = new ast::BlockResourcesNode(*$2); delete $2; }

block_obscene : OBSCENE_BLOCK list_strings { $$ = new ast::BlockObsceneNode(*$2); delete $2; }

block_fame : FAME_BLOCK list_integers list_strings { $$ = new ast::BlockFameNode(*$2, *$3); delete $2; delete $3; }

block_karma : KARMA_BLOCK list_integers list_strings { $$ = new ast::BlockKarmaNode(*$2, *$3); delete $2; delete $3; }

block_nototitles : NOTOTITLES_BLOCK list_integers list_integers list_strings { $$ = new ast::BlockNototitlesNode(*$2, *$3, *$4); delete $2; delete $3; delete $4; }

block_runes : RUNES_BLOCK list_strings { $$ = new ast::BlockRunesNode(*$2); delete $2; }

block_plevel : PLEVEL_BLOCK list_ids { $$ = new ast::BlockPlevelNode($1, *$2); delete $2; }

/*******************************************************************************************
* TYPEDEFS BLOCK
*******************************************************************************************/

block_typedefs : TYPEDEFS_BLOCK list_typedefs { $$ = nullptr; }

list_typedefs : typedefs_entries

typedefs_entries : typedefs_entry typedefs_entries
                 |

typedefs_entry : ID INTEGER

/*******************************************************************************************
* DEFNAME BLOCK
*******************************************************************************************/

block_defname : DEFNAME_BLOCK list_defnames { $$ = nullptr; }

list_defnames : list_defnames_more

list_defnames_more : defname_entry list_defnames_more
                   |

defname_entry : ID defname_expression

defname_expression : defname_or_expression
                   | STRING
                   | defname_random_expression

defname_random_expression : '{' defname_random_tuples '}'

defname_random_tuples : defname_random_tuple defname_random_tuples
                      | defname_random_tuple

defname_random_tuple : defname_random_lvalue INTEGER

defname_random_lvalue : INTEGER
                      | ID
                      | defname_random_expression

defname_or_expression : defname_terminal '|' defname_or_expression
                      | defname_terminal

defname_terminal : INTEGER
                 | ID

/*******************************************************************************************
* FUNCTION BLOCK
*******************************************************************************************/

block_function : FUNCTION_BLOCK function_definition { $$ = nullptr; }

function_definition : function_sentences

function_sentences : function_sentence function_sentences
                   |

function_sentence : conditional_sentence
//                  | loop_sentence
//                  | switch_sentence
//                  | return_sentence
//                  | function_call_sentence
                  | assignment_sentence

//// conditional_sentence

conditional_sentence : conditional_main_condition conditional_extra_conditions ENDIF

conditional_main_condition : IF '(' conditional_expression ')' function_sentences

conditional_extra_conditions : conditional_elseif_conditions conditional_else_condition
                             | conditional_elseif_conditions

conditional_elseif_conditions : conditional_elseif_condition conditional_elseif_conditions
                              |

conditional_elseif_condition : ELIF '(' conditional_expression ')' function_sentences
                             | ELSE IF '(' conditional_expression ')' function_sentences

conditional_else_condition : ELSE function_sentences

//// assignament_sentence

assignment_sentence : lvalue_expression '=' rvalue_expression

lvalue_expression :

rvalue_expression :




conditional_expression:

//loop_sentence :

//switch_sentence :

//return_sentence :

//function_call_sentence :

//assignment_sentence :

/*******************************************************************************************
* TODO: REFACTOR
*******************************************************************************************/

list_integers : INTEGER list_integers_more { $2->push($1); $$ = new ttl::vector<int>; (*$$) = stack_to_vector(*$2); delete $2; }

list_integers_more : ',' INTEGER list_integers_more { $3->push($2); $$ = $3; }
                   | { $$ = new ttl::dynamicstack<int>(); }

list_files : list_files_more { $$ = new ttl::vector<std::string>; (*$$) = stack_to_vector(*$1); delete $1; }

list_files_more : PATH list_files_more { $2->push(*$1); $$ = $2; delete $1; }
                | { $$ = new ttl::dynamicstack<std::string>(); }

list_strings : list_strings_more { $$ = new ttl::vector<std::string>; (*$$) = stack_to_vector(*$1); delete $1; }

list_strings_more : STRING list_strings_more { $2->push(*$1); $$ = $2; delete $1; }
                  |  { $$ = new ttl::dynamicstack<std::string>(); }

list_ids : list_ids_more { $$ = new ttl::vector<std::string>; (*$$) = stack_to_vector(*$1); delete $1; }

list_ids_more : ID list_ids_more { $2->push(*$1); $$ = $2; delete $1; }
              | { $$ = new ttl::dynamicstack<std::string>(); }


%%


extern void tscp_init_parser(const char * buffer);


ast::Node * tscp_parse(const char * buffer)
{
    tscp_init_parser(buffer);
    tscpparse();
    return ast::root;
}


int tscperror(char * msj) {
  throw tscplineno;
  //SyntaxError e(msj);
  //throw e;
  return 1;
}

#ifdef _MSC_VER
#pragma warning(default:4702 4065 4244 4081)
#endif