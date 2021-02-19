#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "mpi.h"

float kernel[3] = {1./3., 1./3., 1./3.};
MPI_Request req_1[4];
MPI_Request req_2[4];
float *v1 = NULL;
float *v2 = NULL;

#define BUFFER_SIZE 4*1024*1024
#define MAX_REPEAT_NB 100

void
Icommunications(float *my_values)
{
    int rank, size, array_size;
    int tag, left_neighbor, right_neighbor;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    tag = 0; 
    left_neighbor  = ( rank - 1 + size ) % size;
    right_neighbor = ( rank + 1 + size ) % size;
    array_size = ( BUFFER_SIZE  / size ) ;

    /* On réalise le partage en deux passes pour ne pas avoir de deadlock */

    /* Les processus de rang pair envoient leur première case à leur voisin de gauche
     * Les processus de rang impair envoient leur dernière case à leur voisin de droite
     */

    if (my_values == v1)
      {
        /* 1 est la première case de tableau réel */
        MPI_Isend( &my_values[1], 1, MPI_FLOAT, left_neighbor, tag, MPI_COMM_WORLD, req_1 );
        /* 0 est la maille fantôme de gauche */
        MPI_Irecv( &my_values[0], 1, MPI_FLOAT, left_neighbor, tag, MPI_COMM_WORLD, req_1 + 1);

        /* array_size + 1 est la maille fantôme de droite */
        MPI_Irecv( &my_values[array_size + 1], 1, MPI_FLOAT, right_neighbor, tag, MPI_COMM_WORLD, req_1 + 2);
        /* array_size est la dernière case du tableau réel */
        MPI_Isend( &my_values[array_size    ], 1, MPI_FLOAT, right_neighbor, tag, MPI_COMM_WORLD, req_1 + 3);
      }
    else
      {
        /* 1 est la première case de tableau réel */
        MPI_Isend( &my_values[1], 1, MPI_FLOAT, left_neighbor, tag, MPI_COMM_WORLD, req_2 );
        /* 0 est la maille fantôme de gauche */
        MPI_Irecv( &my_values[0], 1, MPI_FLOAT, left_neighbor, tag, MPI_COMM_WORLD, req_2 + 1);

        /* array_size + 1 est la maille fantôme de droite */
        MPI_Irecv( &my_values[array_size + 1], 1, MPI_FLOAT, right_neighbor, tag, MPI_COMM_WORLD, req_2 + 2);
        /* array_size est la dernière case du tableau réel */
        MPI_Isend( &my_values[array_size    ], 1, MPI_FLOAT, right_neighbor, tag, MPI_COMM_WORLD, req_2 + 3);
      }

    return;
}

void wait_communications()
{
  MPI_Waitall(4, req_1, MPI_STATUSES_IGNORE);
  MPI_Waitall(4, req_2, MPI_STATUSES_IGNORE);
  return;
}

void
convolution(float *my_values, float *tmp_values)
{
    int i, k, size, array_size;
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    array_size = BUFFER_SIZE / size;    
    
    /* On évite la maille fantôme 
     * Pour rappel: le tableau a array_size + 2 elts 
     * On n'utilise pas deux tableaux car si on modifie my_value
     * alors on modifie la valeur de  my_values[ i - 1 ] de 
     * l'itération suivante.
     */
    for(i = 1; i < array_size + 1; i++)
    {
        tmp_values[ i ] = 0;
        for(k = -1; k <= 1; k++)
            tmp_values[ i ] += my_values[ i + k ] * kernel[ 1 + k ];
    }
    /* copie tous les "array_size + 2 float" de tmp_values dans my_values */
    memcpy( &(my_values[1]), &(tmp_values[1]) , array_size * sizeof( float ) );

    return;
}

int main(int argc, char **argv)
{
    int i, array_size, size, rank;
    float *my_values1, *my_values2, *tmp_values, *full_vector1, *full_vector2;
    double tbeg, tend, telaps;

    MPI_Init( &argc, &argv );
       
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if( BUFFER_SIZE % size != 0 )
    {
        fprintf(stdout, "Le nombre de processus n'est pas un multiple de la taille du tableau.\n");
        abort();
    }    
    
    if( rank == 0)
    {
        full_vector1 = malloc( BUFFER_SIZE * sizeof( float ) ); 
        assert( full_vector1 != NULL );

        full_vector2 = malloc( BUFFER_SIZE * sizeof( float ) ); 
        assert( full_vector2 != NULL );

        /* Initialisation du tableau */
        for( i = 0; i < BUFFER_SIZE; i++ )
        {
            full_vector1[i] = i;
            full_vector2[i] = -i;
        }
    } 
    
    array_size = BUFFER_SIZE / size; 
    my_values1 = malloc( ( array_size + 2 ) * sizeof( float ) ); //+2 pour les mailles fantomes
    assert( my_values1 != NULL );
    my_values2 = malloc( ( array_size + 2 ) * sizeof( float ) ); //+2 pour les mailles fantomes
    assert( my_values2 != NULL );
    tmp_values = malloc( ( array_size + 2 ) * sizeof( float ) ); //+2 pour les mailles fantomes
    assert( tmp_values != NULL );
    
    /* On décale le buffer de 1 pour la réception afin 
     * de garder la maille fantome de gauche vide */
    void * begin_my_value1 = ( char * ) my_values1 + sizeof( float );
    void * begin_my_value2 = ( char * ) my_values2 + sizeof( float );

    v1 = my_values1;
    v2 = my_values2;
    
    /* Distribution du tableau */ 
    MPI_Scatter(full_vector1, array_size, MPI_FLOAT, begin_my_value1, array_size, MPI_FLOAT, 0, MPI_COMM_WORLD);  
    MPI_Scatter(full_vector2, array_size, MPI_FLOAT, begin_my_value2, array_size, MPI_FLOAT, 0, MPI_COMM_WORLD);  
    if ( rank == 0 )
    {
        memset(full_vector1, 0, BUFFER_SIZE*sizeof(float));
        memset(full_vector2, 0, BUFFER_SIZE*sizeof(float));
    }

    tbeg = MPI_Wtime();

    for( i = 0; i < MAX_REPEAT_NB; i++)
    { 
      /* Communications */
      Icommunications( my_values1 );
      Icommunications( my_values2 );

      /* Convolution */
      convolution( my_values1, tmp_values );
      convolution( my_values2, tmp_values );

      wait_communications();
    }

    tend = MPI_Wtime();
    telaps = tend - tbeg;
    if (rank == 0)
    {
        printf("Pt2pt Telaps = %g s\n", telaps);
    }

    /* Récuperation de l'ensemble du tableau */ 
    MPI_Gather( begin_my_value1, array_size, MPI_FLOAT, full_vector1, array_size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Gather( begin_my_value2, array_size, MPI_FLOAT, full_vector2, array_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* Libération de la mémoire */
    if( rank == 0 )
    {
            free(full_vector1); 
            free(full_vector2); 
    }
    free(my_values1);
    free(my_values2);
    free(tmp_values);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
