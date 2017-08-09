// file      : odb/traversal/union-template.hxx
// copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#ifndef ODB_TRAVERSAL_UNION_TEMPLATE_HXX
#define ODB_TRAVERSAL_UNION_TEMPLATE_HXX

#include <odb/semantics/union-template.hxx>

#include <odb/traversal/elements.hxx>
#include <odb/traversal/union.hxx>

namespace traversal
{
  struct union_template: scope_template<semantics::union_template> {};

  struct union_instantiation: scope_template<semantics::union_instantiation>
  {
    virtual void
    traverse (type&);

    virtual void
    instantiates (type&);

    virtual void
    instantiates (type&, edge_dispatcher&);
  };
}

#endif // ODB_TRAVERSAL_UNION_TEMPLATE_HXX
