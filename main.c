#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

sem_t quai;
char *meteo_now[1];

pthread_cond_t condition_poids = PTHREAD_COND_INITIALIZER; /* Création de la condition */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /* Création du mutex */

void *Meteo(void *arg){ //gere meteo

    char *tab_meteo[5] = {"Neige", "Vent", "Soleil", "Brouillard", "Pluie"};
    srand(time(NULL)); //permet de vrai valeur aléatoire
    while(1)
    {
        meteo_now[0] = tab_meteo[rand()%5];
        printf("Changement de la meteo pour : %s\n", meteo_now[0]);
        sleep(4);
    }
    return 0;
}

void *Camion(void *arg){ // thread camion

    sleep(1); //evite les erreurs

    printf("le camion %u attend le quai \n", (unsigned int)pthread_self());

    /* utilisation du semaphore quai */
    sem_wait(&quai);
        printf("  le camion %u se fait charger \n", (unsigned int)pthread_self());
        sleep(2);
    sem_post(&quai);
    /* fin d'utilisation du semaphore quai */

    printf("  le camion %u est charge \n", (unsigned int)pthread_self());
    printf("le camion %u attend le pesage \n", (unsigned int)pthread_self());

    /* utilisation du mutex pesage */
    pthread_mutex_lock (&mutex); /* On verrouille le mutex */
    pthread_cond_signal (&condition_poids); /* On délivre le signal : condition remplie */
    pthread_cond_wait(&condition_poids, &mutex);

        printf("pesage du camion %u en cours\n", (unsigned int)pthread_self());
            sleep(4);
        srand(time(NULL)); //pour l'aléatoire

        printf("    le camion %u pese %d Kg \n", (unsigned int)pthread_self(), (rand()%(5001)+1000));
        printf("    meteo du camion %u : %s\n", (unsigned int)pthread_self(), meteo_now[0]);
        printf("    le camion %u part \n", (unsigned int)pthread_self());

    pthread_cond_broadcast(&condition_poids);
    pthread_mutex_unlock (&mutex); /* On déverrouille le mutex */
    /* fin d'utilisation du mutex pesage */

    pthread_exit(NULL); //fin du thread

    return 0;
}

int main(void)
{

    sem_init(&quai, 0, 2);  //initialisation du semaphore quai

    pthread_t camion1;
    pthread_t camion2;
    pthread_t camion3;
    pthread_t camion4;
    pthread_t meteo;

    pthread_create (&camion1, NULL, Camion, NULL);
    pthread_create (&camion2, NULL, Camion, NULL);
    pthread_create (&camion3, NULL, Camion, NULL);
    pthread_create (&camion4, NULL, Camion, NULL);
    pthread_create (&meteo, NULL, Meteo, NULL); //meteo a cree en dernier pour les id

    //le programme s'arrete quand les threads camion ont fini
    #if 1
    pthread_join (camion1, NULL);
    pthread_join (camion2, NULL);
    pthread_join (camion3, NULL);
    pthread_join (camion4, NULL);
    #endif

    return 0;
}
