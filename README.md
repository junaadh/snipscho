# snipscho 
  - adding easier snippets to helix

  ## currently configured languages
    [x] c 
    [x] cpp 
    [x] react
    [x] rust

## USAGE:
  ```snipscho <language> <snippet_name> <value1>? <value2>?```      
    _value is optional and defaults to " "_

  * in helix
    - `!snipscho <language> <snippet_name> <value1>? <value2>`

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
    _example can be found at [dotfile/snips](https://github.com/junaadh/dotfiles/blob/mac/.config/snips/c.json)_
  - snipscho looks for #term1, #term2 and substitutes them with value 1 and 2 respectively
  - snipscho also looks for #TERM1 and #TERM2 and substitutes them witth value 1 and 2, but with first letter capitalized
  - language and snippet_name input is case insensitive as well as json snippet names

### example screenshot
  <img width="714" alt="example in terminal" src="https://github.com/junaadh/snipscho/assets/67371703/bf3f8fec-a3ee-48fb-b3a7-977f05f040f6">
  <img width="1394" alt="example snippet config" src="https://github.com/junaadh/snipscho/assets/67371703/6ed7c374-78f3-4340-969c-17158ccdddc1">


