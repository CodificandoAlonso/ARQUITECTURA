# Arquitectura de Computadores ![]()
# Proyecto

Grupo Reducido: 80, 81  
Grupo de Trabajo: 14

**Alberto Penas Díaz 100471939**  
**Héctor Álvarez Marcos 100495794**  
**Jorge Agramunt Muro 100495764**  
**Pablo Navarro Hurtado 100495879** 

Alberto Penas | Héctor Álvarez | Jorge Agramunt | Pablo Navarro ![]()![]()

---

## Índice

- [Diseño](#diseño)
- [Optimización](#optimización)
- [Maxlevel](#maxlevel)
- [Resize](#resize)
- [Cutfreq](#cutfreq)
- [Compress](#compress)
- [Pruebas Realizadas](#pruebas-realizadas)
- [Evaluación de Rendimiento y Energía](#evaluación-de-rendimiento-y-energía)
  - [Evaluación en Avignon](#evaluación-en-avignon)
  - [Evaluación en máquina local](#evaluación-en-máquina-local)
- [Organización del Trabajo](#organización-del-trabajo)
- [Conclusiones](#conclusiones)
- [Declaración responsable](#declaración-responsable)
- [Conclusiones Personales](#conclusiones-personales)
- [Anexo](#anexo)

---

## Diseño

Para comenzar el desarrollo de nuestro proyecto, hemos analizado qué funciones son comunes a ambas estrategias (AOS y SOA) para **encapsular** las mismas en clases que acojan conceptualmente a las imágenes. De modo general, el diseño de nuestro programa puede ser resumido en el siguiente diagrama de clases:

![](/docs/images/Aspose.Words.a9a9ffdb-5f8d-4f84-9aa7-f3ac946d3b1f.005.png)

Siguiendo el modelo que se nos ofrecía en el enunciado, todas las funcionalidades que comparten ambas estrategias están agrupadas en la librería “*common*”. En esta librería, además de la superclase “*Image*”, están presentes los siguientes ficheros:

- **binario**:  Agrupación  de  todas las funciones de entrada y salida binaria, así como funciones específicas de manejo de bits.
- **struct-rgb**: Definición de las estructuras específicas usadas para almacenar píxeles en memoria, teniendo en cuenta las estrategias SOA y AOS

De forma general, hemos diseñado todas y cada una de las funciones para que acojan todos los posibles casos de uso posibles en la aplicación. De esta manera, nuestro diseño de cada función separa, en un primer condicional, el formato de entrada y salida de las imágenes para poder dirigirnos a la función específica que gestione la funcionalidad pedida. Esta metodología genera más líneas de código pero organiza el mismo en bloques muy identificativos, lo que ha sido de gran ayuda en el desarrollo del proyecto para navegar en el mismo.

---

## Optimización

Complejidad final **teórica** obtenida:

|Complejidad |Info |Maxlevel |Resize |Cutfreq |Compress |
| - | - | - | - | - | - |
|Image AOS |(1) |( ) |(2 )|- |(2 )|
|Image SOA |(1) |( ) |(2 )|- |(2 )  |

No hemos puesto complejidad a la función Cutfreq debido al gran número de parámetros del que depende. Como  optimizaciones  comunes  a  todas  las  funciones,  hemos  optado  por  agregar  unos  flags  de compilación específicos para aumentar al máximo el rendimiento de nuestro programa:

- **-march=native**:  Esta  opción  le  indica  al  compilador  que  optimice  el  código  para  la arquitectura específica de la CPU en la que se está compilando el código.
- **-mtune=native**: Este flag ajusta el rendimiento del código generado para la CPU específica en  la  que  se  está  compilando.  Mientras  que  -march  define  las  instrucciones  disponibles, -mtune ajusta los detalles de rendimiento (como la programación de instrucciones) para esa CPU en particular, tratando de optimizar su velocidad.
- **-O3**: Esta opción activa el nivel de optimización más alto. -O3 habilita todas las optimizaciones de  -O2,  más  algunas  adicionales  como  la  desenrollación  de  bucles  y  el  uso  de  SIMD (instrucciones de múltiples datos) en bucles.
- **-funroll-loops**:  Desenrolla  bucles, es decir, transforma los bucles en una secuencia de instrucciones repetidas en lugar de mantener la estructura del bucle original.
- **-ffast-math**: Permite ciertas optimizaciones matemáticas que no garantizan un cumplimiento estricto de las reglas IEEE o ISO C en las operaciones de punto flotante. Por ejemplo, puede permitir el reordenamiento de operaciones, suposiciones de asociatividad y otras optimizaciones que pueden mejorar la velocidad de las operaciones matemáticas.
- **-fstrict-aliasing**: Indica al compilador que puede asumir que los punteros de distintos tipos no apuntan a la misma dirección en memoria. Esta suposición permite optimizaciones en el acceso a memoria.

Para  verificar  la  utilidad  de  estos  flags de compilación, hemos medido el tiempo de ejecución con respecto a la versión sin flags (debug) en nuestro equipo local:

![](/docs/images/Aspose.Words.a9a9ffdb-5f8d-4f84-9aa7-f3ac946d3b1f.006.jpeg)

A continuación, se especifican detalladamente las optimizaciones por las que se ha optado en esta función.

---

## Maxlevel

Esta función (junto con “*info*”, pero la omitimos debido a su trivialidad) es la única que NO hace uso de AOS o SOA, ya que no existe ninguna necesidad de almacenar la imagen en memoria. En esta función, nos  recorremos  la  imagen  píxel  a  píxel,  operamos  sobre  cada  píxel  recogido  en  cada  iteración  y finalmente lo escribimos en la imagen de salida.

---

## Resize

En esta función, la imagen de entrada es almacenada en memoria siguiendo las estrategias SOA o AOS. Dependiendo  del  formato[^1]  de la imagen de entrada, se tiene en cuenta el tipo de dato óptimo para almacenar  cada  canal  de  color,  usando  *unsigned  char*  o  *unsigned  short*  según  sea  necesario.  Esta distinción la hacemos para ahorrar memoria en caso de que la imagen sea con formato de 3 Bytes por píxel, ya que sobraría espacio utilizaramos únicamente un tipo de dato capaz de almacenar 16 bits para representar canales de color que únicamente necesitan 8 bits.

Una vez se ha recorrido la imagen de entrada, nos recorremos la imagen de salida según el nuevo tamaño especificado  en  los  argumentos  de  entrada  al  programa.  Para  cada  píxel,  hacemos  las  operaciones convenientes y lo escribimos en la imagen de salida.

---

## Cutfreq

Nuestra  implementación  más  compleja  es  de  la función Cutfreq, pues hemos seguido un desarrollo enrevesado pero óptimo. Como más adelante se podrá observar en el diagrama de flujo, esta función se puede dividir en dos partes importantes:

1. Obtención de colores a eliminar (selección de píxeles únicos y análisis de frecuencia).
2. Sustitución de colores a eliminar.

Para la obtención de colores a eliminar, por ejemplo si queremos eliminar los 3 colores menos frecuentes y tenemos 2 colores con frecuencia 1, son enviados directamente a eliminación, ya que no hace falta comprobar nada más, si tenemos dos colores con frecuencia 2, no sabemos cuál vamos a eliminar, ya que solo nos falta añadir un color más a eliminación. Por ello, a igualdad de frecuencia en la imagen, para elegir el color que vamos a eliminar de los dos….

Una vez hemos obtenido los colores finales a eliminar, falta ver por qué colores van a ser sustituidos. Para ello, a nivel conceptual, nuestra implementación representa los colores que se **no se van a eliminar** en un espacio tridimensional cuyos ejes son los canales de color r, g, b. Para buscar el sustituto de cada color que se va a eliminar, bastaría con recorrerse cada punto de este espacio  
![](/docs/images/Aspose.Words.a9a9ffdb-5f8d-4f84-9aa7-f3ac946d3b1f.007.png)
hasta encontrar el punto con menor distancia euclídea. Sin embargo, esto  puede ser MUY ineficiente, ya que hace falta recorrerse toda la nube de  puntos  para  encontrar  este  sustituto.  Por  esto,  nuestra  primera  idea  consistió  en  implementar el algoritmo de aprendizaje automático **KNN**  (siendo en nuestro caso k=1), que, dado un punto en una nube de puntos,  obtiene el primer punto que entre dentro de una “esfera tridimensional”  cuyo punto céntrico es el color a eliminar (representado en la imagen de la derecha en 2 dimensiones).

Sin embargo, esto también es muy ineficiente cuando quedan muy pocos colores en la imagen, ya que hay que recorrerse prácticamente todo el espacio. Por ello, la implementación definitiva fue dividir el espacio tridimensional en sub-espacios, representados en nuestro código en forma de grafo. De esta manera, por cada píxel que se pretende eliminar, buscamos ÚNICAMENTE en el subespacio que le corresponde, así como en los inmediatamente adyacentes. De esta forma, nos ahorramos comprobar la distancia euclídea de todos aquellos subespacios que estén alejados del punto que queremos eliminar.

De modo más detallado, adjuntamos el diagrama de flujo correspondiente a la función:

![](/docs/images/Aspose.Words.a9a9ffdb-5f8d-4f84-9aa7-f3ac946d3b1f.008.png)

---

## Compress

Para optimizar esta función, hemos tratado de tener mucho cuidado con respecto a las estructuras de datos que empleamos para buscar los píxeles. El diagrama de flujo  de esta función es el siguiente:

![](/docs/images/Aspose.Words.a9a9ffdb-5f8d-4f84-9aa7-f3ac946d3b1f.009.png)

Tal y como se puede observar, nos recorremos la imagen original 2 veces y en cada iteración del recorrido insertamos o buscamos el píxel almacenado en el hashmap[^2]. Es importante mencionar que esta estructura únicamente se usa a **modo de catálogo**, siendo la estructura principal de almacenamiento el SOA o el AOS.

Somos plenamente conscientes de que, usando dos estructuras distintas para guardar la imagen y los índices al mismo tiempo, aumentamos significativamente el uso de memoria y por consiguiente, la tasa de accesos a la caché. Sin embargo, haciendo uso de ésta estrategia, disminuye drásticamente el tiempo de ejecución por lo que hemos optado por ella, ya que consideramos que la ganancia en tiempo de ejecución es proporcionalmente mejor (en este caso) al “excesivo” uso de memoria.

---

## Pruebas Realizadas

Hemos creado tres diferentes directorios para la ejecución de los unitests: utest-common, utest-imgsoa y utest-imgaos. En cada uno de ellos hemos creado un fichero distinto para la realización de los distintos unitests dentro de los ficheros de comonn, imgsoa e imgaos respectivamente.

En binario\_test creamos la clase BinaryTest para probar las distintas funciones dentro de binario.cpp, read\_binary\_8, write\_binary\_8, etc. En cada uno de los tests creamos un fichero temporal en el que escribimos y/o leemos valores en binario dependiendo de la función sobre la que se esté realizando el unitest.

Para cada una de las distintas funciones hemos hecho al menos un caso en el que la función funciona correctamente, un caso en el no funciona y casos extra dependiendo de si salta o no un error.

Con  respecto  a  las  pruebas  funcionales,  hemos  usado  el  lenguaje  de programación python para su realización, debido a la facilidad de uso y la versatilidad que ofrece, **hemos unificado** tanto las pruebas funcionales del AOS como del SOA en un único fichero que prueba directamente comandos de terminal que ejecuta nuestro programa y compara la salida de nuestras fotografías directamente con las salidas esperadas proporcionadas en Aula Global, es por ello (por la necesidad de comparar imágenes) *que el directorio  “ftest”  tiene  a  su vez dos subdirectorios; “input”, donde se encuentran las imágenes de entrada y “expected” donde se encuentran las imágenes esperadas. **PARA LA FUNCIONALIDAD DE LOS FTESTS ES IMPORTANTE QUE SE CARGUEN EN LOS DIRECTORIOS LAS SIGUIENTES IMAGENES.***  Para  mayor  facilidad,  hemos  subido  un  archivo  a  drive  con  las  imágenes  en  sus localizaciones  correctas: <https://drive.google.com/file/d/1hZVw0Jp252VRlmlQJfe9BQ8lBcUxt4Ij/view?usp=sharing>

Estructura de directorios:

**input**: *deer-small.pm deer-large.ppm lake-small.ppm lake-large.ppm sabatini.ppm*

**expected**:

Directorio “**cutfreq**”: Revision 2 de salidas para cutfreq(solo los archivos .ppm)  
Directorio “**resize**”: Revisión 2 de salidas para resize(solo los archivos .ppm)  
Directorio “**compress**”: Salidas de referencia(solo los archivos .cppm)  
Directorio  “**maxlevel**”:  Salidas  de  referencia  +  salidas  adicionales(solo  los  archivos .ppm)

**EL INDICAR LA ESTRUCTURA DE DIRECTORIOS PARA LOS FTESTS SE HA REALIZADO DEBIDO AL LÍMITE DE 500KB DEL ENTREGADOR.**

Para el “*ftest*” del Cutfreq hemos cambiado la interpretación que se ha dado de permisividad de cambios porque consideramos que para esta función el enfoque otorgado como esperado no es el adecuado. Se nos permite  una  variación  de  +-5  en  cada  componente  R,G  y/o  B.  Nosotros  para  esta  función  hemos considerado 2 tipos de aciertos de colores “Distintos”. El primero: Si la distancia euclídea entre el pixel antiguo y el cambiado por la salida esperada es igual a la distancia euclídea entre el pixel antiguo y nuestro cambio no se considera fallo. Ejemplo: Pixel original (10,10,10) Pixel esperado (5,5,5) Pixel nuestro (15,15,15). En este caso la diferencia es mayor de 5 entre el esperado y el nuestro, sin embargo la distancia euclídea es igual.

El segundo caso es, para tener en cuenta esos +-5 en diferencia de color, hemos tenido en cuenta el peor caso. Si la diferencia entre el pixel esperado y el nuestro es de 5 en cada componente, la distancia euclídea entre cambios será de 8.66 == sqrt(5² + 5² + 5²). Por lo que consideramos también acierto si la diferencia entre la distancia euclídea del pixel esperado y de la nuestra es < 8.66. Basamos la superación de la prueba si se consigue un porcentaje de acierto > 99.99 teniendo en cuenta que el % de acierto se mide **no sobre el total de píxeles**, si no sobre el % de píxeles modificados, es decir, los que han sido afectados por el cutfreq.

---

## Evaluación de Rendimiento y Energía

Las  conclusiones  e  interpretaciones  de  los  siguiente  apartados  están  explicados  en  la  sección [Conclusiones](#conclusiones).

### Evaluación en Avignon

Análisis de rendimiento:  
![](/docs/images/Aspose.Words.a9a9ffdb-5f8d-4f84-9aa7-f3ac946d3b1f.010.png)

*(Tabla de datos de rendimiento...)*

Como se puede observar en el análisis de rendimiento de las funciones, con una tasa de instrucciones media de **2,01 para SOA y 2,005 para AOS** y un porcentaje de fallo en predicción de branch de media **0,37%  para  SOA  y  0,37%  para  AOS,**  ambas  configuraciones  muestran  un  rendimiento  similar, indicando que aprovechan el hardware de manera eficiente. La ligera ventaja de SOA podría deberse a un acceso a datos más optimizado para el tipo de operaciones en el programa.

*(Tabla de datos de energía...)*

### Evaluación en máquina local

A continuación, se disponen las gráficas del resultado del análisis de rendimiento en una máquina local, con más recursos que el nodo de Avignon en el que se ha hecho el análisis anterior. Se dispone de un AMD Ryzen 7 4700U. Todas las gráficas fueron ejecutadas en la versión “release”.

![](/docs/images/Aspose.Words.a9a9
