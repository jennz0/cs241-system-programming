This is one problem of my CS241 final assignment. Briefly speaking, it requires us to implement a program to encrypt the existing txt file using XOR binary operation and 
a random number, store the random key and the output into two new files, and use the key and output to decrypt the original file.

Below is the criteria:

Upload your code, helloworld.c; it should compile using the above clang options on a standard
CS241 VM. It will be graded on-
10 Correctly uses the program’s arguments to change behavior between hello-world, encrypt and decrypt modes basd on the process’ name.
10 Zeros the original file contents so the file is the same length but the content is destroyed 10 Random bytes are sourced from /dev/urandom
10 Implements encryption xor-protocol and generates desired output
10 Original file content can be recreated after encrypting and decrypting
10 Encrypted files are removed from the filesystem after decryption processing is complete 10 Correctly uses mmap for the input file(s)
10 Correctly uses fopen & fputc for output
10 The encrypt mode verifies input files exist before processing
10 The decrypt mode verifies input file sizes are equal before processing
