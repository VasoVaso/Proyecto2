Link al video:

****CONSIDERACIONES****

-El programa fallará si se introduce un dato distinto al solicitado, por ejemplo, si se solicita un int y se introduce un char.

-La actualización de la jerarquía no está implementada.

-Los guardianes sobre nivel 90 no pueden ser seleccionados y tampoco pierden o ganan nivel, esto para mantener su rango de guardianes del reino.

-Las ciudades válidas manipulables por archivo serán solo las siguientes:
Tesla Capital
Capital City
Forest City
Cliff City
Desert City
Silverstone City
River City
Mountain City
Sunflower Fields City
Hill City
Valley City
Storm City
Coast City
Island City

****DESCRIPCIÓN DE FUNCIONES****

-> FUNCIONES DEL GRAFO

-addEdge(): se agrega una arista en ambas direcciones al grafo no dirigido representado mediante una lista de adyacencia.
-addConnection(): se verifica si el vértice 1 ya está en el grafo. Luego, adjacencyList.find(city1) intenta encontrar city1 en el mapa; si find no llega al final del mapa, 
significa que el vértice 1 existe. Entonces, se utiliza find para buscar city2 en la lista de adyacencia de city1. Si find no llega al final de la lista de adyacencia, 
significa que el vértice 2 no existe en la lista de adyacencia de city1 y, por lo tanto, la conexión no existe. Si la conexión no existe, se agrega city2 a la lista de adyacencia de 
city1 y viceversa para que la conexión sea bidireccional. Si city no existe en el grafo, se informa que la ciudad no existe. Si city2 ya existe en la lista de adyacencia de city, 
se informa que la conexión ya existe.
-areCitiesConnected(): se verifica si la primera ciudad está presente en el grafo. adjacencyList.find(city1) devuelve un iterador que apunta al elemento correspondiente a city1 en la lista 
de adyacencia. Si find no encuentra la ciudad, el iterador es igual a adjacencyList.end(). Luego, Obtiene la lista de vecinos de la primera ciudad. adjacencyList[city1] devuelve una 
referencia constante a la lista de vecinos de city1. Se utiliza la función find para buscar la segunda ciudad dentro de la lista de vecinos de la primera ciudad. Finalmente se evalúa
si la comparación es falsa o verdadera.
-getPath(): todas las distancias se establecen inicialmente en infinito, excepto la distancia a la ciudad de inicio, que se establece en 0. Se añade la ciudad de inicio con una 
distancia de 0 a la cola de prioridad. Mientras la cola de prioridad no esté vacía, se extrae el vértice con la menor distancia actual. Después de que Dijkstra ha terminado, el camino 
más corto se reconstruye retrocediendo desde la ciudad de destino hasta la ciudad de inicio utilizando el mapeo "previous". La función devuelve un vector que contiene el orden de las 
ciudades por las que hay que viajar desde la ciudad de inicio hasta la ciudad destino.
Para cada vecino del vértice actual, se actualiza la distancia si se encuentra un camino más corto.
-printGraph(): se itera a través de cada par clave-valor en la lista de adyacencia. Luego, se imprime el nombre del vértice actual y se vuelve iterar a través de la lista de adyacencia 
del vértice actual para imprimir cada vértice adyacente.

-> ÁRBOL BINARIO

-insertRankNode(): creación de nodos del árbol binario. Se recibe la raíz del árbol y un elemento guardián. Se insertan los nodos de manera recursiva, según el nivel del guardián recibido. 
-createRankTree(): se recibe la lista de guardianes. En un bucle, se tomará cada elemento guardián de la lista de guardianes para realizar inserción de nodos.
-printRankTree(): se recibe la raíz y se imprime recursivamente el árbol binario con el método in-order.
-freeRankTree(): se elimina recursivamente la raíz del árbol binario hasta eliminar todos los nodos.

-> ÁRBOL GENERAL

