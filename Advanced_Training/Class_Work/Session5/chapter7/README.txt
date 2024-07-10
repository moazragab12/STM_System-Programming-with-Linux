ASLR (Address Space Layout Randomization)
https://docs.oracle.com/en/operating-systems/oracle-linux/6/security/ol_aslr_sec.html#:~:text=ASLR%20can%20locate%20the%20base,%2Fsys%2Fkernel%2Frandomize_va_space%20.

To disable randomization:
echo 0 > /proc/sys/kernel/randomize_va_space

To enable it again:
echo 2 > /proc/sys/kernel/randomize_va_space

------------------------------------------------

To compile the program and generate its map (static linking):
gcc  -Wl,-Map=output.map free_and_sbrk.c --static


To check the memory mapping of the process:
cat /proc/`pidof a.out`/maps


