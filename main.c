#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Estructura para los grafos
typedef struct edgenode {
int src; /* origen*/
int y; /* destino del enlace */
int weight; /* coste del enlace */
struct edgenode *next; /* lista enlazada */
} edgenode_t;

// Estructura para el conjunto de nodos
typedef struct edgelist {
int node;
struct edgelist *next; /* lista enlazada */
} edgelist_t;

// Funcion para insertar enlaces en un grafo
edgenode_t *insertedges(edgenode_t *edges, int x, int y, int w, bool directed)  {
    edgenode_t *p;
    p= (edgenode_t *) malloc(sizeof(edgenode_t));
    p->src= x;
    p->weight= w;
    p->y= y;
    p->next= edges;
    edges= p; /* insercion al principio */

    //printf("Insertando enlace de X: %d a Y: %d con W: %d y direccion: %lld\n",x, y, w, edges);

    // Si es dirigido se añade otro enlace con entradas y salidas cambiadas
    if(directed == false) {
        edges= insertedges(edges, y, x, w, true);
    }
    //printf("%d \n", p->next);
    return edges;
}

// Funcion para leer el archivo que contiene el grafo
int readFile(char *fileName_, edgenode_t **lista, bool directed)
{
    FILE *fd_;
    int ret_, x, y, w;

    fd_ = fopen(fileName_,"r");

    if(fd_ > 0) {
		fscanf(fd_,"%d",&ret_);
		printf("Number of nodes is %d\n",ret_);

		while(fscanf(fd_,"%d,%d,%d",&x,&y,&w) == 3) {
            //printf("Insertando enlace de X: %d a Y: %d con W: %d\n",x, y, w);
            *lista= insertedges(*lista, x,y,w,directed);
		}
		fclose(fd_);
    } else {
		printf("Error - cannot open the input file %s\n",fileName_);
		ret_ =  -1;
    }

    return ret_;
}

// Funcion para inicializar el grafo
edgenode_t *initialize_graph(edgenode_t *edges) {
        return edges= NULL;
}
edgelist_t *initialize_list( int nvertices) {
        edgelist_t *list[nvertices];

        for (int i = 1; i <= nvertices ; i++)    {
            edgelist_t *listAux;

            listAux= (edgelist_t *) malloc(sizeof(edgelist_t));
            listAux->node= i;
            listAux->next= NULL;

            list[i]= listAux;
            // printf("lista: %d, %d\n",list[i]->node, i);
        }

        return list[nvertices];
}

// Funcion para contar los enlaces de una lista
int contarList(edgelist_t *list) {
    edgelist_t *aux;
    int n = 0;

    aux = list;
    // Recorro toda la lista sumando 1 al contador por cada iteracion realizada
    while (aux != NULL) {
        n++;
        aux = aux->next;
    }
    return n;
}

// Funcion para contar los enlaces del mst
int contarEnlacesNodo(edgenode_t *mst, int node, int vertices) {
  edgenode_t *aux;
  int contador= 0;

  for ( int i = 1; i <= vertices; i++)    {
      aux = mst;

      while (aux != NULL) {
          if(aux->src == node){
              contador++;
          }
          aux = aux->next;
      }
      printf("\n");
  }
  return contador;
}

// Funcion para imprimir el conjunto que contiene los enlaces
void imprimirList(int vertices, edgelist_t *edgelist[99])    {
    edgelist_t *aux[vertices];
    int i;
    printf("Los nodos estan unidos a los siguientes: \n");
    for ( i = 1; i <= vertices; i++)    {
        aux[i] = edgelist[i];
        printf("%d: ", i);

        while (aux[i] != NULL) {
            printf("(Node: %d )  ", aux[i]->node);
            aux[i] = aux[i]->next;
        }
        printf("\n");
    }
}


