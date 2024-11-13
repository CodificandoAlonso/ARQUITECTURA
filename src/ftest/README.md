# Pruebas funcionales

En este directorio se encuentra el fichero de python en el que se han realizado las pruebas funcionales del programa.
Para ello, se ha hecho uso de la librería unittest de python. Para ejectuarlo, únicamente hace falta
ejecutar el siguiente comando EN AVIGNON:

```bash 
sbatch ftest.sh
```

Para gestionar las pruebas, se especifica en las primeras lineas de código del ftest.py las rutas de los ejecutables
necesarios para la ejecución de las pruebas. Estos deberían existir en su localicación correcta si se ha hecho uso 
del script build.sh para la compilación del proyecto.
