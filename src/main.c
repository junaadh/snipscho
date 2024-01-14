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
    return res;
  }
  return res;
}

char *to_upper(const char *str) {
  char *res = strdup(str);
  if (res != NULL) {
    for (int i = 0; res[i] != '\0'; ++i) {
      res[i] = toupper((unsigned char)res[i]);
    }
    return res;
  }
  return res;
}

// capitalize first letter
char *to_upper_first(const char *str) {
  char *res = strdup(str);
  if (str != NULL && *str != '\0') {
    res[0] = toupper((unsigned char)res[0]);
    return res;
  } else {
    return res;
  }
}

// substitute string
char *sub_str(const char *format, const char *term, const char *val) {
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
  // strcat(result, tmp);
  free(result);
  return tmp;
}

// format string
char *format(const char *format, const char **inputs) {
  char *result = strdup(format);
  char *simp[] = {"#term1", "#term2", "#term3"};
  char *fcap[] = {"#Term1", "#Term2", "#Term3"};
  char *caps[] = {"#TERM1", "#TERM2", "#TERM3"};

  for (size_t i = 0; i < 3; ++i) {
    result = sub_str(result, simp[i], inputs[i]);
    inputs[i] = to_upper_first(inputs[i]);
    result = sub_str(result, fcap[i], inputs[i]);
    inputs[i] = to_upper(inputs[i]);
    result = sub_str(result, caps[i], inputs[i]);
  }

  return result;
}

// parse json
void parse_json(Lang language, const char *snippet_name, const char **inputs) {
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
  // printf("%s\n", to_upper(argv[1]));
  // return 0;
  if (argc < 3) {
    exit(1);
  }

  const char *lang = to_lower(argv[1]);
  const char *snippet = to_lower(argv[2]);
  const char *inputs[3] = {"#1", "#2", "#3"};
  switch (argc) {
  case 6:
    inputs[2] = argv[5];
  case 5:
    inputs[1] = argv[4];
  case 4:
    inputs[0] = argv[3];
    break;
  default:
    break;
  }
  // printf("%s\n%s\n%s\n%s\n", lang, snippet, input1, input2);
  // return 0;

  Lang language_enum = get_lang_value(lang);
  if (language_enum == null) {
    exit(2);
  }

  // const char *path = expand_path(json_file_paths[language_enum]);
  // printf("%s\n%s\n", path, expand_path(path));
  parse_json(language_enum, snippet, inputs);

  return 0;
}