// Funcion para unir dos nodos en el conjunto
edgelist_t *unirList(edgelist_t *list[99], int a, int b, int nvertices) {
    edgelist_t *p, *q;

    printf("\n A: %d, B: %d \n",a, b);

    // Si el conjunto en el nodo origen no solo se contiene a si mismo
    if(list[a]->next != NULL && list[b]->next == NULL)    {
        // Hago una insercion al final con la informacion del nodo de destino
        q = (edgelist_t *) malloc(sizeof(edgelist_t));
        q->node = list[b]->node;
        q->next = NULL; // Siguiente a nulo ya que va a ser el ultimo

        p = list[a];

        // Recorro el puntero auxiliar buscando el ultimo elemento
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = q; // Cuando le encuentro añado el enlace al final

        list[b]= list[a]; // Igualo el puntero al nodo que se esta uniendo en destino

    } else if(list[a]->next != NULL && list[b]->next != NULL)	{
        // Si tanto en origen como en el final las listas contienen a mas de un nodo se fusionan las dos listas

		int contiene[nvertices]; // Para ello creamos una variable auxiliar en la cual marcaremos los nodos que contienen ambas listas (para asi posteriormente asignarlas el mismo puntero)
		edgelist_t *w, *k, *t;

        // Se inicializa la variable con todas las componentes a 0
		for(int i = 1 ; i <= nvertices ; i++)	{
			contiene[i]= 0;
			// printf("Contiene %d: %d \n", i, contiene[i]);
		}
        // Recorremos la lista del nodo origen
		k= list[a];
		while (k != NULL) {
			contiene[k->node]= 1; // Se guarda el nodo que contiene la lista en la variable auxiliar
			// printf("Contiene %d: %d \n", k->node, contiene[k->node]);
            k = k->next;
        }

        // Recorremos la lista del nodo destino
		w= list[b];
		while (w != NULL) {
			contiene[w->node]= 1; // Se guarda el nodo que contiene la lista en la variable auxiliar
			// printf("Contiene %d: %d \n", w->node, contiene[w->node]);
            w = w->next;
        }

        // Busco la ultima componente de la lista para realizar una insercion al final
        t= list[a];
		while (t->next != NULL) {
            t = t->next;
        }

		t->next= list[b]; // Realizo la insercion al final

        // Sobreescribo el puntero en todos los nodos previamente calculados
		for(int i = 1 ; i <= nvertices ; i++)	{
			if(contiene[i] == 1)	{
				list[i]= list[a];
			}
		}
	} else  {
        // Si el nodo origen solo se contiene a si mismo realizo lo mismo pero cambiando la informacion por la informacion de origen (por si acaso esta lista no esta vacia)
        q = (edgelist_t *) malloc(sizeof(edgelist_t));
        q->node = list[a]->node;
        q->next = NULL;

        p = list[b];

        while (p->next != NULL) {
            p = p->next;
            }
        p->next = q;

        list[a]= list[b];
    }
}

// Funcion para imprimir grafo
void imprimirLista(int vertices, edgenode_t *edgenode)    {
    edgenode_t *aux;
    int i;
    printf("Topologia de la red \n");
    for ( i = 1; i <= vertices; i++)    {
        aux = edgenode;
        printf("Enlaces de %d: ", i);
        while (aux != NULL) {
            if(aux->src==i){
                printf("(D: %d C: %d), ", aux->y, aux->weight);
            }
            //printf("(O: %d D: %d C: %d), \n", aux->src, aux->y, aux->weight);
            aux = aux->next;
        }
        printf("\n");
    }
}

// Funcion para borrar un elemento del grafo
edgenode_t *borrar(edgenode_t *edges, edgenode_t *max)  {
    edgenode_t *aux, *prev;

    aux= edges;
    prev= NULL;

    while (aux != NULL) {
        // BUsco la direccion a borrar en la lista
        if (aux == max)   {
            // Cuando enciuentro la direccion
            if(prev == NULL)    {
                // Si la anterior direccion es null (es decir estas en la primera iteracion)
                edges= aux->next;
            }else   	{
                prev->next = max->next; // Si no es nula entonces prev tendra valor, entonces la siguiente direccion es la que hay que saltar
            }
            free(max); // Libero el espacio en memoria
            return edges;
        }
        prev= aux;
        aux = aux->next;
    }
}

// Funcion para contar los enlaces de un grafo
int contarEnlaces(edgenode_t *edges) {
    edgenode_t *aux;
    int n = 0;

    aux = edges;
    // Recorro toda la lista sumando 1 al contador por cada iteracion realizada
    while (aux != NULL) {
        n++;
        aux = aux->next;
    }
    return n;
}

// Funcion para ordenar la lista de menor a mayor
edgenode_t *ordenar(edgenode_t *edgescopy)  {
    edgenode_t *aux, *max, *listaOrdenada;
    int maxX, maxY, numEnlaces;
    bool directed = false;
    int maxweight = -99;

    listaOrdenada= initialize_graph(listaOrdenada);
    // Cuento cuantos enlaces tiene el grafo para usarlo como criterio de parada en el bucle
    numEnlaces= contarEnlaces(edgescopy);

    for(int i = 1 ; i <= numEnlaces ; i++){
        aux= edgescopy;
        maxweight = -99;
        // Busco el mayor elemento y guardo los valores
        while (aux != NULL) {
            if (aux->weight >= maxweight)   {
                max= aux;
                maxweight= aux->weight;
                maxX= aux->src;
                maxY= aux->y;
            }
           aux = aux->next;
        }
        // Inserto el mayor elemento en la lista ordenada
        listaOrdenada= insertedges(listaOrdenada, maxX, maxY, maxweight, directed);
        // Elimino el nodo maximo de la lista de la que se obtienen los maximos
        edgescopy= borrar(edgescopy, max);
    }

    // imprimirLista(6, listaOrdenada);
    return listaOrdenada;
}

