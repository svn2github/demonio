Proyecto: Demonio - Herramienta de administración remota.
versión: svn
Autor: Alberto Pajuelo Montes
Email: paju1986@gmail.com

INSTRUCIONES DE COMPILACIÓN.

La forma más fácil de compilar es abrir los ficheros .pro tanto del cliente
como del servidor desde el QtCreator.
Recordar que el código fuente está preparado para compilación estática y si
se quiere compilar de forma dinámica hay que tener cuidado con los plugins ya
que probablemente habrá que quitar el #include <QtPlugin> y los Q_IMPORT_PLUGIN 
que se hayan definido antes del main.

El servidor está preparado para ejecutarse en localhost de forma directa, si
se quiere un servidor configurable desde el menú de configuración del cliente
se debe descomentar la parte de código así indicada en el código fuente y renombrar
el ejecutable a rawserver.dat y moverlo al mismo directorio del cliente.