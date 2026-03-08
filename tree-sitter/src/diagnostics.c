#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <tree_sitter/api.h>
#include <tree_sitter_swift_extra/diagnostics.h>

// Symbol Index Implementation
typedef struct TSSwiftSymbol {
    char *name;
    char *file_path;
    uint32_t line;
    uint32_t column;
} TSSwiftSymbol;

struct TSSwiftIndex {
    TSSwiftSymbol *symbols;
    size_t count;
    size_t capacity;
};

TSSwiftIndex* ts_swift_index_new() {
    TSSwiftIndex *index = (TSSwiftIndex*)malloc(sizeof(TSSwiftIndex));
    index->count = 0;
    index->capacity = 128;
    index->symbols = (TSSwiftSymbol*)malloc(sizeof(TSSwiftSymbol) * index->capacity);
    return index;
}

void ts_swift_index_delete(TSSwiftIndex *index) {
    if (index) {
        for (size_t i = 0; i < index->count; i++) {
            free(index->symbols[i].name);
            free(index->symbols[i].file_path);
        }
        free(index->symbols);
        free(index);
    }
}

void ts_swift_index_add_declaration(TSSwiftIndex *index, const char *file_path, const char *symbol_name, uint32_t line, uint32_t column) {
    if (index->count >= index->capacity) {
        index->capacity *= 2;
        index->symbols = (TSSwiftSymbol*)realloc(index->symbols, sizeof(TSSwiftSymbol) * index->capacity);
    }
    index->symbols[index->count].name = strdup(symbol_name);
    index->symbols[index->count].file_path = strdup(file_path);
    index->symbols[index->count].line = line;
    index->symbols[index->count].column = column;
    index->count++;
}

bool ts_swift_index_has_symbol(TSSwiftIndex *index, const char *symbol_name) {
    for (size_t i = 0; i < index->count; i++) {
        if (strcmp(index->symbols[i].name, symbol_name) == 0) {
            return true;
        }
    }
    return false;
}

// Diagnostic Engine Implementation
struct TSSwiftDiagnosticEngine {
    TSSwiftDiagnostic *diagnostics;
    uint32_t count;
    uint32_t capacity;
};

TSSwiftDiagnosticEngine* ts_swift_diagnostic_engine_new() {
    TSSwiftDiagnosticEngine *engine = (TSSwiftDiagnosticEngine*)malloc(sizeof(TSSwiftDiagnosticEngine));
    engine->count = 0;
    engine->capacity = 32;
    engine->diagnostics = (TSSwiftDiagnostic*)malloc(sizeof(TSSwiftDiagnostic) * engine->capacity);
    return engine;
}

void ts_swift_diagnostic_engine_delete(TSSwiftDiagnosticEngine *engine) {
    if (engine) {
        for (uint32_t i = 0; i < engine->count; i++) {
            if (engine->diagnostics[i].message) {
                free((void*)engine->diagnostics[i].message);
            }
        }
        free(engine->diagnostics);
        free(engine);
    }
}

void ts_swift_diagnostic_engine_add_external(TSSwiftDiagnosticEngine *engine, TSSwiftDiagnostic diagnostic) {
    if (engine->count >= engine->capacity) {
        engine->capacity *= 2;
        engine->diagnostics = (TSSwiftDiagnostic*)realloc(engine->diagnostics, sizeof(TSSwiftDiagnostic) * engine->capacity);
    }
    engine->diagnostics[engine->count] = diagnostic;
    engine->diagnostics[engine->count].message = strdup(diagnostic.message);
    engine->count++;
}

extern const TSLanguage *tree_sitter_swift(void);

static void walk_syntax_errors(TSNode node, TSSwiftDiagnosticEngine *engine) {
    if (ts_node_is_error(node)) {
        TSPoint start = ts_node_start_point(node);
        TSPoint end = ts_node_end_point(node);
        TSSwiftDiagnostic diag = {
            .start_line = start.row,
            .start_column = start.column,
            .end_line = end.row,
            .end_column = end.column,
            .severity = TS_SWIFT_DIAGNOSTIC_ERROR,
            .source = TS_SWIFT_DIAGNOSTIC_SOURCE_TREE_SITTER,
            .message = "Syntax error"
        };
        ts_swift_diagnostic_engine_add_external(engine, diag);
    }

    uint32_t child_count = ts_node_child_count(node);
    for (uint32_t i = 0; i < child_count; i++) {
        walk_syntax_errors(ts_node_child(node, i), engine);
    }
}

void ts_swift_diagnostic_engine_run_syntax(TSSwiftDiagnosticEngine *engine, const char *source_code) {
    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_swift());

    TSTree *tree = ts_parser_parse_string(parser, NULL, source_code, strlen(source_code));
    TSNode root_node = ts_tree_root_node(tree);

    walk_syntax_errors(root_node, engine);

    ts_tree_delete(tree);
    ts_parser_delete(parser);
}

uint32_t ts_swift_diagnostic_engine_count(TSSwiftDiagnosticEngine *engine) {
    return engine->count;
}

TSSwiftDiagnostic ts_swift_diagnostic_engine_get(TSSwiftDiagnosticEngine *engine, uint32_t index) {
    if (index < engine->count) {
        return engine->diagnostics[index];
    }
    TSSwiftDiagnostic empty = {0};
    return empty;
}
