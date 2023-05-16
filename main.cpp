#include "user.h"
#include "train.h"
#include "ticket.h"
#include <cstdio>
#include <cstring>
#include <cmath>

bool get_str(char *s){
    char c = getchar(); int pos = 0;
    while(c == ' ') c = getchar();
    if(c == '\n') return 0;
    while(c != ' ' && c != '\n') s[pos ++] = c, c = getchar();
    s[pos] = 0;
    return c != '\n';
}

int main(){
    scanf("%d")
    return 0;
}