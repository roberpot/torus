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
%type <node> block_resources block_obscene block_fame block_karma block_nototitles
%type <node> block_runes block_plevel
// Resources
%type <string_stack> list_strings_more list_ids_more list_files_more
%type <string_vector> list_strings list_ids list_files
%type <int_stack> list_integers_more
%type <int_vector> list_integers
///// Keywords
// Block class
%token FEOF RESOURCES OBSCENE FAME KARMA NOTOTITLES RUNES PLEVEL
%token <str> ID PATH STRING
%token <INTEGER> INTEGER

%%

/* AST root is 'file' */

file : file_blocks FEOF { $$ = $1; ast::root = $1; }

file_blocks : block file_blocks { $$ = new ast::BiNode($1, $2); }
            | { $$ = NULL; }

block : block_resources { $$ = $1; }
      | block_obscene { $$ = $1; }
      | block_fame { $$ = $1; }
      | block_karma { $$ = $1; }
      | block_nototitles { $$ = $1; }
      | block_runes { $$ = $1; }
      | block_plevel { $$ = $1; }

block_resources : '[' RESOURCES ']' list_files { $$ = new ast::BlockResourcesNode(*$4); delete $4; }

block_obscene : '[' OBSCENE ']' list_strings { $$ = new ast::BlockObsceneNode(*$4); delete $4; }

block_fame : '[' FAME ']' list_integers list_strings { $$ = new ast::BlockFameNode(*$4, *$5); }

block_karma : '[' KARMA ']' list_integers list_strings { $$ = new ast::BlockKarmaNode(*$4, *$5); }

block_nototitles : '[' NOTOTITLES ']' list_integers list_integers list_strings { $$ = new ast::BlockNototitlesNode(*$4, *$5, *$6); }

block_runes : '[' RUNES ']' list_strings { $$ = new ast::BlockRunesNode(*$4); }

block_plevel : '[' PLEVEL INTEGER ']' list_ids { $$ = new ast::BlockPlevelNode($3, *$5); delete $5; }

list_integers : INTEGER list_integers_more { $2->push($1); $$ = new ttl::vector<int>; (*$$) = stack_to_vector(*$2); delete $2; }

list_integers_more : ',' INTEGER list_integers_more { $3->push($2); $$ = $3; }
                   | { $$ = new ttl::dynamicstack<int>(); }

list_files : list_files_more { $$ = new ttl::vector<std::string>; (*$$) = stack_to_vector(*$1); delete $1; }

list_files_more : PATH list_files_more { $2->push(*$1); $$ = $2; }
                | { $$ = new ttl::dynamicstack<std::string>(); }

list_strings : list_strings_more { $$ = new ttl::vector<std::string>; (*$$) = stack_to_vector(*$1); delete $1; }

list_strings_more : STRING list_strings_more { $2->push(*$1); $$ = $2; }
                  |  { $$ = new ttl::dynamicstack<std::string>(); }

list_ids : list_ids_more { $$ = new ttl::vector<std::string>; (*$$) = stack_to_vector(*$1); delete $1; }

list_ids_more : ID list_ids_more { $2->push(*$1); $$ = $2; }
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
  std::cout << "YACC::error " << msj << std::endl;
//  SyntaxError e(msj);
//  throw e;
  return 1;
}

#ifdef _MSC_VER
#pragma warning(default:4702 4065 4244 4081)
#endif