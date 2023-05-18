# Sincronización de Procesos en Java

Este es un proyecto para el curso de Teoría de Sistemas Operativos. Ilustra cómo se pueden usar los semáforos y la sincronización de hilos en Java para manejar la sincronización entre procesos. El escenario es un puente por el que las personas pueden cruzar en un solo sentido a la vez, pero más de una persona puede estar en el puente al mismo tiempo si todas van en la misma dirección.

## Cómo ejecutar el programa

1. Compilar el programa: `make`
2. Ejecutar el programa: `make run`

## Salida

La salida del programa muestra cuándo y en qué dirección las personas entran y salen del puente, cuántas personas están actualmente en el puente y cuántas personas están esperando para cruzar en cada dirección.

## Limpieza

Para eliminar los archivos de clase compilados, usa el comando: `make clean`
