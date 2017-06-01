#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "command.h"
#include "getcommand.h"
#include "util.h"

#define PROMPT "->"
#define DEBUG 0

/* A switch that will state if an error occured. */
int error_occured;

static int iscd(commandlist *clist);

/* Signal handler for SIGINT handling.
 * Childs will be killed by SIGINT.
 * Parent only has to care about not forking further childs.
 */
void sig_handler(int signum) { error_occured = 1; }

/* Install Signalhandling of SIGINT
 * SA_RESTART is needed to make sure that wait for child will be
 * restarted after receiving a Signal.
 */
void init_signals() {
        struct sigaction newHandler;
        sigset_t block;
        sigfillset(&block);

        newHandler.sa_handler = sig_handler;
        newHandler.sa_flags = SA_RESTART;
        newHandler.sa_mask = block;

        if ( sigaction(SIGINT, &newHandler, NULL) == -1 ){
                perror("sigaction");
                exit(1);
        }
}

int main(void) {                                                                //MAIN
   commandlist *clist;
   pid_t parent = getpid();
   int ret;

   init_signals();

   while (1) {                                                                  // MAIN LOOP
      error_occured = 0;
      printf("%s ", PROMPT);
      fflush(stdout);
      clist = getcommandlist(stdin);

      if (clist == NULL) {                                                      // CLIST NULL
         if (feof(stdin)) {
            break;
         }
      }                                                                         // END CLIST NULL
      else {                                                                    // CLIST NOT NULL
#if DEBUG
         print_commandlist(clist);
#endif
         if (valid_commandlist(clist)) {                                        // VALID COMMAND LIST
            if (iscd(clist)) {                                                  // CD COMMAND
               ret = chdir(clist->head->args->head->str);
               if (ret != 0) fprintf(stderr, "%s\n", strerror(errno));
            }
            else {                                                              // OTHER COMMAND
               pid_t pid;
               command *cur = clist->head;
               int   fd[2];

               // The input to use (will be overwritten on input redirect)
               int   fd_in = 0;
               // The output to use (will be overwritten on output redirect)
               int   fd_out = 1;

               if (clist->tail->in != NULL) {                                   // OVERWRITE INPUT
                   fd_in = open(clist->tail->in, O_RDONLY);
                   if (fd_in == -1)
                      fprintf(stderr, "%s: %s\n", clist->tail->in,
                         strerror(errno));
               }
               if (clist->tail->out != NULL) {                                  // OVERWRITE OUTPUT
                   fd_out = open(clist->tail->out, O_RDWR |
                      O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
                   if (fd_out == -1)
                      fprintf(stderr, "%s: %s\n", clist->tail->out,
                         strerror(errno));
               }

               while (cur != NULL && error_occured == 0) {                      // ITERATE OVER COMMANDS
                  pipe(fd);
                  if ((pid = fork()) == -1)                                     // FORK ERROR
                     fprintf(stderr, "%s\n", strerror(errno));

                  else if (pid == 0) {                                          // CHILD
                     dup2(fd_in, 0);

                     // If last elment pipe to defined output
                     if (cur->next_one != NULL)
                        dup2(fd[1], 1);
                     else if (fd_out != 1)
                        dup2(fd_out, 1);
                     close(fd[0]);

                     // Create char pointer array out of command
                     char** command =
                        (char **) malloc((cur->args->len + 1) * sizeof(char*));
                     command[0] = cur->cmd;
                     int index = 1;
                     struct listnode *arg;
                     for (arg = cur->args->head; arg != NULL; arg = arg->next,
                                                              ++index)
                        command[index] = arg->str;

                     // Execute command
                     ret = execvp(command[0], command);

                     // Error handling
                     fprintf(stderr, "%s: %s\n", command[0], strerror(errno));
                     free(command);
                     kill(parent, SIGINT);
                     exit(1);
                  }                                                             // END CHILD

                  else {                                                        // PARENT
                     while (wait(NULL) > 128);
                     close(fd[1]);
                     //save the input for the next command
                     fd_in = fd[0];
                     cur = cur->next_one;
                  }                                                             // END PARENT
               }                                                                // END ITERATE OVER COMMANDS
            }                                                                   // END OTHER COMMAND
         }                                                                      // END VALID COMMAND LIST
         delete_commandlist(clist);
         free(clist);
      }                                                                         // END CLIST NOT NULL
   }                                                                            // END MAIN LOOP
   return 0;
}                                                                               // END MAIN

static int iscd(commandlist *clist) {
   return clist != NULL && clist->head != NULL && clist->head->cmd != NULL
      && (strcmp(clist->head->cmd, "cd") == 0);
}
