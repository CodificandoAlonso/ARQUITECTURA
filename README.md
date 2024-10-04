# Práctica Arquitectura de Computadores
## Integrantes
100495764, Agramunt Muro, Jorge  
100495794, Alvarez Marcos, Hector  
100495879, Navarro Hurtado, Pablo  
100471939, Penas Diaz, Alberto

## Resumen de la práctica
Como es jodidamente infumable el enunciado de la práctica, os hago un resumen por aquí para que no tengáis que leerlo entero.
Hay que desarrollar un programa en C++ **SECUENCIAL** (esto es super importante porque pone específicamente que no puede hacerse en paralelo)
con cuatro funcionalidades con respecto a una imagen de entrada (en formato PPM):
- info: muestra todos los metadatos de la imagen (los metadatos son la información que se almacena en una imagen, flow la resolución, la fecha en la que se hizo, etc.)
- maxlevel: realiza un **escalado de la intensidad**, es decir bajar o subir la exposición de la imagen. Esto se hace con unas fórmulas que vienen en el enunciado.
- resize: esto es la movida de la interpolación bilineal xd. Básicamente, se trata de cambiar el tamaño de la imagen. Si la imagen es más grande, se interpola para que no se vea pixelada. 
  Si la imagen es más pequeña, se interpola para que no se vea borrosa.
- cutfreq: elimina los colores menos frecuentes de la imagen.
- compress: realiza una compresión de la imagen al formato cppm (está especificado en el enunciado).

En todas las funcionalidades excepto en la primera, la salida es una nueva foto. Y POR ESO HAY QUE HACER DOS PROGRAMAS DISTINTOS, uno (imtool-aos) que
exporta la imagen en un formato y otro (imtool-soa) que exporta la imagen en otro formato.
El directorio "common" contiene las funcinoes principales que se comparten entre ambos programas.
El resto de directorios solo sirve para gestionar las funcionalidades de cada programa.

El archivo .clang-format es un archivo de configuración para el formateador de código clang-format. Se puede usar para formatear el código de forma automática.
Es decir, es literalmente lo mismo que el pylint de python.
