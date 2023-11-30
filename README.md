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

-> ÁRBOL BINARIO

insertRankNode(): creación de nodos del árbol binario. Se recibe la raíz del árbol y un elemento guardián. Se insertan los nodos de manera recursiva según el nivel del guardián recibido. 
createRankTree(): se recibe la lista de guardianes. En un bucle, se tomará cada elemento guardián de la lista para insertar nodos.
printRankTree(): se recibe la raíz y se imprime el árbol binario con el método in-order.
freeRankTree(): se elimina recursivamente la raíz del árbol binario hasta eliminar todos los nodos.

-> ÁRBOL GENERAL

createHierarchyTree(): se recibe la lista de guardianes y el guardián de nivel 100. Luego, a partir del guardián que se encuentra en la cúspide de la jerarquía, se recorre la lista de guardianes y se asignan los aprendices a sus respectivos maestros.
printHierarchyTree(): función recursiva que se encarga de imprimir la jerarquía de maestros y aprendices. Se utiliza una tabulación para representar de mejor manera el árbol.
freeHierarchyTree(): se eliminan recursivamente los nodos del arbol a partir de los aprendices.

-> OTRAS FUNCIONES

showGuardianList(): se recibe la lista de guardianes y se recorre para luego mostrar el nombre de cada guardián.
getGuardianInfo(): se recibe la lista de guardianes y el índice de un guardián en específico. Este indíce es revisado en la lista y enviado a la siguiente función para ver todos sus datos.
showGuardianInfo(): se recibe el guardián indicado anteriormente y se muestran sus datos.
selectGuardian(): se recibe la lista de guardianes y el nombre del guardián que se va a utilizar. Si válido, se retorna el elemento guardián.
isMaster(): bool donde se compara el guardián recibido con la lista de guardianes, si hay relación entre el string del guardián recibido y el atributo "master" de otro guardián, se retorna true.
getCityGuardians(): se compara el atributo "city" del guardián recibido con la lista de guardianes. Si existe relación, se guardarán en un vector todos los guardianes que compartan la misma ciudad.
selectRival(): se selecciona el guardián a combatir que guarda relación con la ciudad del guardián seleccionado, para esto se recibe el vector de guardianes con la misma ciudad. Luego, mediante índice se elige al rival.
getCityID(): función para reconocer si una ciudad es válida. Para esto se asigna un número según el string de la ciudad. Si el valor retornado es -1, la ciudad no es válida.
getRankString(): función para convertir el atributo "rank" del guardián a un string.
startFight(): se resuelve el resultado del dado, si se obtiene 5 o 6 gana el retador. La cantidad de puntos otorgados al vencedor dependerá de si es maestro o no. Adicionalmente se limita el nivel inferior y el superior.