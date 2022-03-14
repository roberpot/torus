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

#include <parser/ast/ast_garbage_collector.h>

namespace ast {

    ttl::vector<GarbageCollector::NodeInfo> GarbageCollector::_nodes;

    void GarbageCollector::register_node(Node* n) {
        _nodes.push_back(NodeInfo(n));
    }

    void GarbageCollector::unregister_node(Node* n) {
        for(uqword_t i = 0; i < _nodes.size(); ++i) {
            if (_nodes[i].node == n) {
                _nodes[i].collected = true;
            }
        }
    }

    void GarbageCollector::cleanup() {
        for(qword_t i = _nodes.size(); i >= 0; --i) {
            if (false == _nodes[i].collected) {
                delete _nodes[i].node;
            }
        }
        _nodes.resize(0);
    }

    GarbageCollector::NodeInfo::NodeInfo() :
    collected(true), node(nullptr) {}

    GarbageCollector::NodeInfo::NodeInfo(Node* n) :
    collected(false), node(n) {}
}