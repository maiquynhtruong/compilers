#include "symbol_table.h"
#include "debug.h"

extern SymbolTable *symbolTable;

int main() {
    Entry *entry;

    init_symbol_table();

    entry = create_program_entry("PROGRAM");
    enter_scope(entry->progAttrs->scope);

    entry = create_constant_entry("CONST1");
    entry->constAttrs->constantValue = make_int_constant(28);
    declare_entry(entry);

    entry = create_constant_entry("CONST2");
    entry->constAttrs->constantValue = make_char_constant('m');
    declare_entry(entry);

    entry = create_type_entry("TYPE1");
    entry->typeAttrs->type = make_array_type(5, make_int_type());
    declare_entry(entry);

    // TODO: Declare global variables

    entry = create_variable_entry("VAR1");
    entry->varAttrs->type = make_int_type();
    entry->varAttrs->scope = symbolTable->currentScope;
    declare_entry(entry);

    entry = create_variable_entry("VAR2");
    entry->varAttrs->type = make_string_type();
    entry->varAttrs->scope = symbolTable->currentScope;
    declare_entry(entry);

    entry = create_procedure_entry("PROCEDURE1");
    declare_entry(entry);

    enter_scope(entry->procAttrs->scope);

    entry = create_parameter_entry("PARAM1", symbolTable->currentScope->parent);
    entry->paramAttrs->type = make_int_type();
    declare_entry(entry);

    entry = create_parameter_entry("PARAM2", symbolTable->currentScope->parent);
    entry->paramAttrs->type = make_char_type();
    declare_entry(entry);

    exit_scope();

    entry = create_procedure_entry("PROCEDURE2");
    declare_entry(entry);

    enter_scope(entry->procAttrs->scope);

    entry = create_parameter_entry("PARAM3", symbolTable->currentScope->parent);
    entry->paramAttrs->type = make_bool_type();
    declare_entry(entry);

    entry = create_constant_entry("CONST3");
    entry->constAttrs->constantValue = make_bool_constant(true);
    declare_entry(entry);

    entry = create_constant_entry("CONST4");
    entry->constAttrs->constantValue = make_char_constant('m');
    declare_entry(entry);

    entry = create_type_entry("TYPE3");
    entry->typeAttrs->type = make_char_type();
    declare_entry(entry);

    entry = create_variable_entry("VAR3");
    entry->varAttrs->type = make_array_type(5, make_char_type());
    entry->varAttrs->scope = symbolTable->currentScope;
    declare_entry(entry);

    exit_scope(); // exit PROCEDURE2

    exit_scope(); // exit PROGRAM
    print_entry(symbolTable->program);
    clear_symbol_table();

    return 0;
}
