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

#ifndef __TORUS_PARSER_AST_RESOURCES_H
#define __TORUS_PARSER_AST_RESOURCES_H

#include <string>

#include <library/vector.h>
#include <library/utility.h>

#include <parser/ast/base.h>

namespace ast {

    class BlockResourcesNode : public Node {
    public:
        BlockResourcesNode(const ttl::dynamicstack<std::string>& strings);
        bool validate();
        void generate();
    private:
        ttl::vector<std::string> _strings;
    };

}

#endif //__TORUS_PARSER_AST_RESOURCES_H
