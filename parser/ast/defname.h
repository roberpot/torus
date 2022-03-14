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

#ifndef __TORUS_PARSER_AST_DEFNAME_H
#define __TORUS_PARSER_AST_DEFNAME_H

#include <string>

#include <library/types.h>
#include <parser/ast/base.h>

namespace ast {

    class DefnameBlockNode : public MonoNode {
    public:
        DefnameBlockNode(const std::string& id, const std::string& file, uqword_t lineno, Node* n);
        bool validate();
    private:
        std::string _id;
        std::string _file;
        uqword_t _lineno;
    };

    class DefnameEntryNode : public MonoNode {
    public:
        DefnameEntryNode(const std::string& i, const std::string& f, uqword_t l, Node* n);
        bool validate();
        std::string id;
        std::string file;
        uqword_t lineno;
    };

    class DefnameArrayEntryNode : public MonoNode {
    public:
        DefnameArrayEntryNode(const std::string& i, const std::string& f, uqword_t l, uqword_t in, Node* n);
        bool validate();
        std::string id;
        std::string file;
        uqword_t lineno;
        uqword_t index;
    };
}

#endif //__TORUS_PARSER_AST_DEFNAME_H
