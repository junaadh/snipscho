# snipscho 
  - adding easier snippets to helix

  ## currently configured languages
  - [ ] bash 
  - [x] c 
  - [ ] cpp 
  - [ ] java 
  - [x] react
  - [ ] rust

## USAGE:
  ```snipscho <language> <snippet_name> <value1>? <value2>? <value3>?```      
    _value is optional and defaults to #1, #2, #3 respectively_      
    _therefore in hx can s;Enter;di and edit field_

  * in helix
    - `!snipscho <language> <snippet_name> <value1>? <value2>? <value3>?`

  * should work with any editor allowing stdout to be piped to buffer
   
## BUILD:
  ```
    git clone https://github.com/junaadh/snipscho.git
    cd snipscho
    ./build.sh
    cp ./build/snipscho ~/.local/bin
  ```        
**depends on jansson library and requires pkg-config**

## SNIPPETS:
  - snipscho looks for snippets within `$HOME/.config/snips/`
  - snippets need to be in json format with name scheme being `<language>.json`
  - json needs to be formatted as: snippets array with snippet object. Object containing name, body
    _example can be found at [dotfile/snips](https://github.com/junaadh/dotfiles/blob/mac/.config/snips)_
  - snipscho looks for #term1, #term2, #term3 and substitutes them with value 1 2 and 3 respectively
  - snipscho also looks for #TERM1, #TERM2 and #TERM3 and substitutes them witth value 1 2 and 3, but with whole word capitalized
  - newly added support for #Term1, #Term2 and #Term3, will be substituted by value 1 2 and 3 with first letter capitalized
  - language and snippet_name input is case insensitive as well as json snippet names

## ERROR CODES
  - snipscho doesnt output any diagnostics text to stderr or stdout
  - it uses exit codes
  - exit code 0 for success
  - exit code 1 means require more arguments
  - exit code 2 means input language is not configured
  - exit code 3 means error opening file
  - exit code 4 means error reading file
  - exit code 5 means error parsing json

