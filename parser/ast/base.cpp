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

#include <parser/ast/base.h>
#include <parser/ast/ast_garbage_collector.h>

namespace ast {

    Node* root = nullptr;

    Node::Node() {
        NODE_INFO();
        GarbageCollector::register_node(this);
    }

    Node::~Node() {
        NODE_INFO();
        GarbageCollector::unregister_node(this);
    }

    tpu::DataType Node::get_type() const {
        return tpu::DataType::NO_TYPE;
    }

    bool Node::is_const() const {
        return true;
    }

    MonoNode::MonoNode(Node* c) : _c(c) {}

    bool MonoNode::validate() {
        if (_c) return _c->validate();
        return true;
    }

    void MonoNode::generate() {
        if (_c) _c->generate();
    }

    MonoNode::~MonoNode() {
        if (_c) delete _c;
    }


    BiNode::BiNode(Node* l, Node* r) : _l(l), _r(r) {}

    bool BiNode::validate() {
        bool ret = true;
        if (_l) ret = ret && _l->validate();
        if (ret && _r) ret = ret && _r->validate();
        return ret;
    }

    void BiNode::generate() {
        if (_l) _l->generate();
        if (_r) _r->generate();
    }

    BiNode::~BiNode() {
        if (_l) delete _l;
        if (_r) delete _r;
    }


    TriNode::TriNode(Node* l, Node* c, Node* r) : _l(l), _c(c), _r(r) {}

    bool TriNode::validate() {
        bool ret = true;
        if (_l) ret = ret && _l->validate();
        if (ret && _c) ret = ret && _c->validate();
        if (ret && _r) ret = ret && _r->validate();
        return ret;
    }

    void TriNode::generate() {
        if (_l) _l->generate();
        if (_c) _c->generate();
        if (_r) _r->generate();
    }

    TriNode::~TriNode() {
        if (_l) delete _l;
        if (_c) delete _c;
        if (_r) delete _r;
    }
}
