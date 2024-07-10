# To compile the program that uses crypt()
gcc -o check_password  check_password.c  -lcrypt
# We should change the owner to the root and set the set-user-ID bit to run the process with the file owner permissions
sudo chown root check_password
sudo chmod 4775 check_password
