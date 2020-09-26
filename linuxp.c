#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define MaxItems 5 
#define BufferSize 5 

sem_t zero;
sem_t complete;
int receive = 0;
int send = 0;
int buff[BufferSize];
pthread_mutex_t mutex;

void *manufacturer(void *mno)
{   
    int product;
    for(int i = 0; i < MaxItems; i++) 
    {
        product = rand(); 
        sem_wait(&zero);
        pthread_mutex_lock(&mutex);
        buff[receive] = product;
        printf("Manufacturer %d: Insert Item %d at %d\n", *((int *)mno),buff[receive],receive);
        receive = (receive+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&complete);
    }
}
void *client(void *clno)
{   
    for(int i = 0; i < MaxItems; i++) 
    {
        sem_wait(&complete);
        pthread_mutex_lock(&mutex);
        int product = buff[send];
        printf("Client %d: Remove Item %d from %d\n",*((int *)clno),product, send);
        send = (send+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&zero);
    }
}

int main()
{   

    pthread_t adv[5],dis[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&zero,0,BufferSize);
    sem_init(&complete,0,0);

    int first[5] = {1,2,3,4,5}; //Just used for numbering the producer and consumer

    for(int i = 0; i < 5; i++) 
    {
        pthread_create(&adv[i], NULL, (void *)manufacturer, (void *)&first[i]);
    }
    for(int i = 0; i < 5; i++)
     {
        pthread_create(&dis[i], NULL, (void *)client, (void *)&first[i]);
    }

    for(int i = 0; i < 5; i++) 
    {
        pthread_join(adv[i], NULL);
    }
    for(int i = 0; i < 5; i++) 
    {
        pthread_join(dis[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&zero);
    sem_destroy(&complete);

    return 0;
    
}

