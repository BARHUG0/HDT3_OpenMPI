/*----------------------------------------------------------------------
 * Universidad del Valle de Guatemala
 * Curso:     CC3169 - Computacion Paralela y Distribuida
 * Ejercicio: Hoja de Trabajo 03 - OpenMPI comunicacion entre procesos
 *            Inciso 4
 * Descripcion: simulacion de la distribucion de pedidos desde la
 *              Oficina Central hacia las sucursales.
 *
 *              Cada proceso MPI representa una ubicacion diferente:
 *                  rank 0 -> Oficina central
 *                  rank 1 -> Sucursal 1
 *                  rank 2 -> Sucursal 2
 *                  rank 3 -> Sucursal 3
 *
 *              La Oficina Central posee una lista con dos datos por
 *              ubicacion (cantidad de pedidos y cantidad de empleados
 *              disponibles) y distribuye ambos a cada proceso
 *              utilizando MPI_Scatter() con sendcount = recvcount = 2.
 *----------------------------------------------------------------------*/

#include <stdio.h>
#include <mpi.h>

#define DATOS_POR_UBICACION 2

int main(int argc, char *argv[]) {

    int rank;
    int size;
    int datos[4 * DATOS_POR_UBICACION];
    int datos_recibidos[DATOS_POR_UBICACION];

    // Inicializa el entorno MPI
    MPI_Init(&argc, &argv);

    // Obtener el identificador del proceso actual
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Obtener el numero total de procesos que participan
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Este ejercicio requiere exactamente 4 procesos
    if (size != 4) {

        if (rank == 0) {
            printf("Este programa requiere exactamente 4 procesos.\n");
        }

        MPI_Finalize();
        return 0;
    }

    // La Oficina Central define, para cada ubicacion, la cantidad de
    // pedidos y la cantidad de empleados disponibles (en pares contiguos)
    if (rank == 0) {

        datos[0] = 120;  datos[1] = 10;   // Oficina Central
        datos[2] = 95;   datos[3] = 8;    // Sucursal 1
        datos[4] = 140;  datos[5] = 12;   // Sucursal 2
        datos[6] = 110;  datos[7] = 9;    // Sucursal 3

        printf("Oficina Central: distribuyendo pedidos y empleados...\n");
    }

    // Distribuir dos valores consecutivos del arreglo a cada proceso
    MPI_Scatter(
        datos,
        DATOS_POR_UBICACION,
        MPI_INT,
        datos_recibidos,
        DATOS_POR_UBICACION,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    // Cada proceso muestra los valores que recibio
    if (rank == 0) {
        printf("Oficina Central: %d pedidos asignados, %d empleados disponibles.\n",
               datos_recibidos[0], datos_recibidos[1]);
    } else {
        printf("Sucursal %d: %d pedidos asignados, %d empleados disponibles.\n",
               rank, datos_recibidos[0], datos_recibidos[1]);
    }

    // Finaliza correctamente el entorno MPI
    MPI_Finalize();

    return 0;
}
