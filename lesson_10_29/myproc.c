#include <stdio.h>
int main()
{
      FILE* fp = fopen("log.txt", "r");
      if (fp == NULL){
               perror("fopen");
                     return 1;
                        
      }
         char buffer[64];
         int i;
         for (i = 0; i < 5; ++i){
                  fgets(buffer, sizeof(buffer), fp);
                        printf("%s", buffer);
                           
         }
            fclose(fp);
               return 0;

}

