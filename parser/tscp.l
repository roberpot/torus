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
#include <parser/ast/ast.h>
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

%s TSCP_BLOCK_RESOURCES
%x TSCP_BLOCK_DEFNAME
%x TSCP_BLOCK_TYPEDEFS
%x TSCP_BLOCK_FUNCTION
%x TSCP_BLOCK_BOOK

%%

 /* COMMENTS */

<*>"//".*                               ;
<*>[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]  ;

 /* BLOCK HEADERS */

<*>\[[ ]*RESOURCES[ ]*\]             { BEGIN(TSCP_BLOCK_RESOURCES); return RESOURCES_BLOCK; }
<*>\[[ ]*DEFNAME[ ]+{ID}[ ]*\]       { BEGIN(TSCP_BLOCK_DEFNAME); tscplval.str = new std::string(get_header_id(tscptext, "DEFNAME")); return DEFNAME_BLOCK; }
<*>\[[ ]*DIALOG[ ]+{ID}[ ]*\]        { BEGIN(TSCP_BLOCK_FUNCTION); tscplval.str = new std::string(get_header_id(tscptext, "DIALOG")); return DIALOG_BLOCK; }
<*>\[[ ]*FUNCTION[ ]+{ID}[ ]*\]      { BEGIN(TSCP_BLOCK_FUNCTION); tscplval.str = new std::string(get_header_id(tscptext, "FUNCTION")); return FUNCTION_BLOCK; }
<*>\[[ ]*BUTTONEVENTS[ ]+{ID}[ ]*\]  { BEGIN(TSCP_BLOCK_FUNCTION); tscplval.str = new std::string(get_header_id(tscptext, "BUTTONEVENTS")); return BUTTONEVENTS_BLOCK; }
<*>\[[ ]*TYPEDEFS[ ]*\]              { BEGIN(TSCP_BLOCK_TYPEDEFS); return TYPEDEFS_BLOCK; }
<*>\[[ ]*BOOK[ ]+{ID}[ ]*\]          { BEGIN(TSCP_BLOCK_BOOK); tscplval.str = new std::string(get_header_id(tscptext, "BOOK")); return BOOK_BLOCK; }
<*>\[[ ]*EOF[ ]*\]                   { BEGIN(INITIAL); return EOF_BLOCK; }

 /* BLOCK RESOURCES */

<TSCP_BLOCK_RESOURCES>\n       { if (tmp.size() > 0 ) { tscplval.str = new std::string(tmp); tmp.clear(); return PATH; }}
<TSCP_BLOCK_RESOURCES>[ \t\r]+ ;
<TSCP_BLOCK_RESOURCES>.        { tmp.push_back(*tscptext); }

 /* BLOCK DEFNAME */

<TSCP_BLOCK_DEFNAME>{ID}           { tscplval.str = new std::string(tscptext); return ID; }
<TSCP_BLOCK_DEFNAME>[=|;\{\}\[\]]  { return *tscptext; }
<TSCP_BLOCK_DEFNAME>[ \n\t\r]+     ;
<TSCP_BLOCK_DEFNAME>-?[0-9]+       { tscplval.INTEGER = atoi(tscptext); return INTEGER; }
<TSCP_BLOCK_DEFNAME>0[0-9a-f]+     { tscplval.INTEGER = strtol(tscptext, &tmpp, 16); return INTEGER; }
<TSCP_BLOCK_DEFNAME>\".*\"         { tscplval.str = new std::string(tscptext); tscplval.str->pop_back(); tscplval.str->erase(0, 1); return STRING; }

 /* BLOCK TYPEDEFS */

<TSCP_BLOCK_TYPEDEFS>{ID}         { tscplval.str = new std::string(tscptext); return ID; }
<TSCP_BLOCK_TYPEDEFS>[=;]         { return *tscptext; }
<TSCP_BLOCK_TYPEDEFS>[ \n\t\r]+   ;
<TSCP_BLOCK_TYPEDEFS>-?[0-9]+     { tscplval.INTEGER = atoi(tscptext); return INTEGER; }
<TSCP_BLOCK_TYPEDEFS>0[0-9a-f]+   { tscplval.INTEGER = strtol(tscptext, &tmpp, 16); return INTEGER; }

 /* BLOCK BOOK */
