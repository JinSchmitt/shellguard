shellguard
A shell safety layer for zsh that catches typos before they run and warns you before dangerous commands do damage.
What it does
When you mistype a command, shellguard suggests the closest match and asks if you want to run it instead. When you type something destructive, it stops and asks if you are sure.
$ gti status
Command not found. Did you mean: git status?
Run it? (y/n): y
On branch main
nothing to commit, working tree clean
$ rm -rf /
Dangerous command detected: rm -rf /
Are you sure you want to run this? (y/n): n
How it works
shellguard is written in C++ and hooks into zsh using the zle line editor, intercepting every command before the shell executes it.

Valid commands are stored in a trie built from your system $PATH at runtime, giving fast O(k) lookup where k is the length of the command
Typo detection uses Levenshtein distance to find the closest matching command
Suggestions are weighted by your shell history — commands you use frequently rank higher than obscure matches
A hardcoded list of destructive patterns triggers the danger prompt before execution

Installation
Requirements: macOS or Linux, zsh, clang++ with C++17 support, CMake 3.20+
bashgit clone https://github.com/JinSchmitt/shellguard.git
cd shellguard
mkdir build && cd build
cmake ..
make
sudo cp shellguard /usr/local/bin/
Then add the following to your ~/.zshrc:
bashshellguard_widget() {
local cmd=$BUFFER
local result=$(shellguard "$cmd")

    if [ -n "$result" ]; then
        echo ""
        echo "$result (y/n)?"
        read -k1 response
        if [[ "$response" == "y" ]]; then
            local suggested=$(shellguard "$cmd" --raw)
            BUFFER=$suggested
        else
            BUFFER=""
        fi
    fi
    zle accept-line
}

zle -N shellguard_widget
bindkey "^M" shellguard_widget
Reload your shell:
bashsource ~/.zshrc
Dangerous commands
shellguard will prompt for confirmation before running any of the following:

rm -rf and variants
dd, mkfs
chmod -R 777
curl | sh, wget | sh
Fork bombs and other destructive patterns

The list is intentionally conservative. Prompting on too many commands defeats the purpose.
Project structure
shellguard/
├── src/
│   ├── main.cpp          # Entry point, argument parsing, output
│   ├── trie.h / trie.cpp         # Trie data structure for command lookup
│   ├── corrector.h / corrector.cpp   # Levenshtein distance, history weighting, suggestions
│   └── danger.h / danger.cpp     # Dangerous command detection
└── CMakeLists.txt
Built with
C++17, CMake, zsh zleContent


# If you come from bash you might have to change your $PATH.
# export PATH=$HOME/bin:$HOME/.local/bin:/usr/local/bin:$PATH

# Path to your Oh My Zsh installation.
export ZSH="$HOME/.oh-my-zsh"

# Set name of the theme to load --- if set to "random", it will
# load a random theme each time Oh My Zpasted