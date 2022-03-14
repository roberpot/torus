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

#ifndef __TORUS_PARSER_AST_GARBAGE_COLLECTOR_H
#define __TORUS_PARSER_AST_GARBAGE_COLLECTOR_H

#include <library/vector.h>

#include <parser/ast/ast.h>

namespace ast {

    class GarbageCollector {
    public:
        static void register_node(Node* n);
        static void unregister_node(Node* n);
        static void cleanup();
    private:
        struct NodeInfo {
            NodeInfo();
            NodeInfo(Node* n);
            bool collected;
            Node* node;
        };
        static ttl::vector<NodeInfo> _nodes;
    };

}

#endif //__TORUS_PARSER_AST_GARBAGE_COLLECTOR_H
