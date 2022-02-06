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

#include <string>
#include <fstream>
#include <streambuf>

#include <shell.h>
#include <parser/compiler.h>
#include <parser/ast.h>

extern ast::Node * tscp_parse(const char * buffer);


TorusCompiler toruscompiler;


void TorusCompiler::add_file(std::string f) {
    _files.push(f);
}

void TorusCompiler::compile() {
    std::string current_file;
    while (!_files.empty()) {
        current_file = _files.front();
        _files.pop();
        TORUSSHELLECHO("Compiling " << current_file << "...");
        std::ifstream t(current_file);
        if (t.fail()) {
            TORUSSHELLECHO(current_file << " NOT FOUND!");
            continue;
        }
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        ast::Node * tree = tscp_parse(str.c_str());
        tree->generate();
        TORUSSHELLECHO("Compiling " << current_file << "... OK");
    }
}