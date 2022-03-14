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

#include <set>

#include <parser/ast/arithmetic.h>

namespace ast {

    BitwiseOrNode::BitwiseOrNode(Node* l, Node* r) : BiNode(l, r) {}

    IntegerValueNode::IntegerValueNode(qword_t v) : value(v) {}

    bool IntegerValueNode::validate() {
        return true;
    }

    void IntegerValueNode::generate() {
        // TODO
    }

    tpu::DataType IntegerValueNode::get_type() const {
        return tpu::DataType::INTEGER;
    }

    bool IntegerValueNode::is_const() const {
        return true;
    }

    StringValueNode::StringValueNode(const std::string& v) : value(v) {}

    bool StringValueNode::validate() {
        return true;
    }

    void StringValueNode::generate() {
        // TODO.
    }

    tpu::DataType StringValueNode::get_type() const {
        return tpu::DataType::STRING;
    }

    bool StringValueNode::is_const() const {
        return true;
    }

    RandomExpressionNode::RandomExpressionNode(const ttl::vector<Node*>& n) : nodes(n) {
    }

    bool RandomExpressionNode::validate() {
        bool ret = true;
        if (nodes.size() == 0) {
            return false;
        }
        for(uqword_t i = 0; i < nodes.size(); ++i) {
            ret = ret && nodes[i]->validate();
        }
        return ret;
    }

    void RandomExpressionNode::generate() {
        // TODO
    }

    tpu::DataType RandomExpressionNode::get_type() const {
        return tpu::DataType::NO_TYPE;
    }

    bool RandomExpressionNode::is_const() const {
        bool is_const = true;
        for(uqword_t i = 0; i < nodes.size(); ++i) {
            is_const = is_const && nodes[i]->is_const();
        }
        return is_const;
    }

    SingleRandomRangeNode::SingleRandomRangeNode(qword_t r, Node* n) : MonoNode(n), ratio(r) {}

    bool SingleRandomRangeNode::validate() {
        return _c->validate();
    }

    void SingleRandomRangeNode::generate() {
        // TODO
    }

    tpu::DataType SingleRandomRangeNode::get_type() const {
        return _c->get_type();
    }

    bool SingleRandomRangeNode::is_const() const {
        return _c->is_const();
    }

    IdentifierNode::IdentifierNode(const std::string& i, const std::string& f, uqword_t l)
        : id(i), file(f), lineno(l) {}

    bool IdentifierNode::validate() {
        // TODO: check cache table.
        return true;
    }

    void IdentifierNode::generate() {
        // TODO.
    }

    tpu::DataType IdentifierNode::get_type() const {
        // TODO: check type on cache table.
        return tpu::DataType::UNKNOWN;
    }

    bool IdentifierNode::is_const() const {
        // TODO: check type on cache table.
        return true;
    }

    ArrayAccessNode::ArrayAccessNode(const std::string& i, const std::string& f, uqword_t l, Node* c) : MonoNode(c), id(i), file(f), lineno(l) {}

    bool ArrayAccessNode::validate() {
        return _c->get_type() == tpu::DataType::INTEGER;
    }

    void ArrayAccessNode::generate() {
        // TODO
    }

    tpu::DataType ArrayAccessNode::get_type() const {
        // TODO
        return tpu::DataType::UNKNOWN;
    }

    bool ArrayAccessNode::is_const() const {
        // TODO
        return true;
    }
}
