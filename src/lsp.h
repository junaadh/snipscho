#ifndef LSP_H_
#define LSP_H_

#include <stddef.h>
#define LANG_NAMES(LANG)                                                       \
  LANG(c)                                                                      \
  LANG(cpp)                                                                    \
  LANG(react)                                                                  \
  LANG(rust)                                                                   \
  LANG(null)

#define GEN_ENUM(ENUM) ENUM,
#define GEN_STR(STR) #STR,
#define GEN_PATH(STR) [STR] = "$HOME/.config/snips/" #STR ".json",
#define GET_LEN(arr) sizeof(arr) / sizeof(arr[0])

typedef enum { LANG_NAMES(GEN_ENUM) } Lang;
static const char *lang_arr[] = {LANG_NAMES(GEN_STR)};
static const char *json_file_paths[] = {LANG_NAMES(GEN_PATH)};

typedef struct {
  const char *name;
  const char *body;
} Snippet;

Lang get_lang_value(const char *lang_name);
void parse_json(Lang language, const char *snippet_name, const char *val1,
                const char *val2);
char *to_upper_first(const char *str);
char *sub_str(const char *format, const char *term, const char *val);
char *format(const char *format, const char *val1, const char *val2);

#endif