// Funcion Kruskal
edgenode_t *kruksal(edgenode_t *edges, int nvertices, edgelist_t *list[99])     {
    edgenode_t *aux, *mst, *mstPrev;
    bool directed = false;
    int src, y, weight, numEnlaces;
    int contador= 1;

    aux = ordenar(edges); // Ordeno el grafo original de menor a mayor
    mst= initialize_graph(mst); // Inicializo como vacio el mst
	  numEnlaces= contarEnlaces(aux); // Numero de enlaces del grafo

    while (aux != NULL) {

        if(mst == NULL) {
            // Si el mst esta vacio añado automaticamente el primer enlace que esta en la lista ordenada, ya que es el que tiene un peso menor
            printf("El enlace que se esta añadiendo es --> O: %d, D: %d, W: %d \n",aux->src, aux->y, aux->weight);
            mst= insertedges(mst, aux->src, aux->y, aux->weight, directed);
            unirList(list, aux->src, aux->y, nvertices); // Uno esa pareja de nodos en el conjunto

            imprimirList(nvertices, list);
            mstPrev= mst;
        }else   {
            // Si el NO mst esta vacio compruebo si el enlace no esta unido
            mstPrev= mst;
            while(mstPrev != NULL)  {
                // Si los punteros de origen y destino son diferentes y todavia no he añadido n-1 enlaces
                if( list[aux->src] != list[aux->y] && contador <= numEnlaces-1)  {

                    mst= insertedges(mst, aux->src, aux->y, aux->weight, directed); // Añado ese nodo al mst
					unirList(list, aux->src, aux->y, nvertices); // Uno esa pareja de nodos en el conjunto

                    imprimirList(nvertices, list);
                    printf("El enlace que se esta añadiendo es --> O: %d, D: %d, W: %d \n",aux->src, aux->y, aux->weight);
                }

                //printf("Es el siguiente %d\n", mst->src);
                mstPrev= mstPrev->next;
            }
        }
        aux= aux->next;
		  contador++; // Criterio de que los enlaces a añadir son n-1 enlacees como maximo
    }
    return mst;
}

void main(){
    edgenode_t *edges, *mst; /* informacion adyacencia */
    edgelist_t  *list[99]; /* conjunto que contiene los nodos */
    int nvertices; /* numero de vertices */
    int nedges; /* numero de enlaces */
    bool directed; /* grafo dirigido? */
    directed= false; // Grafo no dirigido
    char grafoTxt[100]; // Nombre del archivo que contiene el grafo
    int numAux, numAux2; // Variables para comprobar si existe MST o no

    edges= initialize_graph(edges);

    printf("Nombre del grafo a leer: "); // Pido al usuario el nombre del archivo
    scanf("%s",grafoTxt);

    nvertices= readFile(grafoTxt, &edges, directed); // Leo el archivo

        if (nvertices < 0){
          exit (EXIT_FAILURE); // Si es menor que 0 nodos se detiene la ejecución
        }
        else  {
        // Inicializo el conjunto que contiene los nodos que estan unido o no
          for (int i = 1; i <= nvertices; i++)  {
                edgelist_t * listAux;
                listAux = (edgelist_t *) malloc (sizeof (edgelist_t));
                listAux->node = i;
                listAux->next = NULL;
                list[i] = listAux;
          }

          printf ("El grafo original es: \n");
          imprimirLista (nvertices, edges);


          mst = kruksal (edges, nvertices, list);

          // Comprobar si existe arbol mínimo
          numAux2= 0; // Inicializo la variable aux
          for (int i = 1; i <= nvertices; i++)  { // Recorro todo el MST
            numAux= 0;
            numAux= contarEnlacesNodo(mst, i, nvertices);
            if (numAux != 0) { // Si existe algun enlace
              numAux2++; // Añado uno a la variable auxiliar, lo que significa que para ese nodo contiene algunb enlace por lo que pertenece al arbol.
            }
          }

          if (numAux2 == nvertices) { // Si todos los nodos tienen al menos un enlace es que tiene solución
            printf ("\n\nLa solucion del algoritmo de kruskal es: \n");
            imprimirLista (nvertices, mst);
          } else  {
            printf ("\n\nNo existe arbol mínimo\n");
          }


    }
}