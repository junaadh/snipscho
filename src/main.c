#include "lsp.h"
#include <jansson.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>

#define len GET_LEN(lang_arr)

char *expand_path(const char *path) {
  wordexp_t res;
  wordexp(path, &res, 0);
  return res.we_wordv[0];
}

Lang get_lang_value(const char *lang_name) {
  const char **lang = lang_arr;
  for (size_t i = 0; i < len; ++i) {
    if (strcmp(lang_name, lang[i]) == 0) {
      return (Lang)i;
    }
  }

  return null;
}

char *sub_str(const char *format, const char *val) {
  const char *placeholder = strstr(format, "%s");
  if (placeholder == NULL) {
    return strdup(format);
  }

  size_t resultlen = strlen(format) - 2 + strlen(val) + 1;
  char *result = (char *)malloc(resultlen);

  strncpy(result, format, placeholder - format);
  result[placeholder - format] = '\0';
  strcat(result, val);
  strcat(result, placeholder + 2);
  return result;
}

void parse_json(Lang language, const char *snippet_name, const char *value) {
  const char *path = expand_path(json_file_paths[language]);

  FILE *file = fopen(path, "r");
  if (!file) {
    free((void *)path);
    exit(3);
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *json_content = (char *)malloc(file_size + 1);
  if (!json_content) {
    fclose(file);
    free((void *)path);
    exit(4);
  }

  fread(json_content, 1, file_size, file);
  fclose(file);
  json_content[file_size] = '\0';

  // printf("%s", json_content);

  json_error_t error;
  json_t *root = json_loads(json_content, 0, &error);
  if (!root) {
    free(json_content);
    free((void *)path);
    exit(5);
  }

  // int snippets_found = 0;
  json_t *snippets_array = json_object_get(root, "snippets");
  if (json_is_array(snippets_array)) {
    size_t array_size = json_array_size(snippets_array);
    for (size_t i = 0; i < array_size; ++i) {
      json_t *snippet = json_array_get(snippets_array, i);
      if (json_is_object(snippet)) {
        const char *name = json_string_value(json_object_get(snippet, "name"));
        const char *body = json_string_value(json_object_get(snippet, "body"));

        // printf("%s\n%s\n", name, body);
        // ++snippets_found;
        if (name != NULL && strcmp(name, snippet_name) == 0) {
          const char *subbed = sub_str(body, value);
          printf("%s", subbed);
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    exit(1);
  }

  const char *lang = argv[1];
  const char *snippet = argv[2];
  const char *input = argc >= 4 ? argv[3] : " ";
  // printf("%s\n%s\n%s\n", lang, snippet, input);
  Lang language_enum = get_lang_value(lang);
  // if (language_enum == null) {
  //   exit(2);
  // }
  if (language_enum == null) {
    exit(2);
  }

  // const char *path = expand_path(json_file_paths[language_enum]);
  // printf("%s\n%s\n", path, expand_path(path));
  parse_json(language_enum, snippet, input);

  return 0;
}
