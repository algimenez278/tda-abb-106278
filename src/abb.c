#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

#define DERECHA 1
#define IZQUIERDA 0

//funcion util:
nodo_abb_t *nodo_arbol_crear(void* elemento){

	nodo_abb_t *nodo_nuevo = calloc(1, sizeof(nodo_abb_t));

	if(nodo_nuevo == NULL){
		return NULL;
	}

	nodo_nuevo->elemento= elemento;
	nodo_nuevo->izquierda= NULL;
	nodo_nuevo->derecha= NULL;

	return nodo_nuevo;
}




abb_t *insertar_nodo_rec(nodo_abb_t *nodo_actual, abb_t *arbol, void* elemento, int lugar){

	if(lugar== IZQUIERDA){
		if(nodo_actual->izquierda == NULL){
			nodo_actual->izquierda= nodo_arbol_crear(elemento);
			arbol->tamanio++;
			return arbol;
		}
		nodo_actual= nodo_actual->izquierda;
	}
	
	if(lugar== DERECHA){
		if(nodo_actual->derecha == NULL){
			nodo_actual->derecha= nodo_arbol_crear(elemento);
			arbol->tamanio++;
			return arbol;
		}
		nodo_actual= nodo_actual->derecha;
	}

	if(arbol->comparador(elemento, nodo_actual->elemento) <= 0){
		return insertar_nodo_rec(nodo_actual, arbol, elemento, IZQUIERDA);
	}

	return insertar_nodo_rec(nodo_actual, arbol, elemento, DERECHA);
}


void *quitar_nodo_rec(nodo_abb_t *nodo_actual, nodo_abb_t *nodo_padre, abb_t *arbol, void* elemento){
	
	if(nodo_actual == NULL){
		return NULL;
	}

	///si es hoja
	if(arbol->comparador(elemento, nodo_actual->elemento) == 0){
		if(nodo_actual->izquierda == nodo_actual->derecha == NULL){
			void *elemento_eliminado= nodo_actual->elemento;
			free(nodo_actual->elemento);
			free(nodo_actual);
			arbol->tamanio--;
			return elemento;
		}
	}

	if(arbol->comparador(elemento, nodo_actual->elemento) < 0){
		return quitar_nodo_rec(nodo_actual->izquierda, arbol, elemento);
	}

	return quitar_nodo_rec(nodo_actual->derecha, arbol, elemento);
}



void *buscar_nodo_rec(nodo_abb_t *nodo_actual, abb_t *arbol, void* elemento){

	if(nodo_actual == NULL){
		return NULL;
	}

	if(arbol->comparador(elemento, nodo_actual->elemento) == 0){
		return nodo_actual->elemento;
	}

	if(arbol->comparador(elemento, nodo_actual->elemento) < 0){
		return buscar_nodo_rec(nodo_actual->izquierda, arbol, elemento);
	}

	return buscar_nodo_rec(nodo_actual->derecha, arbol, elemento);
}





///////////////

void abb_destruir_nodo(abb_t *arbol, nodo_abb_t* nodo){

	if(nodo== NULL){
		return;
	}
	abb_destruir_nodo(arbol, nodo->izquierda);
	abb_destruir_nodo(arbol, nodo->derecha);
	free(nodo);
	arbol->tamanio--;
}

void abb_destruir_todo_nodo(abb_t *arbol, void (*destructor)(void *), nodo_abb_t *nodo){
	
	if(nodo== NULL){
		return;
	}

	abb_destruir_todo_nodo(arbol, destructor, nodo->izquierda);
	abb_destruir_todo_nodo(arbol, destructor, nodo->derecha);
	if(destructor){
		destructor(nodo->elemento);
	}	
	free(nodo);
	arbol->tamanio--;
}

//////////

abb_t *abb_crear(abb_comparador comparador)
{
	if(comparador == NULL){
		return NULL;
	}

	abb_t *arbol= calloc(1, sizeof(abb_t));

	if(arbol == NULL){
		return NULL;
	}

	arbol->nodo_raiz= NULL;
	arbol->comparador= comparador;
	arbol->tamanio= 0;

	return arbol;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if(arbol == NULL){
		return NULL;
	}

	if(arbol->tamanio == 0){
		arbol->nodo_raiz= nodo_arbol_crear(elemento);
		arbol->tamanio++;
		return arbol;
	}

	//si es menor o igual va a la izquierda

	if(arbol->comparador(elemento, arbol->nodo_raiz->elemento) <= 0){
		return insertar_nodo_rec(arbol->nodo_raiz, arbol, elemento, IZQUIERDA);
	}

	return insertar_nodo_rec(arbol->nodo_raiz, arbol, elemento, DERECHA);
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if( arbol== NULL){
		return NULL;
	}
	return elemento;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if(arbol == NULL || arbol->nodo_raiz == NULL){
		return NULL;
	}

	if(arbol->comparador(elemento, arbol->nodo_raiz->elemento) == 0){
		return arbol->nodo_raiz->elemento;
	}

	if(arbol->tamanio > 0){

		if(arbol->comparador(elemento, arbol->nodo_raiz->elemento) < 0){

			return buscar_nodo_rec(arbol->nodo_raiz->izquierda, arbol, elemento);
		}

		return buscar_nodo_rec(arbol->nodo_raiz->derecha, arbol, elemento);
	}

	return NULL;
}

bool abb_vacio(abb_t *arbol)
{
	return (arbol == NULL || arbol->tamanio == 0 || arbol->nodo_raiz == NULL);
}

size_t abb_tamanio(abb_t *arbol)
{
	if(arbol == NULL){
		return 0;
	}

	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	if(arbol == NULL){
		return;
	}
	if(arbol->nodo_raiz == NULL){
		free(arbol);
		return;		
	}
	abb_destruir_nodo(arbol, arbol->nodo_raiz->izquierda);
	abb_destruir_nodo(arbol, arbol->nodo_raiz->derecha);
	free(arbol->nodo_raiz);
	arbol->tamanio--;
	free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if(arbol == NULL){
		return;
	}
	if(arbol->nodo_raiz == NULL){
		free(arbol);
		return;		
	}

	abb_destruir_todo_nodo(arbol, destructor, arbol->nodo_raiz);
	free(arbol);
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	return 0;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	return 0;
}
