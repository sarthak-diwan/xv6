#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char* argv[]){
    int size = atoi(argv[1]);
    void* buff = malloc(size*sizeof(char));
    int x = draw(buff, size);
    char* b = (char*)buff;
    printf(1, "%d\n", x);
    if(x!=-1){
        for(int i=0;i<x;i++){
            printf(1, "%c", b[i]);
        }
        printf(1, "\n");
    }
    exit();
}