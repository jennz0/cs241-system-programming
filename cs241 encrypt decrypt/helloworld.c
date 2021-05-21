//Naifu Zheng   naifuz2
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>



#define Max_words 1024
int main (int argc, char ** argv) {
    if (strcmp((char*)argv[0],(char*)"./encrypt")!=0 && strcmp((char*)argv[0],(char*)"./decrypt")!=0) {
        printf("%s","Hello World");      //CHANGE FIRST 1 TO 0 LATER!!!
        return 0;
    }
    if (argc != 4) {
        printf("Hello World");
        return 0;
    }


    if (strcmp((char*)argv[0],(char*)"./encrypt")==0) {
        printf("%s","encrypt mode\n");

        char* content = malloc(Max_words);
        char toappend[Max_words];
        char randomData[Max_words];

        FILE *fp;
        fp = fopen("/dev/urandom", "r");
        fread(&randomData, Max_words, 1, fp);
        fclose(fp);
        

        FILE *write_ptr;
        write_ptr = fopen("1.bin","wb");
        fwrite(randomData,sizeof(randomData),1,write_ptr);
        fclose(write_ptr);

        unsigned char buffer[10];
        FILE *ptr;
        ptr = fopen("1.bin","rb");
        fread(buffer,sizeof(buffer),1,ptr); 
        fclose(ptr);

        fp = fopen(argv[1], "r+");
        struct stat filestat;
        if (stat(argv[1], &filestat)==-1) {//check existence
            printf("%s","Hello World");
            return 0;
        }
        int my_fd = open(argv[1], O_RDWR);
        size_t filesize = filestat.st_size;
        char* address = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, my_fd, 0);
        if (address == (void *)-1) {
            //perror("mmap fail!");
            puts("Hello World");
            exit(0);
        }
        while (fgets(toappend, Max_words, fp) != NULL) {
            strcat(content,toappend);
        }
        for (size_t i = 0; i < filesize; i++) {
            
            address[i] = 0;
        }
        printf("%s\n",content);
        unsigned long i;
        char output[Max_words];
        for (i=0; content[i]!='\0'; i++){//performing xor
            char temp = content[i] ^ randomData[i];
            output[i] = temp;
        }
        printf("%s\n",output);
        fclose(fp);
        write_ptr = fopen("2.bin","wb");  // w for write, b for binary
        fwrite(output,sizeof(output),1,write_ptr);

        fclose(write_ptr);
        munmap(address,filesize);
    }

    ///////////////////////////////DECRYPT BELOW///////////////////////////////////////////////////////////////////////

    if (strcmp((char*)argv[0],(char*)"./decrypt")==0) {
        printf("%s","decrypt mode\n");
        char bin1[Max_words];
        char bin2[Max_words];
        FILE * ptr;
        ptr = fopen(argv[2],"rb");
        fread(bin1,sizeof(bin1),1,ptr); 
        fclose(ptr);
        ptr = fopen(argv[3],"rb");
        fread(bin2,sizeof(bin2),1,ptr); 
        fclose(ptr);

        int fd_de1 = open(argv[2], O_RDWR);
        int fd_de2 = open(argv[3], O_RDWR);
        struct stat stat1;
        struct stat stat2;
        if(fstat(fd_de1, &stat1) !=0) {
            printf("%s","Hello World");     //check existence
            exit(0);
        } else if (fstat(fd_de2, &stat2) !=0) {
            printf("%s","Hello World");
            exit(0);
        }

        char* mpd1 = mmap(NULL, stat1.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_de1, 0);
        char* mpd2 = mmap(NULL, stat2.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_de2, 0);
        if (mpd1 == (void *)-1) {
            puts("Hello World");
            return 0;
        } else if (mpd2 == (void*)-1) {
            puts("Hello World");
            return 0;
        }


        char original[Max_words];
        for (unsigned long i=0; i<strlen(bin2); i++){
            char temp = bin2[i] ^ bin1[i];
            original[i] = temp;
        }
        ptr = fopen(argv[1],"w");  
        for (unsigned long i = 0; original[i] != '\0'; i++) {
            fputc(original[i],ptr);
        }
        fclose(ptr);
        printf("%s\n",original);
        if (remove(argv[2]) == 0 && remove(argv[3]) == 0)
            printf("%s","Deleted successfully");
        else
            printf("%s","Unable to delete the file");
        munmap(mpd1,stat1.st_size);
        munmap(mpd2,stat2.st_size);
    }
    return 0;
}