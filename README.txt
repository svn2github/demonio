Proyecto: Demonio - Herramienta de administraci�n remota.
versi�n: svn
Autor: Alberto Pajuelo Montes
Email: paju1986@gmail.com

INSTRUCIONES DE COMPILACI�N.

La forma m�s f�cil de compilar es abrir los ficheros .pro tanto del cliente
como del servidor desde el QtCreator.
Recordar que el c�digo fuente est� preparado para compilaci�n est�tica y si
se quiere compilar de forma din�mica hay que tener cuidado con los plugins ya
que probablemente habr� que quitar el #include <QtPlugin> y los Q_IMPORT_PLUGIN 
que se hayan definido antes del main.

El servidor est� preparado para ejecutarse en localhost de forma directa, si
se quiere un servidor configurable desde el men� de configuraci�n del cliente
se debe descomentar la parte de c�digo as� indicada en el c�digo fuente y renombrar
el ejecutable a rawserver.dat y moverlo al mismo directorio del cliente.