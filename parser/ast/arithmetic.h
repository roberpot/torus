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

#ifndef __TORUS_PARSER_AST_ARITHMETIC_H
#define __TORUS_PARSER_AST_ARITHMETIC_H

#include <string>

#include <library/types.h>
#include <library/vector.h>
#include <parser/ast/base.h>

namespace ast {

    class BitwiseOrNode : public BiNode {
    public:
        BitwiseOrNode(Node* l, Node* r);
    };

    class IntegerValueNode : public Node {
    public:
        IntegerValueNode(qword_t v);
        bool validate();
        void generate();
        tpu::DataType get_type() const;
        bool is_const() const;
        qword_t value;
    };

    class StringValueNode : public Node {
    public:
        StringValueNode(const std::string& v);
        bool validate();
        void generate();
        tpu::DataType get_type() const;
        bool is_const() const;
        std::string value;
    };

    class RandomExpressionNode : public Node {
    public:
        RandomExpressionNode(const ttl::vector<Node*>& n);
        bool validate();
        void generate();
        tpu::DataType get_type() const;
        bool is_const() const;
        ttl::vector<Node*> nodes;
    };

    class SingleRandomRangeNode : public MonoNode {
    public:
        SingleRandomRangeNode(qword_t r, Node* n);
        bool validate();
        void generate();
        tpu::DataType get_type() const;
        bool is_const() const;
        qword_t ratio;
    };

    class IdentifierNode : public Node {
    public:
        IdentifierNode(const std::string& i, const std::string& f, uqword_t l);
        bool validate();
        void generate();
        tpu::DataType get_type() const;
        bool is_const() const;
        std::string id;
        std::string file;
        uqword_t lineno;
    };

    class ArrayAccessNode : public MonoNode {
    public:
        ArrayAccessNode(const std::string& i, const std::string& f, uqword_t l, Node* c);
        bool validate();
        void generate();
        tpu::DataType get_type() const;
        bool is_const() const;
        std::string id;
        std::string file;
        uqword_t lineno;
    };
}

#endif //__TORUS_PARSER_AST_ARITHMETIC_H
