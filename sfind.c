//sfind: versão simplificada de find

/*Criar um código que recrie a acção do comand find(), o qual vai vasculhar o diretório actual por ficheiros, a ver se encontra aqueles que cumpram a condição que é pedida.
Depois de procurar por ficheiros, se houver um sub-diretório nesse diretório, vai repetir este processo para ele e para os demais que houverem.
Depois de encontrado um ficheiro, deve executar as acções print, delete e exec command sobre este.
Quando o utilizador carrega em CTRL+C, o programa interrompe qualquer operação e pergunta ao utilizador se quer mesmo interromper. Caso seja sim (Y ou y) o programa termina, caso não (N ou n) o programa prossegue. */

#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <stdio.h> 
#include <string.h> 

void sig_handler(int signo) 
{ 
    char option = 'a';
    printf("\nAre you sure you want to exit? (Y/N)\n");
    scanf("%c", &option); 
    if (option == 'Y' || option == 'y')
        exit(1);
}

void listdir(const char *name, int level)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            printf("%*s[%s]\n", level*2, "", entry->d_name);
            listdir(path, level + 1);
        }
        else
            printf("%*s- %s\n", level*2, "", entry->d_name);
    } while (entry = readdir(dir));
    closedir(dir);
}

int main(int argc, char *argv[])
{
    struct sigaction action; 
    action.sa_handler = sig_handler; 
    sigemptyset(&action.sa_mask); 
    action.sa_flags = 0; 
    if (sigaction(SIGINT,&action,NULL) < 0) 
    { 
        fprintf(stderr,"Unable to install SIGINT handler\n"); 
        exit(1); 
    } 
    if (sigaction(SIGTERM,&action,NULL) < 0) 
    { 
        fprintf(stderr,"Unable to install SIGTERM handler\n"); 
        exit(1); 
    }

    int i;
    char *path, *name;
    char type;
    int perm;
    
    for (i = 2; i < argc; i+=2)
    {
        if (strcmp(argv[i-1], "-path") == 0)
            path = argv[i];
        else if (strcmp(argv[i-1], "-name") == 0)
            name = argv[i];
        else if (strcmp(argv[i-1], "-type") == 0)
            type = argv[i][0];
        else if (strcmp(argv[i-1], "-perm") == 0)
            perm = atoi(argv[i]);
        else
        {
            printf("invalid argument %s\n", argv[i]);
            exit(1);
        }
    }
    printf("name: %s\n", name);
    printf("path: %s\n", path);
    printf("type: %c\n", type);
    printf("perm: %d\n", perm);
    //listdir(path, 0);
    return 0;
}
