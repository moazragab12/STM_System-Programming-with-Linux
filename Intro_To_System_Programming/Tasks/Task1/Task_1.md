# Task_1 README
## Counting Lines in /etc/group
To count the number of lines in file use the following command:
```
wc -l FileName
```
### example
```
wc -l /etc/group
```
This command uses the ` wc` (word count) command with the ` -l` option to count the number of lines in the specified file.

## Asking Before remove
it is a safety option in `rm` command you can use `-i` option to be asked before every remove
For example:
```
rm -i file
```
## Alias
In a command-line interface, an alias serves as a personalized abbreviation or replacement for a lengthier command. It empowers users to create their own commands or adjust existing ones for ease of use. Typically, aliases are established within configuration files such as `.bashrc `or `.bash_profile.`


## List All Current Aliases
To list all current aliases in the Bash shell, use the following command:
```
alias
```

## Create an Alias for 'rm -i'
To create an alias for 'rm -i' , use the following command:
```
alias rmi='rm -i'
```
This alias named 'rmi' is replaced by  a command  'rm -i'

