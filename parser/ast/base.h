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

#ifndef __TORUS_PARSER_AST_BASE_H
#define __TORUS_PARSER_AST_BASE_H

#include <tpu/data.h>

#define NODE_DEBUG 0
#if NODE_DEBUG == 1
#define NODE_INFO() std::cout << __PRETTY_FUNCTION__ << ":" << this << std::endl
#else
#define NODE_INFO()
#endif

#define NODE_VISITABLE
/*
virtual void accept(ast::Visitor* visitor) { \
    visitor->visit(this);                    \
}*/

namespace ast {

    class Node {
    public:
        Node();
        virtual bool validate() = 0;
        virtual void generate() = 0;
        virtual tpu::DataType get_type() const;
        virtual bool is_const() const;
        virtual ~Node();
        NODE_VISITABLE
    };
    extern Node* root;

    class MonoNode : public Node {
    public:
        MonoNode(Node* c);
        virtual bool validate();
        virtual void generate();
        virtual ~MonoNode();
        Node* _c;
    };

    class BiNode : public Node {
    public:
        BiNode(Node* l, Node* r);
        virtual bool validate();
        virtual void generate();
        virtual ~BiNode();
        Node* _l;
        Node* _r;
    };

    class TriNode : public Node {
    public:
        TriNode(Node* l, Node* c, Node* r);
        virtual bool validate();
        virtual void generate();
        virtual ~TriNode();
        Node* _l;
        Node* _c;
        Node* _r;
    };
}

#endif //__TORUS_PARSER_AST_BASE_H
