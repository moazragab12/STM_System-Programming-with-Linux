dup_share_same_file_desc.c: proves that dup() system call gets a new fd which point to the same file description.
open_get_different_file_desc.c: proves that openning the same file with same flags using 2 open() calls gets different file descriptions.
myshell.c: proves that two processes can share the same file description.
