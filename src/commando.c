#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "commando.h"
#include <sys/wait.h>



int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0); // Turn off output buffering
  int echo=0;
  if(argc>1 && strcmp("-echo",argv[1])==0){
    echo=1;
  }
  char* ctbuf = malloc(sizeof(char)*MAX_LINE);
  // char* tokens[ARG_MAX];
  char* tokens[ARG_MAX];
  int ntok, job=0, job_done_update=0;

  cmd_t* tcmd = malloc(sizeof(cmd_t));
  cmdcol_t* tcol = malloc(sizeof(cmdcol_t));
  tcol->size=0;
  int size = tcol->size;
  //cmdcol_update_state();


  while(1){
    printf("@> ");
    fgets(ctbuf,MAX_LINE,stdin);
    parse_into_tokens(ctbuf, tokens, &ntok);
    //If there are no more inputs and exit was not used:
    if(feof(stdin)){
      printf("\nEnd of input");
      free(ctbuf);
      while(ntok != -1){                               // free argv from the last
        tokens[ntok+1] = NULL;
        ntok--;
      }
      free(tcol);
      free(tcmd);
      break;
    }
    if(tokens[0] == NULL){            // if user press enter
      printf("\n");
    }
    else if(strncmp(tokens[0],"help", MAX_LINE) == 0){
      if(echo==1){
        printf("help\n");
      }
      printf("help\n");
      printf("COMMANDO COMMANDS\n");
      printf("help               : show this message\n");
      printf("exit               : exit the program\n");
      printf("list               : list all jobs that have been started giving information on each\n");
      printf("pause nanos secs   : pause for the given number of nanseconds and seconds\n");
      printf("output-for int     : print the output for given job number\n");
      printf("output-all         : print output for all jobs\n");
      printf("wait-for int       : wait until the given job number finishes\n");
      printf("wait-all           : wait for all jobs to finish\n");
      printf("command arg1 ...   : non-built-in is run as a job\n");
    }else if(strncmp(tokens[0],"list",MAX_LINE)==0){
      if(echo==1){
        printf("list\n");
      }
      printf("list\n");
      cmdcol_print(tcol);
    }else if(strncmp(ctbuf,"\r\n",MAX_LINE)==0){
      if(echo==1){
        printf("\n");
      }
    }
    else if(strncmp(tokens[0], "pause", MAX_LINE)==0){
      printf("pause %s %s \n", tokens[1], tokens[2]);
      pause_for(atoi(tokens[1]), atoi(tokens[2]));
       for(int i=job_done_update; i<size; i++){
         // cmd_update_state(tcol->cmd[i], NOBLOCK);
         if(tcol->cmd[i]->finished != 1){
           cmd_update_state(tcol->cmd[i], NOBLOCK);
         }
         job_done_update++;
      }

    }
    else if (strncmp(tokens[0], "output-for", MAX_LINE)==0){
      job = atoi(tokens[1]);
      printf("output-for %s \n", tokens[1]);
      printf("@<<< Output for %s[#%d] (%d bytes):\n", tcol->cmd[job]->name, tcol->cmd[job]->pid, tcol->cmd[job]->output_size);
      printf("----------------------------------------\n");
      cmd_print_output(tcol->cmd[job]);
      printf("----------------------------------------\n");
    }
    else if(strncmp(tokens[0], "output-all", MAX_LINE)==0){
      printf("output-all\n");
      for(int i=0; i<size; i++){
      printf("@<<< Output for %s[#%d] (%d bytes):\n", tcol->cmd[i]->name, tcol->cmd[i]->pid, tcol->cmd[i]->output_size);
      printf("----------------------------------------\n");
      cmd_print_output(tcol->cmd[i]);
      printf("----------------------------------------\n");
      }
    }
    else if(strncmp(tokens[0], "wait-for", MAX_LINE)==0){
      job = atoi(tokens[1]);
      printf("wait-for %s \n", tokens[1]);
       if(tcol->cmd[job]->finished != 1){
        cmd_update_state(tcol->cmd[job], DOBLOCK);
       }
    }
    else if(strncmp(tokens[0], "wait-all", MAX_LINE)==0){
      printf("wait-all\n");
      for(int i=0; i<size; i++){
        if(tcol->cmd[i]->finished != 1){
        cmd_update_state(tcol->cmd[i], DOBLOCK);
      }
      }
    }
    else if(strncmp(tokens[0], "exit", MAX_LINE)==0){
      if(echo==1){
        printf("exit\n");
      }
      printf("exit\n");
      free(ctbuf);
      while(ntok != -2){                               // free argv from the last
        tokens[ntok+1] = NULL;
        ntok--;
      }
      // cmdcol_freeall(tcol);

      free(tcmd);
      cmdcol_freeall(tcol);
      free(tcol);
      break;
    }
    else{
      for(int i=0; i<ntok; i++){
        printf("%s ", tokens[i]);
      }
      printf("\n");
      cmd_t *new = cmd_new(tokens); // set new cmd_t with the given commands
      cmd_start(new);
      cmdcol_add(tcol, new);        // add new cmd_t into cmdcol

      size++;
    }
     for(int i=0; i<size; i++){
     if(tcol->cmd[i]->finished != 1){
        cmd_update_state(tcol->cmd[i], NOBLOCK);
     }
     }
    }
    exit(1);

  }
