# Informe

Nos encontramos muy dificil agregar una malla de triángulos sin utilizar alguna manera para dividir el espacio en cajas para optimizar la velocidad. Ya el modelo de among us tiene 3412 tríangulos. 
Al principio tratamos de crearla manualmente mediante cajas dado 2 puntos. Las cajas se formaban mediante 6 caras, donde cada cara se formaba mediante 2 triángulos. Luego vimos la implementación del libro de Shirley y por la simpleza decidimos quedarnos con su implementación que utiliza Axis-Aligned bounding boxes las cuales son simples de implementar la intersección con los rayos y rápida (comparado a los 12 tríangulos de la caja).

## Multithreading rendering
El primer intento de dividir el trabajo fue el más simple que se me ocurrió: dividir el alto entre la cantidad de cores del cpu y que cada uno computara una banda de la imagen.
Un problema que luego descubrí con ese método, es que siempre los cores terminaban de manera desigual, entonces siempre se terminaba esperando a uno último que terminara mientras los demás ya habían terminado. Esto tiene que ver con que el tiempo de computo de un pixel depende de lo que estamos rendereando, y sucede que no es uniforme para cada pixel, teniendo partes donde hay poco computo (computar el fondo) y otras donde hay más (objetos que reflejan).
Para solucionar eso, cambié la forma de computar a que cada core compute subsiguientes, 
- El 1er core computa el 0, cant_cores, cant_cores*2, ...
- El 2er core computa el 1, 1+cant_cores, 1+cant_cores*2, ...
- ...

Lo único malo de computar de esta manera es que no es uniforme calcular el tiempo estimado. Aunque me parece que es difícil calcularlo de cualquier manera.