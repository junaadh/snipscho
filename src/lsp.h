#ifndef LSP_H_
#define LSP_H_

#include <jansson.h>
#include <stddef.h>
// define languages
#define LANG_NAMES(LANG)                                                       \
  LANG(bash)                                                                   \
  LANG(c)                                                                      \
  LANG(cpp)                                                                    \
  LANG(java)                                                                   \
  LANG(react)                                                                  \
  LANG(rust)                                                                   \
  LANG(null)

// macros to generate enums, path array and string array of languages
#define GEN_ENUM(ENUM) ENUM,
#define GEN_STR(STR) #STR,
#define GEN_PATH(STR) [STR] = "$HOME/.config/snips/" #STR ".json",
#define GET_LEN(arr) sizeof(arr) / sizeof(arr[0])

// genearate enum
typedef enum { LANG_NAMES(GEN_ENUM) } Lang;
// generate string array of languages
static const char *lang_arr[] = {LANG_NAMES(GEN_STR)};
// generate array of enum members and corresponding path
static const char *json_file_paths[] = {LANG_NAMES(GEN_PATH)};

// snippet structure as in json
typedef struct {
  const char *name;
  const char *body;
} snippet_t;

// get enum member from string value
// @lang_name = language name from argv[1]
Lang get_lang_value(const char *lang_name);

// converts word to lowercase
// @str = string to be converted to lower case
char *to_lower(const char *str);

// converts word to uppercase
// @str = string to be converted to uppercase
char *to_upper(const char *str);

// capitalize first letter of word
// @str = string to capitalie
char *to_upper_first(const char *str);

// capitalize letter of word with offset
// @str = string to capitalie
// @offset = ofset from start of str
char *to_upper_offset(const char *str, size_t offset);

// word substitution
// @format = string which is to be be modified
// @term = string which is the value which is deleted
// @val = string which is the value which get inserted
char *sub_str(const char *format, char *term, const char *val);

// format snippet body
// @format = string which is to be modified
// @inputs = array of inputs or defaults
char *format(const char *format, char **inputs);

// read file and opens content
// @language = enum value of language input by user
json_t *read_file(Lang language);

// parse json file of snippets
// @root = jsom_t value from read_file
// @snippet_name = snippet name or argv[2]
// @inputs = array of inputs or defaults
void parse_json(json_t *root, const char *snippet_name, char **inputs);

#endif
