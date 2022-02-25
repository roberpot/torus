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

#ifndef __TORUS_AST_H
#define __TORUS_AST_H

#include <string>

#include <library/stack.h>
#include <library/vector.h>


namespace ast {

    extern class Node {
    public:
        Node() {}
        virtual void generate() = 0;
        virtual ~Node() {};
    private:
    } * root;

    class MonoNode : public Node {
    public:
        MonoNode(Node* c) : _c(c) {}
        virtual void generate();
        virtual ~MonoNode();
    private:
        Node* _c;
    };

    class BiNode : public Node {
    public:
        BiNode(Node* l, Node* r) : _l(l), _r(r) {}
        virtual void generate();
        virtual ~BiNode();
    private:
        Node * _l, * _r;
    };

    class TriNode : public Node {
    public:
        TriNode(Node* l, Node* c, Node* r) : _l(l), _c(c), _r(r) {}
        virtual void generate();
        virtual ~TriNode();
    private:
        Node * _l, * _c, * _r;
    };

    class BlockResourcesNode : public Node {
    public:
        BlockResourcesNode(const ttl::dynamicstack<std::string>& strings);
        void generate();
    private:
        ttl::vector<std::string> _strings;
    };

    class BlockObsceneNode : public Node {
    public:
        BlockObsceneNode(const ttl::vector<std::string>& strings) : _strings(strings) {}
        void generate();
    private:
        ttl::vector<std::string> _strings;
    };

    class BlockFameNode : public Node {
    public:
        BlockFameNode(const ttl::vector<int>& ints, const ttl::vector<std::string>& strings) : _ints(ints), _strings(strings) {}
        void generate();
    private:
        ttl::vector<int> _ints;
        ttl::vector<std::string> _strings;
    };

    class BlockKarmaNode : public Node {
    public:
        BlockKarmaNode(const ttl::vector<int>& ints, const ttl::vector<std::string>& strings) : _ints(ints), _strings(strings) {}
        void generate();
    private:
        ttl::vector<int> _ints;
        ttl::vector<std::string> _strings;
    };

    class BlockNototitlesNode : public Node {
    public:
        BlockNototitlesNode(const ttl::vector<int>& ints1, const ttl::vector<int>& ints2, ttl::vector<std::string> strings) : _ints1(ints1), _ints2(ints2), _strings(strings) {}
        void generate();
    private:
        ttl::vector<int> _ints1, _ints2;
        ttl::vector<std::string> _strings;
    };

    class BlockRunesNode : public Node {
    public:
        BlockRunesNode(const ttl::vector<std::string>& strings) : _strings(strings) {}
        void generate();
    private:
        ttl::vector<std::string> _strings;
    };

    class BlockPlevelNode : public Node {
    public:
        BlockPlevelNode(udword_t plevel, const ttl::vector<std::string>& strings) : _plevel(plevel), _strings(strings) {}
        void generate();
    private:
        udword_t _plevel;
        ttl::vector<std::string> _strings;
    };
}

#endif //__TORUS_AST_H
