#ifndef TREE_SITTER_API_H_
#define TREE_SITTER_API_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t TSSymbol;
typedef uint16_t TSFieldId;

typedef struct TSLanguage TSLanguage;
typedef struct TSParser TSParser;
typedef struct TSTree TSTree;
typedef struct TSQuery TSQuery;
typedef struct TSQueryCursor TSQueryCursor;
typedef struct TSLookaheadIterator TSLookaheadIterator;

typedef struct {
  uint32_t row;
  uint32_t column;
} TSPoint;

typedef struct {
  TSPoint start_point;
  TSPoint end_point;
  uint32_t start_byte;
  uint32_t end_byte;
} TSRange;

typedef struct {
  uint32_t context[4];
  const void *id;
  const TSLanguage *language;
} TSNode;

TSParser *ts_parser_new(void);
void ts_parser_delete(TSParser *);
bool ts_parser_set_language(TSParser *, const TSLanguage *);
TSTree *ts_parser_parse_string(TSParser *, const TSTree *, const char *, uint32_t);
TSNode ts_tree_root_node(const TSTree *);
void ts_tree_delete(TSTree *);
bool ts_node_has_error(TSNode);
bool ts_node_is_error(TSNode);
TSPoint ts_node_start_point(TSNode);
TSPoint ts_node_end_point(TSNode);
uint32_t ts_node_child_count(TSNode);
TSNode ts_node_child(TSNode, uint32_t);

#ifdef __cplusplus
}
#endif

#endif // TREE_SITTER_API_H_
