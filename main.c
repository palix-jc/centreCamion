#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *F_thread(void *arg){
    char *tab[5] = {"Neige", "Vent", "Soleil", "Brouillard", "Pluie"};
    //char meteo ;
    srand(time(NULL));
    for (int i = 0 ; i < 20 ; i++)
    {
        //meteo = tab[rand() % 5];
        printf("%s\n", tab[rand() % 5]);
        //printf("%s\n", meteo);
        sleep(1);
    }
    return 0;
}

int main(void)
{
    pthread_t t1;
    pthread_create (&t1, NULL, F_thread, NULL);


    sleep(40);
    return 0;
}
