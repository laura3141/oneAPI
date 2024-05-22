#include <stdio.h>

int numeroC (char s [10],int *x){
    int qtDigitos;
    int y;
    for(int i=0;i<10;i++){
        if(s[i]=='0'||s[i]=='1'||s[i]=='2'||s[i]=='3'||s[i]=='4'||s[i]=='5'||s[i]=='6'||s[i]=='7'||s[i]=='8'||s[i]=='9')qtDigitos++;
        else if(s[i]=='A'||s[i]=='E'||s[i]=='I'||s[i]=='O'||s[i]=='U'){
            y++;
            *x=y;
        }
    }
    return qtDigitos;// por valor
}


int main(){
    char s[10]= "AaBEC19D";
    int qtDigitos=0, qtMaiusculas=0;

    printf("digitos: %d, maisculas: %d",qtDigitos,qtMaiusculas);
    qtDigitos=numeroC(s,&qtMaiusculas);
    printf("digitos: %d, maisculas: %d",qtDigitos,qtMaiusculas);
}