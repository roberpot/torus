set(LIBPQXX_MAIN_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/basic_connection          ${CMAKE_CURRENT_LIST_DIR}/basic_connection.hxx
    ${CMAKE_CURRENT_LIST_DIR}/binarystring              ${CMAKE_CURRENT_LIST_DIR}/binarystring.hxx          ${CMAKE_CURRENT_LIST_DIR}/binarystring.cxx
    ${CMAKE_CURRENT_LIST_DIR}/compiler-internal.hxx     ${CMAKE_CURRENT_LIST_DIR}/compiler-internal-post.hxx ${CMAKE_CURRENT_LIST_DIR}/compiler-internal-pre.hxx
    ${CMAKE_CURRENT_LIST_DIR}/compiler-public.hxx
    ${CMAKE_CURRENT_LIST_DIR}/config.h                  ${CMAKE_CURRENT_LIST_DIR}/config.h.in
    ${CMAKE_CURRENT_LIST_DIR}/config-internal-autotools.h
    ${CMAKE_CURRENT_LIST_DIR}/config-internal-libpq.h
    ${CMAKE_CURRENT_LIST_DIR}/config-internal-compiler.h
    ${CMAKE_CURRENT_LIST_DIR}/config-public-compiler.h
    ${CMAKE_CURRENT_LIST_DIR}/connection                ${CMAKE_CURRENT_LIST_DIR}/connection.hxx            ${CMAKE_CURRENT_LIST_DIR}/connection.cxx
    ${CMAKE_CURRENT_LIST_DIR}/connection_base           ${CMAKE_CURRENT_LIST_DIR}/connection_base.hxx       ${CMAKE_CURRENT_LIST_DIR}/connection_base.cxx
    ${CMAKE_CURRENT_LIST_DIR}/connectionpolicy          ${CMAKE_CURRENT_LIST_DIR}/connectionpolicy.hxx
    ${CMAKE_CURRENT_LIST_DIR}/cursor                    ${CMAKE_CURRENT_LIST_DIR}/cursor.hxx                ${CMAKE_CURRENT_LIST_DIR}/cursor.cxx
    ${CMAKE_CURRENT_LIST_DIR}/dbtransaction             ${CMAKE_CURRENT_LIST_DIR}/dbtransaction.hxx         ${CMAKE_CURRENT_LIST_DIR}/dbtransaction.cxx
    ${CMAKE_CURRENT_LIST_DIR}/errorhandler              ${CMAKE_CURRENT_LIST_DIR}/errorhandler.hxx          ${CMAKE_CURRENT_LIST_DIR}/errorhandler.cxx
    ${CMAKE_CURRENT_LIST_DIR}/except                    ${CMAKE_CURRENT_LIST_DIR}/except.hxx                ${CMAKE_CURRENT_LIST_DIR}/except.cxx
    ${CMAKE_CURRENT_LIST_DIR}/field                     ${CMAKE_CURRENT_LIST_DIR}/field.hxx                 ${CMAKE_CURRENT_LIST_DIR}/field.cxx
    ${CMAKE_CURRENT_LIST_DIR}/isolation                 ${CMAKE_CURRENT_LIST_DIR}/isolation.hxx
    ${CMAKE_CURRENT_LIST_DIR}/largeobject               ${CMAKE_CURRENT_LIST_DIR}/largeobject.hxx           ${CMAKE_CURRENT_LIST_DIR}/largeobject.cxx
    ${CMAKE_CURRENT_LIST_DIR}/nontransaction            ${CMAKE_CURRENT_LIST_DIR}/nontransaction.hxx        ${CMAKE_CURRENT_LIST_DIR}/nontransaction.cxx
    ${CMAKE_CURRENT_LIST_DIR}/notification              ${CMAKE_CURRENT_LIST_DIR}/notification.hxx          ${CMAKE_CURRENT_LIST_DIR}/notification.cxx
    ${CMAKE_CURRENT_LIST_DIR}/notify-listen             ${CMAKE_CURRENT_LIST_DIR}/notify-listen.hxx         ${CMAKE_CURRENT_LIST_DIR}/notify-listen.cxx
    ${CMAKE_CURRENT_LIST_DIR}/performance.hxx
    ${CMAKE_CURRENT_LIST_DIR}/pipeline                  ${CMAKE_CURRENT_LIST_DIR}/pipeline.hxx              ${CMAKE_CURRENT_LIST_DIR}/pipeline.cxx
    ${CMAKE_CURRENT_LIST_DIR}/prepared_statement        ${CMAKE_CURRENT_LIST_DIR}/prepared_statement.hxx    ${CMAKE_CURRENT_LIST_DIR}/prepared_statement.cxx
    ${CMAKE_CURRENT_LIST_DIR}/result                    ${CMAKE_CURRENT_LIST_DIR}/result.hxx                ${CMAKE_CURRENT_LIST_DIR}/result.cxx
    ${CMAKE_CURRENT_LIST_DIR}/prepared_statement        ${CMAKE_CURRENT_LIST_DIR}/prepared_statement.hxx    ${CMAKE_CURRENT_LIST_DIR}/prepared_statement.cxx
    ${CMAKE_CURRENT_LIST_DIR}/robusttransaction         ${CMAKE_CURRENT_LIST_DIR}/robusttransaction.hxx     ${CMAKE_CURRENT_LIST_DIR}/robusttransaction.cxx
    ${CMAKE_CURRENT_LIST_DIR}/statement_parameters.cxx
    ${CMAKE_CURRENT_LIST_DIR}/strconv                   ${CMAKE_CURRENT_LIST_DIR}/strconv.hxx               ${CMAKE_CURRENT_LIST_DIR}/strconv.cxx
    ${CMAKE_CURRENT_LIST_DIR}/subtransaction            ${CMAKE_CURRENT_LIST_DIR}/subtransaction.hxx        ${CMAKE_CURRENT_LIST_DIR}/subtransaction.cxx
    ${CMAKE_CURRENT_LIST_DIR}/tablereader               ${CMAKE_CURRENT_LIST_DIR}/tablereader.hxx           ${CMAKE_CURRENT_LIST_DIR}/tablereader.cxx
    ${CMAKE_CURRENT_LIST_DIR}/tablestream               ${CMAKE_CURRENT_LIST_DIR}/tablestream.hxx           ${CMAKE_CURRENT_LIST_DIR}/tablestream.cxx
    ${CMAKE_CURRENT_LIST_DIR}/tablewriter               ${CMAKE_CURRENT_LIST_DIR}/tablewriter.hxx           ${CMAKE_CURRENT_LIST_DIR}/tablewriter.cxx
    ${CMAKE_CURRENT_LIST_DIR}/transaction               ${CMAKE_CURRENT_LIST_DIR}/transaction.hxx           ${CMAKE_CURRENT_LIST_DIR}/transaction.cxx
    ${CMAKE_CURRENT_LIST_DIR}/transaction_base          ${CMAKE_CURRENT_LIST_DIR}/transaction_base.hxx      ${CMAKE_CURRENT_LIST_DIR}/transaction_base.cxx
    ${CMAKE_CURRENT_LIST_DIR}/transactor                ${CMAKE_CURRENT_LIST_DIR}/transactor.hxx
    ${CMAKE_CURRENT_LIST_DIR}/trigger                   ${CMAKE_CURRENT_LIST_DIR}/trigger.hxx
    ${CMAKE_CURRENT_LIST_DIR}/tuple                     ${CMAKE_CURRENT_LIST_DIR}/tuple.hxx                 ${CMAKE_CURRENT_LIST_DIR}/tuple.cxx
    ${CMAKE_CURRENT_LIST_DIR}/util                      ${CMAKE_CURRENT_LIST_DIR}/util.hxx                  ${CMAKE_CURRENT_LIST_DIR}/util.cxx
    ${CMAKE_CURRENT_LIST_DIR}/version                   ${CMAKE_CURRENT_LIST_DIR}/version.hxx
)
SOURCE_GROUP(db\\libpqxx FILES ${LIBPQXX_MAIN_SOURCES})

