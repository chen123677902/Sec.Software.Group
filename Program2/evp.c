/*
 *   Compilation: gcc -o EVP EVP.c -static -lcrypto
 *
 *     Invocation: EVP file [hash]
 *
 *     */
        
#include <stdio.h>
#include <openssl/evp.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
   EVP_MD_CTX    mdctx;
   const EVP_MD  *md;
   unsigned char md_value[EVP_MAX_MD_SIZE];
   int           md_len;
   int           i;
   struct stat   fileStat;
   int           fd;
   char          *buf;

        fd=open(argv[1],O_RDONLY);
      
        fstat(fd,&fileStat);
        buf=malloc(fileStat.st_size);
        read(fd,buf,fileStat.st_size);
        
        EVP_MD_CTX_init(&mdctx);

        EVP_DigestInit_ex(&mdctx, EVP_sha1(), NULL);

        /* Demonstrate that we can do this in pieces */
        if (fileStat.st_size > 100){  
             EVP_DigestUpdate(&mdctx, buf, 100);
             EVP_DigestUpdate(&mdctx, buf+100, fileStat.st_size-100);
             printf("Splitting file.\n");
       	 } else {
             EVP_DigestUpdate(&mdctx, buf, fileStat.st_size);
	 }

        EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
        EVP_MD_CTX_cleanup(&mdctx);

        printf("Digest is: ");
        for(i = 0; i < md_len; i++) printf("%02x", md_value[i]);
        printf("\n");
        }
