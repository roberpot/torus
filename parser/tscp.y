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

#include <iostream>
#include "ast.h"
//#include "parser_errors.h"

#define YY_(x) (char *)(x)

extern int tscperror(char * msj);
extern int tscplex(void);
extern int tscplineno;

%}

%union {
 Node * node;
 std::string * str;
 int INTEGER;
};

%type <node> file file_blocks block
// Blocks
%type <node> block_resources block_obscene block_fame
// Resources
%type <node> list_files list_strings list_integers list_integers_more
%token FEOF RESOURCES OBSCENE FAME
%token <str> ID PATH STRING
%token <INTEGER> INTEGER

%%

/* AST root is 'file' */

file : file_blocks FEOF { $$ = NULL; }

file_blocks : block file_blocks { $$ = NULL; }
            | { $$ = NULL; }

block : block_resources { $$ = NULL; }
      | block_obscene { $$ = NULL; }
      | block_fame { $$ = NULL; }

block_resources : '[' RESOURCES ']' list_files { std::cout << "block_resources" << std::endl; $$ = NULL; }

block_obscene : '[' OBSCENE ']' list_strings { std::cout << "block_obscene" << std::endl; $$ = NULL; }

block_fame : '[' FAME ']' list_integers list_strings { std::cout << "block_fame" << std::endl; $$ = NULL; }

list_integers : INTEGER list_integers_more { std::cout << $1 << std::endl; $$ = NULL; }

list_integers_more : ',' INTEGER list_integers_more { std::cout << $2 << std::endl; $$ = NULL; }
                   | { $$ = NULL; }

list_files : PATH list_files { std::cout << "FILE PATH: " << *$1 << std::endl; $$ = NULL; }
           | { $$ = NULL; }

list_strings : STRING list_strings { std::cout << *$1 << std::endl; $$ = NULL; }
             |  { $$ = NULL; }

%%
/*
%type <node> body_file body_format integer_list
%token <INTEGER> INTEGER

body_file : body_format body_file { $$ = new ultimaonline::body::BodyFileNode($1, $2); }
     | { $$ = NULL; }

body_format : INTEGER '{' integer_list '}' INTEGER { $$ = new ultimaonline::body::BodyFormatNode($1, $3, $5); }

integer_list : INTEGER ',' integer_list { $$ = new ultimaonline::body::IntegerListNode($1, $3); }
             | INTEGER { $$ = new ultimaonline::body::IntegerListNode($1, NULL); }
*/

extern void tscp_init_parser(const char * buffer);


void tscp_parse(const char * buffer)
{
    //if (ultimaonline::body::root_node) delete ultimaonline::body::root_node;
    tscp_init_parser(buffer);
    tscpparse();
    //ultimaonline::body::root_node->generate();
    //delete ultimaonline::body::root_node;
}


int tscperror(char * msj) {
  std::cout << "YACC::error " << msj << std::endl;
//  SyntaxError e(msj);
//  throw e;
  return 1;
}
