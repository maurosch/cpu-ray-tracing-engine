# Informe
## Triangle Mesh
Me encontré muy difícil agregar una malla de triángulos sin utilizar alguna manera para dividir el espacio en cajas para optimizar la velocidad. El modelo inicial fue el de among us el cual tiene 3412 tríangulos. Con 20 rayos por píxeles con 5 cantidades de rebotes tenemos 3412*20*5 tenemos 3millones de cálculos por píxel.
Para subdividir el espacio traté primero hacerlo manualmente mediante cajas dado 2 puntos. Las cajas se formaban mediante 6 caras, donde cada cara se formaba mediante 2 triángulos. Esto conllevaba muchos cálculos para ver si el rayo le pega a la bounding box. Luego ví la implementación del libro de ray tracing de Shirley y por la simpleza decidimos quedarnos con su implementación que utiliza Axis-Aligned bounding boxes la cual es rápida para computar la intersección con rayos.

## Multithreading rendering
El primer intento de dividir el trabajo fue el más simple que se me ocurrió: dividir el alto entre la cantidad de cores del cpu y que cada uno computara una banda de la imagen.
Un problema que luego descubrí con ese método, es que siempre los cores terminaban de manera desigual, entonces siempre se terminaba esperando a uno último que terminara mientras los demás ya habían terminado. Esto tiene que ver con que el tiempo de computo de un pixel depende de lo que estamos rendereando, y sucede que no es uniforme para cada pixel, teniendo partes donde hay poco computo (computar el fondo) y otras donde hay más (objetos que reflejan).
Para solucionar eso, cambié la forma de computar a que cada core compute subsiguientes, 
- El 1er core computa el 0, cant_cores, cant_cores*2, ...
- El 2er core computa el 1, 1+cant_cores, 1+cant_cores*2, ...
- ...

Lo único malo de computar de esta manera es que no es uniforme calcular el tiempo estimado. Es bastante malo estimando. Aunque me parece que es difícil calcularlo de cualquier manera porque depende de como esté estructurada la imagen.