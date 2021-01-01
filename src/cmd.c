



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commando.h"

cmd_t *cmd_new(char *argv[]){
  char* init = "INIT";
  cmd_t *new = malloc(sizeof(cmd_t));                //allocates new cmd_t
  int i;                                            // to get size of the argv[]
  for(i=0; i<=ARG_MAX-1 && argv[i] != NULL; i++){   //copy the strings form argv into the new cmd
    new->argv[i] = strdup(argv[i]);
  }
  strcpy(new->name, argv[0]);                     //copy argv[0] into the cmd name
  new->argv[i] = NULL;                            //set NULL to the end of the array by using the size
  new->finished = 0;                              //set other data as default
  snprintf(new->str_status, 5, "%s", init);
  new->pid = -1;
  new->status = -1;
  new->output = NULL;
  new->output_size = -1;
  new->out_pipe[0] = -1;
  new->out_pipe[1] = -1;
  return new;

}

void cmd_free(cmd_t *cmd){
  int b = 0;
  while(cmd->argv[b] != NULL){                  // get the size of argv[]
    b++;
  }
  while(b != -1){                               // free argv from the last
    free(cmd->argv[b]);
    b--;
  }
  if(cmd->output != NULL){                      //deallocate cmd output
    free(cmd->output);
  }
  free(cmd);                                    //deallocate cmd itself
}

void cmd_start(cmd_t *cmd){
  int pipe_result = pipe(cmd->out_pipe);           // create pipe
  char* run = "RUN";
  snprintf(cmd->str_status, 4, "%s", run);         // set str_status to RUN
  pid_t child = fork();                            // fork process
  cmd->pid = child;                                //set the parent pid as child pid
  if(pipe_result != 0){                            //error case
    perror("Error");
  }
  if(child == 0){                                  //child parameter
    dup2(cmd->out_pipe[PWRITE], STDOUT_FILENO);    //to start giving the output into the pipe without showing up on screen
    execvp(cmd->name, cmd->argv);
    close(cmd->out_pipe[PREAD]);                   //close the pipe
    exit(0);
  }
    close(cmd->out_pipe[PWRITE]); //close the pipe    //parent parameter
}

void cmd_update_state(cmd_t *cmd, int block){
  if(cmd->finished == 1){               
    exit(0);
  }
  else{                                   //to set whether the process is finished or not
    int status;
    pid_t wpid = waitpid(cmd->pid, &status, block);
    if(wpid != 0){
    if(WIFEXITED(status)){
      int exit_status = WEXITSTATUS(status);
      cmd->finished = 1;
      cmd->status = exit_status;
      sprintf(cmd->str_status, "EXIT(%d)", cmd->status);
    }
    cmd_fetch_output(cmd);                // to set output
    }
    if(cmd->finished == 1){
    printf("@!!! %s[#%d]: EXIT(%d)\n", cmd->name, cmd->pid, cmd->status);
    }
  }
}

char *read_all(int fd, int *nread){
  int max_size = 4, cur_pos = 0;
  int readed=0;
  char *buf = malloc(max_size*sizeof(char)); // allocate memory
  while(1){
    if(cur_pos >= max_size){                  // if already reached max size, reallocate the size
      max_size = max_size * 2;
      buf = realloc(buf, max_size);
    }

    int max_read = max_size - cur_pos;        // calculate maximum read size
    int byte_read = read (fd, buf+cur_pos, max_read); // do read()

    if(byte_read != 0){
      cur_pos += byte_read;
    }
    if(byte_read == 0){
      readed = cur_pos;
      buf[cur_pos] = '\0';                  // set the last array as NULL
      break;
    }
    else if(byte_read == -1){               // if reading fail, error case
      perror("read failed");
      exit(1);
    }
  }
  *nread = readed;
  return buf;
}

void cmd_fetch_output(cmd_t *cmd){
  if(cmd->finished == 0){
    printf("%s [#%d] not finished yet\n", cmd->name, cmd->pid); //error with format ls[#12341] not finished yet
  }
  else{
    int readed;
    cmd->output = read_all(cmd->out_pipe[PREAD], &readed);      // set output pointer to the data that is being read
    cmd->output_size = readed;
    close(cmd->out_pipe[PREAD]); //close the pipe associated with the command after reading all input
  }
}

void cmd_print_output(cmd_t *cmd){
   if(cmd->output == NULL){          // output not ready case
    printf("%s[#%d] : output not ready\n", cmd->name, cmd->pid);
  }
  else{           // output ready, print out the data
    printf("%s", (char *) cmd->output);
  }
}






