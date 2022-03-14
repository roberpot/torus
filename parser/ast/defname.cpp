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

#include <parser/ast/defname.h>
#include <shell.h>

namespace ast {
    DefnameBlockNode::DefnameBlockNode(const std::string& id, const std::string& file, uqword_t lineno, Node* n)
    : MonoNode(n), _id(id), _file(file), _lineno(lineno) {
    }

    bool DefnameBlockNode::validate() {
        if (_id.find("d_") != 0) {
            TORUSSHELLECHO("[" << _lineno << "] WARNING: Defname identifier " << _id << " does not start with 'd_'.");
        }
        return MonoNode::validate();
    }

    DefnameEntryNode::DefnameEntryNode(const std::string& i, const std::string& f, uqword_t l, Node* n)
    : MonoNode(n), id(i), file(f), lineno(l) {
    }

    bool DefnameEntryNode::validate() {
        bool ret = MonoNode::validate() && _c->is_const();
        return ret;
    }

    DefnameArrayEntryNode::DefnameArrayEntryNode(const std::string& i, const std::string& f, uqword_t l, uqword_t in, Node* n) :
        MonoNode(n), id(i), file(f), lineno(l), index(in) {}

    bool DefnameArrayEntryNode::validate() {
        bool ret = MonoNode::validate() && _c->is_const();
        return ret;
    }

}