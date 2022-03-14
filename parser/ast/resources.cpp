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

#include <parser/ast/resources.h>
#include <parser/compiler.h>

namespace ast {

    BlockResourcesNode::BlockResourcesNode(const ttl::dynamicstack<std::string>& strings) {
        _strings = ttl::stack_to_vector(strings);
    }

    bool BlockResourcesNode::validate() {
        return true;
    }

    void BlockResourcesNode::generate() {
        for(udword_t i = 0; i < _strings.size(); ++i) {
            toruscompiler.add_file(_strings[i]);
        }
    }
}
