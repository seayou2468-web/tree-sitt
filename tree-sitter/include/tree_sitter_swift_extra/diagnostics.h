#ifndef TREE_SITTER_SWIFT_EXTRA_DIAGNOSTICS_H_
#define TREE_SITTER_SWIFT_EXTRA_DIAGNOSTICS_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TS_SWIFT_DIAGNOSTIC_ERROR,
    TS_SWIFT_DIAGNOSTIC_WARNING,
    TS_SWIFT_DIAGNOSTIC_NOTE
} TSSwiftDiagnosticSeverity;

typedef enum {
    TS_SWIFT_DIAGNOSTIC_SOURCE_TREE_SITTER,
    TS_SWIFT_DIAGNOSTIC_SOURCE_SWIFT_COMPILER,
    TS_SWIFT_DIAGNOSTIC_SOURCE_LLVM
} TSSwiftDiagnosticSource;

typedef struct {
    uint32_t start_line;
    uint32_t start_column;
    uint32_t end_line;
    uint32_t end_column;
    TSSwiftDiagnosticSeverity severity;
    TSSwiftDiagnosticSource source;
    const char *message;
} TSSwiftDiagnostic;

typedef struct TSSwiftIndex TSSwiftIndex;

// Index management for cross-file symbol tracking (to be populated by caller)
TSSwiftIndex* ts_swift_index_new();
void ts_swift_index_delete(TSSwiftIndex *index);
void ts_swift_index_add_declaration(TSSwiftIndex *index, const char *file_path, const char *symbol_name, uint32_t line, uint32_t column);
bool ts_swift_index_has_symbol(TSSwiftIndex *index, const char *symbol_name);

// Diagnostic Engine
typedef struct TSSwiftDiagnosticEngine TSSwiftDiagnosticEngine;

TSSwiftDiagnosticEngine* ts_swift_diagnostic_engine_new();
void ts_swift_diagnostic_engine_delete(TSSwiftDiagnosticEngine *engine);

// Add diagnostic from external sources (LLVM/Swift Compiler)
void ts_swift_diagnostic_engine_add_external(TSSwiftDiagnosticEngine *engine, TSSwiftDiagnostic diagnostic);

// Run tree-sitter syntax diagnostics and combine with external ones
void ts_swift_diagnostic_engine_run_syntax(TSSwiftDiagnosticEngine *engine, const char *source_code);

// Retrieve all diagnostics
uint32_t ts_swift_diagnostic_engine_count(TSSwiftDiagnosticEngine *engine);
TSSwiftDiagnostic ts_swift_diagnostic_engine_get(TSSwiftDiagnosticEngine *engine, uint32_t index);

#ifdef __cplusplus
}
#endif

#endif // TREE_SITTER_SWIFT_EXTRA_DIAGNOSTICS_H_
