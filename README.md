# snipscho 
## adding easier snippets to helix

USAGE:
  snipscho <language> <snippet_name> [value]
  _value is optional and defaults to " "_
  
BUILD:
  ```
    git clone https://github.com/junaadh/snipscho.git
    cd snipscho
    ./build.sh
    cp ./build/snipscho ~/.local/bin
  ```

SNIPPETS:
  - snipscho looks for snippets within `$HOME/.config/snips/`
  - snippets need to be in json format with name scheme being `<language>.json`
  - json needs to be formatted as: snippets array with snippet object. Object containing name, body
    _example can be found at [dotfile/snips](https://github.com/junaadh/dotfiles/blob/mac/.config/snips/c.json)_