set(LIBPQXX_INTERNAL_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/internal/callgate.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/libpq-forward.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/result_data.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/statement_parameters.hxx
)
SOURCE_GROUP(db\\libpqxx\\internal FILES ${LIBPQXX_INTERNAL_SOURCES})

set(LIBPQXX_INTERNAL_GATES_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-dbtransaction.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-errorhandler.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-largeobject.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-notification_receiver.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-parameterized_invocation.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-pipeline.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-prepare-invocation.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-reactivation_avoidance_exemption.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-sql_cursor.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/connection-transaction.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/errorhandler-connection.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/icursor_iterator-icursorstream.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/icursorstream-icursor_iterator.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/result-connection.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/result-creation.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/result-sql_cursor.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/transaction-subtransaction.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/transaction-tablereader.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/transaction-tablewriter.hxx
    ${CMAKE_CURRENT_LIST_DIR}/internal/gates/transaction-transactionfocus.hxx
)
SOURCE_GROUP(db\\libpqxx\\internal\\gates FILES ${LIBPQXX_INTERNAL_GATES_SOURCES})

set(LIBPQXX_SOURCES
    ${LIBPQXX_MAIN_SOURCES}
    ${LIBPQXX_INTERNAL_SOURCES}
    ${LIBPQXX_INTERNAL_GATES_SOURCES})