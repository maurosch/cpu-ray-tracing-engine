---
title: Proyecto Final Computación Gráfica
author: Mauro Schiavinato
date: 10-01-2022
...
\newcommand{\img}[1]{\begin{center}\includegraphics[width=0.7\textwidth]{#1}\end{center}}

# Informe
El trabajo se basó en las guías [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) y [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html). Del primero está usado todo lo que muestra mientras del segundo se utiliza para jerarquía de borde objectos (optimización), iluminación, texturas y densidad de objectos (gases como aire).


## Agregados de la guía de Shirley
- Malla de triángulos y polígonos (con lectura de OBJ).
- Reestructuración del código.
- Cálculo multithreading con visualizador de porcentaje y tiempo estimado.
- Configuración desde json.
- Formatos de salida de imágenes

## Triangle Mesh
Me encontré muy difícil agregar una malla de triángulos sin utilizar alguna manera para dividir el espacio en cajas para optimizar la velocidad. El modelo inicial fue el de among us el cual tiene 3412 tríangulos. Con 20 rayos por píxeles con 5 cantidades de rebotes tenemos 3412*20*5 tenemos 3millones de cálculos por píxel.
Para subdividir el espacio traté primero hacerlo manualmente mediante cajas dado 2 puntos. Las cajas se formaban mediante 6 caras, donde cada cara se formaba mediante 2 triángulos. Esto conllevaba muchos cálculos para ver si el rayo le pega a la bounding box. Luego ví la implementación del libro de ray tracing de Shirley y por la simpleza decidimos quedarnos con su implementación que utiliza Axis-Aligned bounding boxes la cual es rápida para computar la intersección con rayos.

Luego para la intersección con un triángulo, usé la implementación del libro del Fundamentals of Computer Graphics Shirley.

Al comenzar a implementarla me encontré con este error visual:

\img{../renders/25spp-10r-02-01-2022.png}

El cual se producía porque no le agregaba la normal al triángulo, entonces algunos tríangulos tenían la normal para el otro lado. El código del material le importa la dirección de la normal porque es usada para el rebote, aunque podría hacerse que no. Pero está bueno si se quiere algún efecto que dependa de si la cara es exterior o inferior (un ejemplo podría ser un panel de luz).  
Luego de solucionarlo:

\img{../renders/25spp-10r-03-01-2022.png}

Como los OBJ tienen una normal para cada vértice, tomo la normal de cada triángulo como promedio de las normales de cada vértice.

## Multithreading rendering
El primer intento de dividir el trabajo fue el más simple que se me ocurrió: dividir el alto entre la cantidad de cores del cpu y que cada uno computara una banda de la imagen.
Un problema que luego descubrí con ese método, es que siempre los cores terminaban de manera desigual, entonces siempre se terminaba esperando a uno último que terminara mientras los demás ya habían terminado. Esto tiene que ver con que el tiempo de computo de un pixel depende de lo que estamos rendereando, y sucede que no es uniforme para cada pixel, teniendo partes donde hay poco computo (computar el fondo) y otras donde hay más (objetos que reflejan).
Para solucionar eso, cambié la forma de computar a que cada core compute subsiguientes,  
- El 1er core computa el 0, cant_cores, cant_cores*2, ...  
- El 2er core computa el 1, 1+cant_cores, 1+cant_cores*2, ...  
- ...

Lo único malo de computar de esta manera es que no es uniforme calcular el tiempo estimado. Es bastante malo estimando. Aunque me parece que es difícil calcularlo de cualquier manera porque depende de como esté estructurada la imagen.

## Carga muy lenta de OBJ
Partí haciendo copy-paste de la carga de OBJ de proyectos anteriores de la materia. 
Tomaba mucho tiempo desde que empezaba el programa hasta que empezaba a calcular los rayos (20 segundos), suponiendo que era problema de la carga de OBJ. Luego de hacer profiling, noté que el código de Shirley para los BVH generaba una copia recursivamente del arreglo de objectos, haciendo muy lento el código. Luego de sacarlo y hacerla mutable pasó a 4 segundos. Para hacer el profiling me encontré que la manera más rápido (porque valgrind era una tortuga) era parando el debugger en momentos dados y viendo donde caía, más info en [esta respuesta](https://stackoverflow.com/questions/375913/how-can-i-profile-c-code-running-on-linux). Debido a este problema al principio al importar modelos de afuera les rebajaba la cantidad de triángulos mediante blender (tiene una opción).

## Parser de OBJ
Un error ya terminando el proyecto es que algunos modelos como el among us, teapot y dragon tienen 2 espacios entre el vértice y la definición mientras que el duck tiene un solo espacio. Pero lo interesante es que no tiraba error al parsear porque como estaba parseando floats, si le pasas .18 te lo toma como 0.18. Entonces pasaba que algunas veces se comía un número o el signo del primer vértice. Aquí una visualización del error:

\img{../renders/error-obj-file.jpg}

Y luego de corregirlo:

\img{../renders/error-obj-file2.jpg}


## Ayuda con blender

Algunas veces me manejaba con Blender para ver la escala que debían quedar los objetos y en que posición, ya que correr el programa tarda bastante tiempo.

\img{imgs/blender.png}

## Renders finales

![Render final dragon](../renders/10000spp-50r-08-01-2022.jpg)

![Render final varios modelos](../renders/(4000spp-50r-10-01-2022.jpg)

## Ambisiones no hechas
- Soportar OBJ más complejos: con múltiples mallas adentro y con múltiples imágenes para texturas.
- Animaciones
- Crear la escena desde el navegador web con el [editor de three.js](https://threejs.org/editor) y mediante un servidor web generar el rending final y mandarlo al navegador de nuevo.
- Utilizar las funcionalidades de las RTX para producir imagenes similares (es decir hardware acelerado).