-createHierarchyTree(): se recibe la lista de guardianes y el guardián de nivel 100. Se identifica al maestro guardián y luego se realiza otro bucle para recorrer la lista de guardianes 
nuevamente para comparar el nombre del posible aprendiz, si hay coincidencia, se agrega a la lista de aprendices de ese maestro.
-printHierarchyTree(): se utiliza un bucle para imprimir tabulaciones según el nivel de indentación para visualizar de mejor manera el árbol. La función se llama de manera recursiva 
para cada aprendiz del nodo actual.
-freeHierarchyTree(): se eliminan recursivamente los nodos del arbol a partir de los aprendices.

-> LECTURA DE ARCHIVOS

-readCitiesFile(): se utiliza getline para extraer cada línea del archivo y almacenarla en la variable line. Después de obtener una línea, se crea un istringstream llamado iss a partir 
de esa línea. La línea contiene dos ciudades separadas por una coma, por lo que se utiliza getline nuevamente para extraer ambas ciudades. Finalmente, después de procesar una línea, 
la función llama al método addEdge del grafo para agregar una arista, conectando las dos ciudades.  Una vez que se han procesado todas las líneas del archivo, se cierra el archivo y 
se retorna true para indicar que la operación de lectura fue exitosa. Adicionalmente, se revisa que la ciudad sea válida mediante la función getCityID, donde se retornará un -1 si la 
ciudad no es válida, posteriormente invalidando el juego cambiando la variable global validGame a false.
-readGuardiansFile(): se utiliza istringstream para crear un flujo de cadena a partir de la línea actual del archivo. Se declaran variables locales para almacenar temporalmente los 
datos de cada guardián. Adicionalmente se crea el atributo cityID para determinar que la ciudad existe dentro del archivo de los guardianes, esto se corrobora con la función getCityID.
Utilizando el flujo de cadena, se extraen los datos de la línea hasta la próxima coma o hasta el final de la línea. Se crea un nuevo objeto Guardian utilizando los datos extraídos y 
se agrega a la lista de guardianes. Además, se crean variables para ir corroborando que el archivo sea válido, en caso de detectar una irregularidad, el bool global validGame se torna 
falso. Una vez que se han leído todas las líneas del archivo, se cierra el archivo. Finalmente, la función devuelve la lista de guardianes construida a partir de la información 
leída del archivo.

-> OTRAS FUNCIONES

-showGuardianList(): se recibe la lista de guardianes y se recorre para luego mostrar solamente el nombre de cada guardián.
-getGuardianInfo(): se recibe la lista de guardianes y el índice de un guardián en específico. Este indíce es revisado en la lista y enviado a la siguiente función para ver todos sus datos.
-showGuardianInfo(): se recibe el guardián indicado anteriormente a partir de su índice y se muestran sus datos.
-selectGuardian(): se recibe la lista de guardianes y el nombre del guardián que se va a utilizar. Se compara el nombre recibido con la lista de guardianes, si existe relación, se retorna
ese guardián a un puntero.
-isMaster(): bool donde se compara el guardián recibido con la lista de guardianes, si hay relación entre el string del guardián recibido y el atributo "master" de otro guardián, 
se retorna true.
-getCityGuardians(): se compara el atributo "city" del guardián recibido con la lista de guardianes. Si existe relación, se guardarán en un vector todos los guardianes que compartan 
la misma ciudad.
-selectRival(): se selecciona el guardián a combatir que guarda relación con la ciudad del guardián seleccionado, para esto se recibe el vector de guardianes con la misma ciudad. 
Luego, mediante índice se elige al rival y se retornan sus datos a un puntero.
-getCityID(): función para reconocer si una ciudad es válida. Para esto se asigna un número según el string de la ciudad. Si el valor retornado es -1, la ciudad no es válida.
-getRankString(): función para convertir el atributo "rank" del guardián a un string.
-startFight(): se resuelve el resultado del dado, si se obtiene 5 o 6 gana el retador. La cantidad de puntos otorgados al vencedor dependerá de si es maestro o no. Adicionalmente 
se establecen los límites de los niveles.