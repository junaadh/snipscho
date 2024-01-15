#include "lsp.h"
#include <ctype.h>
#include <jansson.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>

// length of language string array gives number of languages
#define len GET_LEN(lang_arr)
#define in_len 3

// expand $HOME as on terminal
char *expand_path(const char *path) {
  wordexp_t res;
  wordexp(path, &res, 0);
  return res.we_wordv[0];
}

// get enum member corresponding to string input
Lang get_lang_value(const char *lang_name) {
  const char **lang = lang_arr;
  for (size_t i = 0; i < len; ++i) {
    if (strcmp(lang_name, lang[i]) == 0) {
      return (Lang)i;
    }
  }

  return null;
}

char *to_lower(const char *str) {
  char *res = strdup(str);
  if (res != NULL) {
    for (int i = 0; res[i] != '\0'; ++i) {
      res[i] = tolower((unsigned char)res[i]);
    }
  }
  return res;
}

char *to_upper(const char *str) {
  char *res = strdup(str);
  if (res != NULL) {
    for (int i = 0; res[i] != '\0'; ++i) {
      res[i] = toupper((unsigned char)res[i]);
    }
  }
  return res;
}

// capitalize first letter
char *to_upper_first(const char *str) {
  char *res = strdup(str);
  if (res != NULL && res[0] != '\0') {
    res[0] = toupper((unsigned char)res[0]);
  }
  return res;
}

char *to_upper_offset(const char *str, size_t offset) {
  char *res = strdup(str);
  if (res != NULL && res[offset] != '\0') {
    res[offset] = toupper((unsigned char)res[offset]);
  }
  return res;
}

// substitute string
char *sub_str(const char *format, char *term, const char *val) {
  const char *placeholder = strstr(format, term);
  if (placeholder == NULL) {
    return strdup(format);
  }

  size_t resultlen = strlen(format) - strlen(term) + strlen(val) + 1;
  char *result = (char *)malloc(resultlen);

  strncpy(result, format, placeholder - format);
  result[placeholder - format] = '\0';
  strcat(result, val);
  strcat(result, placeholder + strlen(term));

  char *tmp = sub_str(result, term, val);
  free(result);
  return tmp;
}

// format string
char *format(const char *format, char **inputs) {
  char *result = strdup(format);
  char search_buf[7];
  char *search_term = "#term%zu";

  for (size_t i = 0; i < in_len; ++i) {
    sprintf(search_buf, search_term, i + 1);
    result = sub_str(result, search_buf, inputs[i]);
    inputs[i] = to_upper_first(inputs[i]);
    result = sub_str(result, to_upper_offset(search_buf, 1), inputs[i]);
    inputs[i] = to_upper(inputs[i]);
    result = sub_str(result, to_upper(search_buf), inputs[i]);
  }

  return result;
}

// parse json
void parse_json(Lang language, const char *snippet_name, char **inputs) {
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

  // size_t snippets_found = 0;
  json_t *snippets_array = json_object_get(root, "snippets");
  if (json_is_array(snippets_array)) {
    size_t array_size = json_array_size(snippets_array);
    for (size_t i = 0; i < array_size; ++i) {
      json_t *snippet = json_array_get(snippets_array, i);
      if (json_is_object(snippet)) {
        // ++i;
        const char *name =
            to_lower(json_string_value(json_object_get(snippet, "name")));
        const char *body = json_string_value(json_object_get(snippet, "body"));
        // // printf("%s\n%s\n", name, body);
        // // ++snippets_found;
        if (name != NULL && strcmp(name, snippet_name) == 0) {
          const char *subbed = format(body, inputs);
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

  char *inputs[in_len + 2] = {"", "", "#1", "#2", "#3"};
  for (size_t i = 0; i < (size_t)argc - 1 && i < in_len + 2; ++i) {
    inputs[i] = argv[i + 1];
  }

  Lang language_enum = get_lang_value(to_lower(inputs[0]));
  if (language_enum == null) {
    exit(2);
  }

  parse_json(language_enum, to_lower(inputs[1]), inputs + 2);

  return 0;
}
