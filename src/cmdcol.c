#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commando.h"

void cmdcol_add(cmdcol_t *col, cmd_t *cmd){

  if(col->size>=MAX_CMDS){
    printf("ERROR. can't add any more");
  }else{
    col->cmd[col->size]=cmd;
    col->size++;
  }
}

void cmdcol_print(cmdcol_t *col){
  int j;
  printf("JOB  #PID      STAT   STR_STAT OUTB COMMAND\n");
  if(col->size > 0){
    for(int i=0; i<col->size; i++){
      printf("%d    #%d %10d %10s %4d ",i,col->cmd[i]->pid, col->cmd[i]->status,col->cmd[i]->str_status,col->cmd[i]->output_size);
      j=0;
      while(col->cmd[i]->argv[j]!=NULL){
        printf("%s ",col->cmd[i]->argv[j]);
        j++;
      }
      printf("\n");
    }
  }
}
void cmdcol_update_state(cmdcol_t *col, int block){
  // Update each cmd in col by calling cmd_update_state() which is also
  // passed the block argument (either NOBLOCK or DOBLOCK)
//## this function has a arguments about cmd_update_state function fisrt.
  for(int i=0; i<col->size; i++){
    cmd_update_state(col->cmd[i], block);
  }
}

void cmdcol_freeall(cmdcol_t *col){
// Call cmd_free() on all of the constituent cmd_t's.
  for(int i=0; i<col->size; i++){
    cmd_free(col->cmd[i]);
  }

}
