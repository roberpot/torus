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

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]

%{

#include <iostream>
#include <cstdlib>
//#include "parser_errors.h"
#include "../debug/info.h"
#include "ast.h"
#include "tscp_parser.h"

std::string tmp;

%}

%option yylineno
%option noyywrap

%x TSCP_BLOCK_COMMENT
%x TSCP_BLOCK_RESOURCE_FILES

%%

 /* Comments */

<INITIAL,TSCP_BLOCK_RESOURCE_FILES>"//".*				;
"/*"				{ BEGIN(TSCP_BLOCK_COMMENT); }
<TSCP_BLOCK_COMMENT>"*/"	{ BEGIN(INITIAL); }
<TSCP_BLOCK_COMMENT>\n		;
<TSCP_BLOCK_COMMENT>.		;

 /* File path scanner. */

<TSCP_BLOCK_RESOURCE_FILES>\]  { return *tscptext; }
<TSCP_BLOCK_RESOURCE_FILES>\[  { BEGIN(INITIAL); unput('['); }
<TSCP_BLOCK_RESOURCE_FILES>\n   { if (tmp.size() > 0 ) { tscplval.str = new std::string(tmp); tmp.clear(); return PATH; }}
<TSCP_BLOCK_RESOURCE_FILES>[ \t\r]+ ;
<TSCP_BLOCK_RESOURCE_FILES>.   { tmp.push_back(*tscptext); }

 /* Ignorables */
[ \n\t\r]+			        ;

 /* Keywords */
"RESOURCES"                 { BEGIN(TSCP_BLOCK_RESOURCE_FILES); tscplval.str = new std::string; return RESOURCES; }
"OBSCENE"                   return OBSCENE;
"FAME"                      return FAME;
"[EOF]"                     return FEOF;
 /*  */
[a-zA-Z_][a-zA-Z_0-9]*      { tscplval.str = new std::string(tscptext); return ID; }
\".*\"                      { tscplval.str = new std::string(tscptext); tscplval.str->pop_back(); tscplval.str->erase(0, 1); return STRING; }
-?[0-9]+	            	{ tscplval.INTEGER = atoi(tscptext); return INTEGER; }

[{},\[\]]			return *tscptext;

%%

/*


.			{ LexicalError e("Carácter no reconocido."); throw e; }
*/


void tscp_init_parser(const char * buffer) {
 tscp_scan_string(buffer);
}

UNREFERENCED_FUNCTION(yyunput);