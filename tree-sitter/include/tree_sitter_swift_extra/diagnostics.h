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

typedef struct {
    uint32_t start_line;
    uint32_t start_column;
    uint32_t end_line;
    uint32_t end_column;
    TSSwiftDiagnosticSeverity severity;
    const char *message;
} TSSwiftDiagnostic;

typedef struct TSSwiftIndex TSSwiftIndex;

TSSwiftIndex* ts_swift_index_new();
void ts_swift_index_delete(TSSwiftIndex *index);

void ts_swift_index_add_declaration(TSSwiftIndex *index, const char *file_path, const char *symbol_name, uint32_t line, uint32_t column);
bool ts_swift_index_has_symbol(TSSwiftIndex *index, const char *symbol_name);

// Function to run diagnostics on a file given its content and the global index
void ts_swift_run_diagnostics(TSSwiftIndex *index, const char *file_path, const char *source_code, void (*callback)(TSSwiftDiagnostic));

#ifdef __cplusplus
}
#endif

#endif // TREE_SITTER_SWIFT_EXTRA_DIAGNOSTICS_H_
