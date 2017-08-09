# file      : odb/pgsql/Makefile.am
# copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

lib_LTLIBRARIES = libodb-pgsql.la
libodb_pgsql_la_SOURCES = auto-handle.cxx connection.cxx connection-factory.cxx database.cxx error.cxx exceptions.cxx prepared-query.cxx query.cxx query-dynamic.cxx query-const-expr.cxx simple-object-statements.cxx statement.cxx statements-base.cxx tracer.cxx traits.cxx transaction.cxx transaction-impl.cxx details/endian-traits.cxx details/options.cxx

odbpgsqlincludedir = $(includedir)/odb/pgsql/
nobase_odbpgsqlinclude_HEADERS = auto-handle.hxx binding.hxx connection-factory.hxx connection.hxx connection.ixx container-statements.hxx container-statements.txx database.hxx database.ixx details/config.hxx details/conversion.hxx details/endian-traits.hxx details/export.hxx details/options.hxx details/options.ixx error.hxx error.ixx exceptions.hxx forward.hxx no-id-object-result.hxx no-id-object-result.txx no-id-object-statements.hxx no-id-object-statements.txx pgsql-fwd.hxx pgsql-oid.hxx pgsql-types.hxx polymorphic-object-result.hxx polymorphic-object-result.txx polymorphic-object-statements.hxx polymorphic-object-statements.txx prepared-query.hxx query-dynamic.hxx query-dynamic.ixx query-dynamic.txx query.hxx query.ixx query.txx section-statements.hxx section-statements.txx simple-object-result.hxx simple-object-result.txx simple-object-statements.hxx simple-object-statements.ixx simple-object-statements.txx statement-cache.hxx statement-cache.txx statement.hxx statements-base.hxx tracer.hxx traits-calls.hxx traits.hxx transaction-impl.hxx transaction-impl.ixx transaction.hxx transaction.ixx version.hxx view-result.hxx view-result.txx view-statements.hxx view-statements.txx
nobase_nodist_odbpgsqlinclude_HEADERS = details/config.h

EXTRA_DIST = details/options.cli details/config.h.in libodb-pgsql-vc8.vcproj libodb-pgsql-vc9.vcproj libodb-pgsql-vc10.vcxproj libodb-pgsql-vc10.vcxproj.filters libodb-pgsql-vc11.vcxproj libodb-pgsql-vc11.vcxproj.filters libodb-pgsql-vc12.vcxproj libodb-pgsql-vc12.vcxproj.filters

AM_CPPFLAGS= -I'$(top_builddir)' -I'$(top_srcdir)' -DLIBODB_PGSQL_DYNAMIC_LIB
AM_LDFLAGS = -release 2.4 -no-undefined
