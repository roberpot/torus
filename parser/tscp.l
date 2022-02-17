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

NUM       [0-9]
LETTER    [a-zA-Z]
ALPHA     {LETTER}|_
ALPHANUM  {LETTER}|{NUM}|_
ID        {ALPHA}{ALPHANUM}*

%{
#ifdef _MSC_VER
#pragma warning(disable:4005)
#endif
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <debug_support/info.h>
#include <library/stack.h>
#include <library/vector.h>
#include <parser/ast.h>
#include <parser/tscp_parser.h>

std::string tmp;
char* tmpp;

int get_header_int(const char* buffer, const char* label) {
    const char* i = strstr(buffer, label) + strlen(label);
    while (*i == ' ') i++;
    return std::atoi(i);
}

std::string get_header_id(const char* buffer, const char* label) {
    char* tmpbuffer = new char[strlen(buffer)];
    const char* i = strstr(buffer, label) + strlen(label);
    while (*i == ' ') i++;
    strcpy(tmpbuffer, i);
    char* j = tmpbuffer + strlen(tmpbuffer) - 1;
    while (*j == ' ' || *j == ']') *j-- = '\000';
    std::string s(tmpbuffer);
    delete tmpbuffer;
    return s;
}

%}

%option yylineno
%option noyywrap
%option never-interactive

%x TSCP_BLOCK_COMMENT
%x TSCP_BLOCK_RESOURCES
%x TSCP_BLOCK_FUNCTION

%%

 /* Comments */

"//".*				;
"/*"				{ BEGIN(TSCP_BLOCK_COMMENT); }
<TSCP_BLOCK_COMMENT>"*/"	{ BEGIN(INITIAL); }
<TSCP_BLOCK_COMMENT>\n		;
<TSCP_BLOCK_COMMENT>.		;

 /* Block headers */
<*>\[[ ]*RESOURCES[ ]*\]           { BEGIN(TSCP_BLOCK_RESOURCES); return RESOURCES_BLOCK; }
<*>\[[ ]*OBSCENE[ ]*\]             { BEGIN(INITIAL); return OBSCENE_BLOCK; }
<*>\[[ ]*FAME[ ]*\]                { BEGIN(INITIAL); return FAME_BLOCK; }
<*>\[[ ]*KARMA[ ]*\]               { BEGIN(INITIAL); return KARMA_BLOCK; }
<*>\[[ ]*NOTOTITLES[ ]*\]          { BEGIN(INITIAL); return NOTOTITLES_BLOCK; }
<*>\[[ ]*RUNES[ ]*\]               { BEGIN(INITIAL); return RUNES_BLOCK; }
<*>\[[ ]*PLEVEL[ ]+{NUM}[ ]*\]     { BEGIN(INITIAL); tscplval.INTEGER = get_header_int(tscptext, "PLEVEL"); return PLEVEL_BLOCK; }
<*>\[[ ]*DEFNAME[ ]+{ID}[ ]*\]     { BEGIN(INITIAL); tscplval.str = new std::string(get_header_id(tscptext, "DEFNAME")); return DEFNAME_BLOCK; }
<*>\[[ ]*FUNCTION[ ]+{ID}[ ]*\]    { BEGIN(TSCP_BLOCK_FUNCTION); tscplval.str = new std::string(get_header_id(tscptext, "FUNCTION")); return FUNCTION_BLOCK; }
<*>\[[ ]*TYPEDEFS[ ]*\]               { BEGIN(INITIAL); return TYPEDEFS_BLOCK; }
<*>\[[ ]*EOF[ ]*\]                 { BEGIN(INITIAL); return FEOF_BLOCK; }

 /* RESOURCES BLOCK */

<TSCP_BLOCK_RESOURCES>\n   { if (tmp.size() > 0 ) { tscplval.str = new std::string(tmp); tmp.clear(); return PATH; }}
<TSCP_BLOCK_RESOURCES>[ \t\r]+ ;
<TSCP_BLOCK_RESOURCES>.   { tmp.push_back(*tscptext); }

<TSCP_BLOCK_FUNCTION>"if"         { return IF; }
<TSCP_BLOCK_FUNCTION>"elif"       { return ELIF; }
<TSCP_BLOCK_FUNCTION>"elseif"     { return ELIF; }
<TSCP_BLOCK_FUNCTION>"else"       { return ELSE; }
<TSCP_BLOCK_FUNCTION>"endif"      { return ENDIF; }


 /* Ignorables */
<INITIAL,TSCP_BLOCK_FUNCTION>[ \n\t\r]+			        ;
 /* Keywords */
 /*  */
{ID}                        { tscplval.str = new std::string(tscptext); return ID; }
\".*\"                      { tscplval.str = new std::string(tscptext); tscplval.str->pop_back(); tscplval.str->erase(0, 1); return STRING; }
-?[0-9]+	            	{ tscplval.INTEGER = atoi(tscptext); return INTEGER; }
0[0-9a-f]+                  { tscplval.INTEGER = strtol(tscptext, &tmpp, 16); return INTEGER; }

[{},\[\]|]                  return *tscptext;
.			                { /*LexicalError e("Carácter no reconocido."); throw e;*/ throw 1; }
%%

void tscp_init_parser(const char* buffer) {
 tscp_scan_string(buffer);
 tscplineno = 1;
}

UNREFERENCED_FUNCTION(yyunput);
#ifdef _MSC_VER
#pragma warning(default:4005)
#endif