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

#include "ast.h"
#include "../shell.h"
#include "../core/config.h"
#include "../debug/debug.h"
#include "compiler.h"


namespace ast {

    Node * root = NULL;

    void MonoNode::generate() {
        if (_c) _c->generate();
    }

    MonoNode::~MonoNode() {
        if (_c) delete _c;
    }

    BiNode::~BiNode() {
        if (_l) delete _l;
        if (_r) delete _r;
    }

    void BiNode::generate() {
        if (_l) _l->generate();
        if (_r) _r->generate();
    }

    TriNode::~TriNode() {
        if (_l) delete _l;
        if (_c) delete _c;
        if (_r) delete _r;
    }

    void TriNode::generate() {
        if (_l) _l->generate();
        if (_c) _c->generate();
        if (_r) _r->generate();
    }

    void BlockResourcesNode::generate() {
        for(t_udword i = 0; i < _strings.size(); ++i) {
            toruscompiler.add_file(_strings[i]);
        }
    }

    void BlockObsceneNode::generate() {
        for(t_udword i = 0; i < _strings.size(); ++i) {
            toruscfg.obscene_strings.insert(_strings[i]);
        }
    }

    void BlockFameNode::generate() {
        DEBUG_MSG("Fame ranges: " << _ints.size() << " titles: " << _strings.size());
        if (_ints.size() != _strings.size()) {
            // TODO: Fame ranges and titles incorrect.
        }
        toruscfg.fame_ranges = _ints;
        toruscfg.fame_titles = _strings;
    }

    void BlockKarmaNode::generate() {
        DEBUG_MSG("Karma ranges: " << _ints.size() << " titles: " << _strings.size());
        if (_ints.size() != _strings.size()) {
            // TODO: Karma ranges and titles incorrect.
        }
        toruscfg.karma_ranges = _ints;
        toruscfg.karma_titles = _strings;
    }

    void BlockNototitlesNode::generate() {
        DEBUG_MSG("Nototitles ranges: " << _ints1.size() << " " << _ints2.size() << " (" << (_ints1.size() * _ints2.size()) << ") " << " titles: " << _strings.size());
        if ((_ints1.size() * _ints2.size()) != _strings.size()) {
            // TODO: Nototitles ranges and titles incorrect.
        }
        toruscfg.noto_fame_ranges = _ints1;
        toruscfg.noto_karma_ranges = _ints2;
        toruscfg.noto_titles = _strings;
    }

    void BlockRunesNode::generate() {
        DEBUG_MSG("Runes count: " << _strings.size());
        toruscfg.runes = _strings;
    }

    void BlockPlevelNode::generate() {
        DEBUG_MSG("Plevel: " << _plevel << " function count: " << _strings.size());
    }

}