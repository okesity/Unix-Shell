# Unix-Shell

This is a Unix shell like "bash" called "nush". It can operates as follows:

1. Accept an optional script file as the first command line argument.
2. If there's no script, show a prompt. Your prompt should be "nush$ ". Command input is accepted on the same line after the prompt.
3. Read one line of commands from either stdin or the script.
4. Execute that line of commands.
5. Repeat until EOF or an "exit" command.

### It supports nine operators:

- Redirect input: `sort < foo.txt`
- Redirect output: `sort foo.txt > output.txt`
- Pipe: `sort foo.txt | uniq`
- Background: `sleep 10 &`
- And: `true && echo one`
- Or: `true || echo one`
- Semicolon: `echo one; echo two`
- Variable Assignment: `FOO=one`
- Variable Use: `echo $FOO`

### and some built in commands:

- "cd": changes the current directory. 
- "exit": exits the shell.

### and also some other important features:

- Subshell: `((sleep 3) && echo one)`
- Quotes: `echo ";)echo one"`
- Newline: 
    ```
    echo \  
    one
    ```
