#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
   int i;
      pid_t ids[10];
      for ( i = 0; i < 10; ++i){
               pid_t id = fork();
               if (id == 0){
                           //child
                                  printf("child process created successfully...PID:%d\n", getpid());
                                           sleep(3);
                                                    exit(i); //将子进程的退出码设置为该子进程PID在数组ids中的下标
                                                         
               }
                     //father
                         ids[i] = id;
                     
      }
      for ( i = 0; i < 10; ++i){
               int status = 0;
                     pid_t ret = waitpid(ids[i], &status, 0);
                     if (ret >= 0){
                                 //wait child success
                                        printf("wiat child success..PID:%d\n", ids[i]);
                                                 if (WIFEXITED(status)){
                                 //                            //exit normal
                                                                         printf("exit code:%d\n", WEXITSTATUS(status));
                                                                                  }
                                                                                           else{
                                                                                                       //signal killed
                                                                                                                   printf("killed by signal %d\n", status & 0x7F);
                                                                                                                         }
                                                                                                                                  }
                                                                                                                                     }
                                                                                                                                        return 0;
                                                                                                                                        }
                                 
                                 