<TSCP_BLOCK_BOOK>[ \n\t\r]+                          ;
<TSCP_BLOCK_BOOK>"pages"                             { return BOOK_PAGES; }
<TSCP_BLOCK_BOOK>"title"                             { return BOOK_TITLE; }
<TSCP_BLOCK_BOOK>"author"                            { return BOOK_AUTHOR; }
<TSCP_BLOCK_BOOK>"page"                              { return BOOK_PAGE; }
<TSCP_BLOCK_BOOK>\"[^\"]*\"                          { tscplval.str = new std::string(tscptext); tscplval.str->pop_back(); tscplval.str->erase(0, 1); return STRING; }
<TSCP_BLOCK_BOOK>[;=\{\}]                            { return *tscptext; }
<TSCP_BLOCK_BOOK>[0-9]+                              { tscplval.INTEGER = atoi(tscptext); return INTEGER; }
<TSCP_BLOCK_BOOK>0[0-9a-f]+                          { tscplval.INTEGER = strtol(tscptext, &tmpp, 16); return INTEGER; }

 /* BLOCK FUNCTION */

<TSCP_BLOCK_FUNCTION>[ \n\t\r]+                          ;
<TSCP_BLOCK_FUNCTION>"location"                          { return DIALOG_LOCATION; }
<TSCP_BLOCK_FUNCTION>"page"                              { return DIALOG_PAGE; }
<TSCP_BLOCK_FUNCTION>"resizepic"                         { return DIALOG_RESIZEPIC; }
<TSCP_BLOCK_FUNCTION>"gumppictiled"                      { return DIALOG_GUMPPICTILED; }
<TSCP_BLOCK_FUNCTION>"checkertrans"                      { return DIALOG_CHECKERTRANS; }
<TSCP_BLOCK_FUNCTION>"button"                            { return DIALOG_BUTTON; }
<TSCP_BLOCK_FUNCTION>"checkbox"                          { return DIALOG_CHECKBOX; }
<TSCP_BLOCK_FUNCTION>"dtext"                             { return DIALOG_DTEXT; }
<TSCP_BLOCK_FUNCTION>"if"                                { return IF; }
<TSCP_BLOCK_FUNCTION>"elseif"                            { return ELIF; }
<TSCP_BLOCK_FUNCTION>"elif"                              { return ELIF; }
<TSCP_BLOCK_FUNCTION>"else"                              { return ELSE; }
<TSCP_BLOCK_FUNCTION>"for"                               { return FOR; }
<TSCP_BLOCK_FUNCTION>"return"                            { return RETURN; }
<TSCP_BLOCK_FUNCTION>"!=="                               { return NOT_EXACT; }
<TSCP_BLOCK_FUNCTION>"==="                               { return EXACT; }
<TSCP_BLOCK_FUNCTION>"=="                                { return EQUAL; }
<TSCP_BLOCK_FUNCTION>"!="                                { return NOT_EQUAL; }
<TSCP_BLOCK_FUNCTION>"<="                                { return LESS_OR_EQUAL; }
<TSCP_BLOCK_FUNCTION>">="                                { return GREATER_OR_EQUAL; }
<TSCP_BLOCK_FUNCTION>"&&"                                { return AND; }
<TSCP_BLOCK_FUNCTION>"||"                                { return OR; }
<TSCP_BLOCK_FUNCTION>"+="                                { return SUM_ASIG; }
<TSCP_BLOCK_FUNCTION>"-="                                { return SUB_ASIG; }
<TSCP_BLOCK_FUNCTION>"*="                                { return MUL_ASIG; }
<TSCP_BLOCK_FUNCTION>"/="                                { return DIV_ASIG; }
<TSCP_BLOCK_FUNCTION>"%="                                { return REM_ASIG; }
<TSCP_BLOCK_FUNCTION>"&="                                { return AND_ASIG; }
<TSCP_BLOCK_FUNCTION>"^="                                { return XOR_ASIG; }
<TSCP_BLOCK_FUNCTION>"|="                                { return OR_ASIG; }
<TSCP_BLOCK_FUNCTION>"++"                                { return UNARY_INCREMENT; }
<TSCP_BLOCK_FUNCTION>"--"                                { return UNARY_DECREMENT; }
<TSCP_BLOCK_FUNCTION>"<<"                                { return SHIFT_LEFT; }
<TSCP_BLOCK_FUNCTION>\"[^\"]*\"                          { tscplval.str = new std::string(tscptext); tscplval.str->pop_back(); tscplval.str->erase(0, 1); return STRING; }
<TSCP_BLOCK_FUNCTION>"TAG"                               { return TAG; }
<TSCP_BLOCK_FUNCTION>"CTAG"                              { return CTAG; }
<TSCP_BLOCK_FUNCTION>"LOCAL"                             { return LOCAL; }
<TSCP_BLOCK_FUNCTION>"VAR"                               { return VAR; }
<TSCP_BLOCK_FUNCTION>"SRC"                               { return SRC; }
<TSCP_BLOCK_FUNCTION>"ACT"                               { return ACT; }
<TSCP_BLOCK_FUNCTION>"ARGV"                              { return ARGV; }
<TSCP_BLOCK_FUNCTION>"SELF"                              { return SELF; }
<TSCP_BLOCK_FUNCTION>"ARRAY_CLEAR"                       { return ARRAY_CLEAR; }
<TSCP_BLOCK_FUNCTION>"ARRAY_PUSH"                        { return ARRAY_PUSH; }
<TSCP_BLOCK_FUNCTION>"ARRAY_INSERT"                      { return ARRAY_INSERT; }
<TSCP_BLOCK_FUNCTION>"ARRAY_REMOVE"                      { return ARRAY_REMOVE; }
<TSCP_BLOCK_FUNCTION>"ARRAY_LENGTH"                      { return ARRAY_LENGTH; }
<TSCP_BLOCK_FUNCTION>"ON_BUTTON"                         { return ON_BUTTON; }
<TSCP_BLOCK_FUNCTION>"ON_BUTTONS"                        { return ON_BUTTONS; }
<TSCP_BLOCK_FUNCTION>[,();\:\.=<>\{\}+\-*/!&\[\]\?~]     { return *tscptext; }
<TSCP_BLOCK_FUNCTION>-?[0-9]+                            { tscplval.INTEGER = atoi(tscptext); return INTEGER; }
<TSCP_BLOCK_FUNCTION>0[0-9a-f]+                          { tscplval.INTEGER = strtol(tscptext, &tmpp, 16); return INTEGER; }
<TSCP_BLOCK_FUNCTION>{ID}                                { tscplval.str = new std::string(tscptext); return ID; }

 /* Glyphs */
[=;]                   { return *tscptext; }

 /* Ignorables */
<INITIAL>[ \n\t\r]+    ;
<*>.                   { throw ttl::LexicalError(*tscptext, tscplineno); }
%%

void tscp_init_parser(const char* buffer) {
 tscp_scan_string(buffer);
 tscplineno = 1;
}


/*
{ID}                        { tscplval.str = new std::string(tscptext); return ID; }
\".*\"                      { tscplval.str = new std::string(tscptext); tscplval.str->pop_back(); tscplval.str->erase(0, 1); return STRING; }
-?[0-9]+	            	{ tscplval.INTEGER = atoi(tscptext); return INTEGER; }
0[0-9a-f]+                  { tscplval.INTEGER = strtol(tscptext, &tmpp, 16); return INTEGER; }

*/
UNREFERENCED_FUNCTION(yyunput);
#ifdef _MSC_VER
#pragma warning(default:4005)
#endif