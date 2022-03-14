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
#include <parser/ast/ast.h>
#include <parser/ast/ast_garbage_collector.h>

extern ast::Node * tscp_parse(const char * buffer, const std::string& filename);


TorusCompiler toruscompiler;
std::string tscp_current_filename;


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
        try {
            ast::Node* tree = tscp_parse(str.c_str(), current_file);
            if (nullptr == tree) {
                TORUSSHELLECHO("Compiling " << current_file << "... parser unknown error.");
                ast::GarbageCollector::cleanup();
                continue;
            }
            if (false == tree->validate()) {
                TORUSSHELLECHO("Compiling " << current_file << "... semantic error.");
            } else {
                TORUSSHELLECHO("Compiling " << current_file << "... OK");
                tree->generate();
            }
            delete tree;
            ast::GarbageCollector::cleanup();
        }
        catch (ttl::LexicalError& e) {
            TORUSSHELLECHO("Compiling " << current_file << "... " << e.what());
            ast::GarbageCollector::cleanup();
        }
        catch (ttl::SyntaxError& e) {
            TORUSSHELLECHO("Compiling " << current_file << "... " << e.what());
            ast::GarbageCollector::cleanup();
        }
    }
}