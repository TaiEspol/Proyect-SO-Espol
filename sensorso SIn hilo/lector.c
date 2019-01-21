/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) Daniel Ochoa Donoso 2010 <dochoa@fiec.espol.edu.ec>
 * 
 * main.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * main.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SHMSZ  27
#define PI 3.14159265



char *memoria_key(key_t key);

int shmidd,shmidt;
key_t keyd,keyt;
char *shmd, *shmt;

int main()
{
    struct timespec start, finish;
    long seconds,ns;

	char tmpd[SHMSZ];
    char oldt[SHMSZ];
    char tmpt[SHMSZ];
    double distancia = 0.0,angulo = 0.0;
    int num_distancia = 0,num_angulo = 0;
    int comienzo = 0,clock_enter = 0;
    double valor,subtotal,total;
    
    keyd = 1234;
    keyt = 5678;
    shmt= memoria_key(keyt);
    shmd= memoria_key(keyd);

    while(1){
        if (comienzo == 0){
            sleep(2);
            comienzo = 1;
        }
        if (clock_enter == 0){
            clock_gettime( CLOCK_REALTIME, &start);
            clock_enter = 1;
        }
		strcpy(tmpt,shmt);
		if ((strcmp(tmpt,"--")!=0)&&(strcmp(oldt,tmpt)!=0)){
			strcpy(oldt,tmpt);
            angulo = angulo + cos((atof(oldt)/(PI*180)));
            num_angulo = num_angulo + 1;
            
        }
		if (strcmp(shmd,tmpd)!=0){
            distancia = distancia + atof(shmd);
            num_distancia = num_distancia + 1;
        }
		strcpy(tmpd,shmd);
        
        if (distancia != 0.0 && angulo != 0.0){
            subtotal = distancia/num_distancia;
            valor = angulo/num_angulo;
            total = subtotal * valor;
            
            angulo = 0.0;
            distancia = 0.0; 
            num_angulo = 0;
            num_distancia = 0;
            printf("****************************************************************************\n");
            fprintf(stdout,"Distancia Total: %lf\n",total);
            if (clock_enter == 1){
                clock_gettime(CLOCK_REALTIME, &finish); 
                seconds = finish.tv_sec - start.tv_sec; 
                ns = finish.tv_nsec - start.tv_nsec; 
                if (start.tv_nsec > finish.tv_nsec) { // clock underflow 
                    --seconds; 
                    ns += 1000000000; 
                }
                clock_enter = 0;
            }
            printf("Tiempo transcurrido: %e segundos\n", (double)seconds + (double)ns/(double)1000000000); 
            printf("****************************************************************************\n");
        }

	}
    return(0);

}

char *memoria_key(key_t key){
    char *shmts;
    int shmidt;

    if ((shmidt = shmget(key, SHMSZ,  0666)) < 0) {
        perror("shmget");
    }
    if ((shmts = shmat(shmidt, NULL, 0)) == (char *) -1) {
        perror("shmat");
    }
    return shmts;
}