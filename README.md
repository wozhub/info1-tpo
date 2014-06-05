# **Informática I** (Curso R1041)
## Trabajo práctico obligatorio.
## Juego de Chinchón en red.

## Integrantes 
* **Iannuzzi**, Franco. 
* **Ranú**, Agustín. 
* **Vinazza**, David.

\pagebreak

## Introducción

### Objetivos

Aplicar los conocimientos teóricos y prácticos adquiridos en la materia como base para desarrollar un programa, en este caso un juego de chinchón en red.

### Conceptos Previos

Representación de números y caracteres. Números aleatorios. Estructuras. Memoria dinámica. Listas. Ordenamiento de arreglos. Argumentos por línea de comando. Manejo de archivos. Sockets. Estrategia Top-Down.

### Herramientas utilizadas

El código se desarrolló utilizando *Kate* y *Vim* de forma indistinta. Se compiló en un entorno Linux mediante *Gcc*. Además se utilizó la aplicacióń *UberWriter* para escribir este informe en formato markdown y latex, junto con la herramienta web *draw.io* para el diagrama de flujo de la máquina de estados.

\pagebreak

## Práctica

Para comenzar el proyecto nos familiarizamos con el juego de chinchón y comenzamos a pensar como instrumentarlo. Nos propusimos utilizar la estrategia *top-down*, separando el curso del juego en distintas etapas, que a su vez podrían depender de distintas funciones y estructuras. Esto nos permitió desarrollar por separado la interfaz y el funcionamiento abstracto del juego. Hecho esto pudimos probar aspectos puntuales del juego mediante programas de pruebas.

Así, desarrollamos primero una estructura *carta* como elemento de una lista, luego una estructura *mazo* para abstraer la lista y administrar las cartas de forma sencilla. A partir de ahí desarrollamos funciones para repartir cartas aleatorias y ordenar juegos.

Una vez tuvimos lo anterior funcionando, nos pusimos a trabajar en las funcionalidades de red. Cuando pudimos enviar y recibir datos, pudimos condensar todo en una *máquina de estados* es decir, un modelo de lógica a partir de entradas y salidas de datos que nos permitió diagramar los comportamientos del *servidor* y el *cliente*.

Para no salirnos de una estructura de proceso único, tuvimos que adaptar algunas reglas del juego para evitar simulateidad. Precisamente, al final de cada ronda cuando los jugadores en el chichón original pueden agregar las cartas que tengan libres a juegos de otros jugadores.

Lo primero que armamos cuando tuvimos las funciones necesarias fue el servidor, dado que su comportamiento debía ser *automático*, es decir, una vez iniciado no debía requerir entrada por parte del usuario.

En el desarrollo del cliente para simplificar la interfaz del juego utilizamos la librería ncurses que provee abstracciones para manejar 'ventanas' y la entrada por teclado. Esto nos permitió simplificar las tareas de selección de cartas.

### Detección y verificación de juegos

Encontramos bastante difícil hacer que la computadora interprete cuales son los juegos que tiene el jugador, en tanto puede o no tratarse de *el mejor juego posible*. Para simplificar, pedimos al usuario que decida cuales cartas pertenecen a cada juego.

Para detectar que tipo de juego es, ordenamos las cartas y comparamos las primeras dos. Si son del mismo palo, entonces deberá tratarse de una escalera. Si son de palos distintos, entonces deberá ser un juego de cartas del mismo valor.

Cada juego debe tener por lo menos dos cartas y como máximo un comodín. También hay que considerar que en el juego de chinchón los comodines valen 25 puntos, por tanto en el ordenamiento siempre van a quedar últimos.

### Ordenamiento de Cartas

Para verificar los juegos de cartas primero los ordenamos mediante el método burbuja o burbujeo. Este mecanismo es suficiente, considerando que los juegos son arreglos de pocas cartas, y que a su vez suelen estar bastante ordenados de por si, considerando la cantidad posible de cartas y la distancia mayor entre el valor de una y otra.

#### Detección de un juego de cartas del mismo palo, en escalera

Al ordenar un arreglo de cartas, si se trata de un juego de escalera las primeras dos deberán ser del mismo palo y consecutivas, y así hasta la última, admitiéndose un salto si el juego tuviese un comodín.

#### Detección de un juego de cartas de distintos palos, mismo valor

En caso de no ser un juego de escalera, entonces deberá verificarse la condición de que todas las cartas tienen el mismo valor o son un comodín.

#### Detección de chinchón

La detección del chinchón es bastante simple pues verifica ser un juego de 7 cartas sin comodines.

### Máquinas de estados

El comportamiento del servidor y el cliente esta basado en una máquina de estados. Adjuntamos un diagrama aproximado de su funcionamiento que utilizamos como guía al momento de desarrollar el código.

Por cuestiones de espacio y simplicidad, el diagrama no contempla "estados" menores, que no dependen de intercambio de información con el servidor.

### Envío de estructuras por red

Durante la investigación previa al desarrollo del proyecto descubrimos que en el campo profesional se utiliza un mecanismo llamado *serialización* para asegurar que tanto el emisor como el receptor entiendan la información del mismo modo, evitando los problemas que podrían surgir en la comunicación de dos sistemas con plataformas diferentes. [^1]

Sabido esto, consideramos la *serialización* no es parte del propósito de este trabajo práctico por tanto pertenece a contenidos más avanzados. 

[^1]: Dependiendo de la plataforma pueden existir diferentes *alignments* y *endianness*.

\pagebreak 

## Conclusión

La dificultad en el desarrollo de una aplicación crece conforme se requiere que distintas lógicas que son de alguna forma independientes se relacionen entre si. A su vez, intercomunicar procesos locales o remotos trae consigo la necesidad de planificar más profundamente el recorrido de cada programa.

La idea de desarrollar los distintos componentes por separado nos permitió probar su funcionamiento aislado, pero a la hora de conjugar todo en un solo programa, surgieron algunas discordancias. En ese momento, la extensión del código hizo más dificil debuggearlo.

Por eso mismo hay que ser bastante prolijos a la hora de utilizar memoria o esperar entrada por parte del usuario, ya que cualquier detalle que omitamos puede convertirse en un problema a la hora de concatenar funciones.

Aunque no pudimos probarlo aún, creemos que el código del programa puede adaptarse fácilmente para simular otros juegos de cartas de turnos no simultáneos, como pueden ser el truco o la escoba del quince, dado que las funciones independientes al juego estan separadas. A su vez, la máquina de estados basada en intercambio de señales y la función *switch* hace que diagramar el funcionamiento de los juegos sean relativamente simples.

## Bibliografía y Material consultado

Apuntes de la cátedra. Material en el grupo de yahoo. Guía de trabajos prácticos resuelta por los integrantes del grupo durante la cursada. Man pages. Sitio: Stackoverflow.com. Manuales Beej.