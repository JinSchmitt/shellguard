# Shellguard

A shell safety layer for **zsh** that catches typos before they run and warns you before dangerous commands do damage.

## What it does

When you mistype a command, Shellguard suggests the closest match and asks if you want to run it instead. When you type something destructive, it stops and asks if you are sure.

```zsh
$ gti status
Command not found. Did you mean: git status?
Run it? (y/n): y
On branch main
nothing to commit, working tree clean

$ rm -rf /
Dangerous command detected: rm -rf /
Are you sure you want to run this? (y/n): n

How it Works:
Shellguard is written in C++ and hooks into zsh to intercept commands before the shell executes them.
	•	Valid commands are stored in a trie built from your system $PATH at runtime, giving fast lookup
	•	Typo detection uses Levenshtein distance to find the closest matching command
	•	Suggestions are weighted by your shell history, so commands you use frequently rank higher than obscure matches
	•	A hardcoded list of destructive patterns triggers the danger prompt before execution
Installation:
   Requirements:
      1. macOS or Linux
      2. zsh
      3. clang++ or g++ with C++17 support
      4. CMake 3.20+

Option 1: Install with Homebrew
   brew tap JinSchmitt/shellguard
   brew install shellguard

   then add the following to your ~/.zshrc:
command_not_found_handler() {
  local suggestion
  suggestion=$(shellguard "$@" --raw)

  if [[ -n "$suggestion" ]]; then
    echo "Command not found. Did you mean: $suggestion?"
    read "reply?Run it? (y/n): "

    if [[ "$reply" == "y" || "$reply" == "Y" ]]; then
      eval "$suggestion"
      return 0
    else
      echo "zsh: command not found: $1"
      return 127
    fi
  fi

  echo "zsh: command not found: $1"
  return 127
}
reload your shell:
source ~/.zshrc
Dangerous commands

Shellguard will prompt for confirmation before running commands that match destructive patterns such as:
	•	rm -rf and variants
	•	dd
	•	mkfs
	•	chmod -R 777
	•	curl | sh
	•	wget | sh
	•	fork bombs and other destructive patterns

The list is intentionally conservative. Prompting on too many commands defeats the purpose.

