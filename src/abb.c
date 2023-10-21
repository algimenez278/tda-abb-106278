#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

#define DERECHA 1
#define IZQUIERDA 0

//PRE: -
//POST: Crea un nodo con el elemento recibido.
nodo_abb_t *nodo_arbol_crear(void *elemento)
{
	nodo_abb_t *nodo_nuevo = calloc(1, sizeof(nodo_abb_t));

	if (nodo_nuevo == NULL) {
		return NULL;
	}

	nodo_nuevo->elemento = elemento;

	return nodo_nuevo;
}

//PRE: -
//observación: nodo y arbol deben ser distintos de NULL,
// pero eso es chequeado previamente a invocar esta funcion, en abb_insertar.
//POST: Inserta un nodo en el árbol en el lugar correspondiente, con el elemento recibido.
abb_t *insertar_nodo_rec(nodo_abb_t *nodo_actual, abb_t *arbol, void *elemento,
			 int lugar)
{
	if (lugar == IZQUIERDA) {
		if (nodo_actual->izquierda == NULL) {
			nodo_actual->izquierda = nodo_arbol_crear(elemento);
			arbol->tamanio++;
			return arbol;
		}
		nodo_actual = nodo_actual->izquierda;
	}

	if (lugar == DERECHA) {
		if (nodo_actual->derecha == NULL) {
			nodo_actual->derecha = nodo_arbol_crear(elemento);
			arbol->tamanio++;
			return arbol;
		}
		nodo_actual = nodo_actual->derecha;
	}

	if (arbol->comparador(elemento, nodo_actual->elemento) <= 0) {
		return insertar_nodo_rec(nodo_actual, arbol, elemento,
					 IZQUIERDA);
	}

	return insertar_nodo_rec(nodo_actual, arbol, elemento, DERECHA);
}

//PRE: nodo no debe ser NULL
//POST: Devuelve el predecesor inorden del nodo recibido.
nodo_abb_t *buscar_predecesor(nodo_abb_t *nodo)
{
	nodo_abb_t *predecesor = nodo->izquierda;
	while (predecesor->derecha) {
		predecesor = predecesor->derecha;
	}
	return predecesor;
}

//PRE: -
//POST: Quita del arbol el nodo que contiene al elemento recibido.
// En caso de no ser nodo hoja lo reemplaza por su hijo o su predecesor.
void *quitar_nodo_rec(nodo_abb_t *nodo, abb_t *arbol, void *elemento,
		      void **elemento_eliminado)
{
	if (arbol == NULL || nodo == NULL) {
		return NULL;
	}

	if (arbol->comparador(elemento, nodo->elemento) == 0) {
		*elemento_eliminado = nodo->elemento;

		if (nodo->izquierda != NULL && nodo->derecha != NULL) {
			nodo_abb_t *predecesor = buscar_predecesor(nodo);
			void *elemento_a_eliminar = nodo->elemento;
			nodo->elemento = predecesor->elemento;
			predecesor->elemento = elemento_a_eliminar;
			nodo->izquierda = quitar_nodo_rec(nodo->izquierda,
							  arbol,
							  predecesor->elemento,
							  elemento_eliminado);
			return nodo;
		} else {
			nodo_abb_t *izquierda = nodo->izquierda;
			nodo_abb_t *derecha = nodo->derecha;
			free(nodo);
			nodo = NULL;
			arbol->tamanio--;

			if (izquierda) {
				return izquierda;
			}

			return derecha;
		}
	}

	else if (arbol->comparador(elemento, nodo->elemento) < 0) {
		nodo->izquierda = quitar_nodo_rec(nodo->izquierda, arbol,
						  elemento, elemento_eliminado);
	}

	else {
		nodo->derecha = quitar_nodo_rec(nodo->derecha, arbol, elemento,
						elemento_eliminado);
	}

	return nodo;
}

//PRE: -
//POST: Devuelve el elemento del nodo que es igual al elemento recibido.
void *buscar_nodo_rec(nodo_abb_t *nodo_actual, abb_t *arbol, void *elemento)
{
	if (arbol == NULL || nodo == NULL) {
		return NULL;
	}

	if (arbol->comparador(elemento, nodo_actual->elemento) == 0) {
		return nodo_actual->elemento;
	}

	if (arbol->comparador(elemento, nodo_actual->elemento) < 0) {
		return buscar_nodo_rec(nodo_actual->izquierda, arbol, elemento);
	}

	return buscar_nodo_rec(nodo_actual->derecha, arbol, elemento);
}

//PRE: -
//POST: Destruye los nodos del arbol, liberando la memoria reservada.
void abb_destruir_nodo(abb_t *arbol, nodo_abb_t *nodo)
{
	if (arbol == NULL || nodo == NULL) {
		return;
	}

	abb_destruir_nodo(arbol, nodo->izquierda);
	abb_destruir_nodo(arbol, nodo->derecha);
	free(nodo);
	arbol->tamanio--;
}

//PRE: -
//POST: Destruye los nodos del arbol, liberando la memoria reservada,
// y aplicando la funcion destructor en caso de que la misma no sea NULL.
void abb_destruir_todo_nodo(abb_t *arbol, void (*destructor)(void *),
			    nodo_abb_t *nodo)
{
	if (nodo == NULL) {
		return;
	}

	abb_destruir_todo_nodo(arbol, destructor, nodo->izquierda);
	abb_destruir_todo_nodo(arbol, destructor, nodo->derecha);
	if (destructor) {
		destructor(nodo->elemento);
	}
	free(nodo);
	arbol->tamanio--;
}

//PRE: -
//POST: Recorre el arobol inorden y aplica la función f a cada elemento.
bool abb_con_cada_elemento_inorden(nodo_abb_t *nodo, bool(f)(void *, void *),
				   void *aux, size_t *cant_funcion_invocada)
{
	if (nodo == NULL) {
		return true;
	}

	if (abb_con_cada_elemento_inorden(nodo->izquierda, f, aux,
					  cant_funcion_invocada) == false) {
		return false;
	}

	(*cant_funcion_invocada)++;
	if (f(nodo->elemento, aux) == false) {
		return false;
	}

	return abb_con_cada_elemento_inorden(nodo->derecha, f, aux,
					     cant_funcion_invocada);
}

//PRE: -
//POST: Recorre el arobol preorden y aplica la función f a cada elemento.
bool abb_con_cada_elemento_preorden(nodo_abb_t *nodo, bool(f)(void *, void *),
				    void *aux, size_t *cant_funcion_invocada)
{
	if (nodo == NULL) {
		return true;
	}

	(*cant_funcion_invocada)++;
	if (f(nodo->elemento, aux) == false) {
		return false;
	}

	if (abb_con_cada_elemento_preorden(nodo->izquierda, f, aux,
					   cant_funcion_invocada) == false) {
		return false;
	}

	return abb_con_cada_elemento_preorden(nodo->derecha, f, aux,
					      cant_funcion_invocada);
}

//PRE: -
//POST: Recorre el arobol inorden y aplica la función f a cada elemento.
bool abb_con_cada_elemento_postorden(nodo_abb_t *nodo, bool(f)(void *, void *),
				     void *aux, size_t *cant_funcion_invocada)
{
	if (nodo == NULL) {
		return true;
	}

	if (abb_con_cada_elemento_postorden(nodo->izquierda, f, aux,
					    cant_funcion_invocada) == false) {
		return false;
	}

	if (abb_con_cada_elemento_postorden(nodo->derecha, f, aux,
					    cant_funcion_invocada) == false) {
		return false;
	}

	(*cant_funcion_invocada)++;
	return f(nodo->elemento, aux);
}

//PRE: tamanio debe ser mayor a 0
//POST: Recorre el arobol inorden y guarda los elementos en el array recibido.
void abb_recorrer_inorden(nodo_abb_t *nodo, size_t *nodos_guardados,
			  void **array, size_t tamanio)
{
	if (nodo == NULL || (*nodos_guardados) == tamanio) {
		return;
	}

	abb_recorrer_inorden(nodo->izquierda, nodos_guardados, array, tamanio);
	if ((*nodos_guardados) < tamanio) {
		array[*nodos_guardados] = nodo->elemento;
		(*nodos_guardados)++;
	}
	abb_recorrer_inorden(nodo->derecha, nodos_guardados, array, tamanio);
}

//PRE: tamanio debe ser mayor a 0
//POST: Recorre el arobol preorden y guarda los elementos en el array recibido.
void abb_recorrer_preorden(nodo_abb_t *nodo, size_t *nodos_guardados,
			   void **array, size_t tamanio)
{
	if (nodo == NULL || (*nodos_guardados) == tamanio) {
		return;
	}

	if ((*nodos_guardados) < tamanio) {
		array[*nodos_guardados] = nodo->elemento;
		(*nodos_guardados)++;
	}

	abb_recorrer_preorden(nodo->izquierda, nodos_guardados, array, tamanio);

	abb_recorrer_preorden(nodo->derecha, nodos_guardados, array, tamanio);
}

//PRE: tamanio debe ser mayor a 0
//POST: Recorre el arobol postorden y guarda los elementos en el array recibido.
void abb_recorrer_postorden(nodo_abb_t *nodo, size_t *nodos_guardados,
			    void **array, size_t tamanio)
{
	if (nodo == NULL || (*nodos_guardados) == tamanio) {
		return;
	}

	abb_recorrer_postorden(nodo->izquierda, nodos_guardados, array,
			       tamanio);

	abb_recorrer_postorden(nodo->derecha, nodos_guardados, array, tamanio);

	if ((*nodos_guardados) < tamanio) {
		array[*nodos_guardados] = nodo->elemento;
		(*nodos_guardados)++;
	}
}

//////////FIN FUNCIONES MIAS/////////

abb_t *abb_crear(abb_comparador comparador)
{
	if (comparador == NULL) {
		return NULL;
	}

	abb_t *arbol = calloc(1, sizeof(abb_t));

	if (arbol == NULL) {
		return NULL;
	}

	arbol->comparador = comparador;

	return arbol;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL) {
		return NULL;
	}

	if (arbol->tamanio == 0) {
		arbol->nodo_raiz = nodo_arbol_crear(elemento);
		arbol->tamanio++;
		return arbol;
	}

	if (arbol->comparador(elemento, arbol->nodo_raiz->elemento) <= 0) {
		return insertar_nodo_rec(arbol->nodo_raiz, arbol, elemento,
					 IZQUIERDA);
	}

	return insertar_nodo_rec(arbol->nodo_raiz, arbol, elemento, DERECHA);
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL) {
		return NULL;
	}

	void *elemento_eliminado = NULL;

	arbol->nodo_raiz = quitar_nodo_rec(arbol->nodo_raiz, arbol, elemento,
					   &elemento_eliminado);

	return elemento_eliminado;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL) {
		return NULL;
	}

	if (arbol->comparador(elemento, arbol->nodo_raiz->elemento) == 0) {
		return arbol->nodo_raiz->elemento;
	}

	if (arbol->tamanio > 0) {
		if (arbol->comparador(elemento, arbol->nodo_raiz->elemento) <
		    0) {
			return buscar_nodo_rec(arbol->nodo_raiz->izquierda,
					       arbol, elemento);
		}

		return buscar_nodo_rec(arbol->nodo_raiz->derecha, arbol,
				       elemento);
	}

	return NULL;
}

bool abb_vacio(abb_t *arbol)
{
	return (arbol == NULL || arbol->tamanio == 0 ||
		arbol->nodo_raiz == NULL);
}

size_t abb_tamanio(abb_t *arbol)
{
	if (arbol == NULL) {
		return 0;
	}

	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	if (arbol == NULL) {
		return;
	}
	if (arbol->nodo_raiz == NULL) {
		free(arbol);
		return;
	}
	abb_destruir_nodo(arbol, arbol->nodo_raiz);
	arbol->tamanio--;
	free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (arbol == NULL) {
		return;
	}
	if (arbol->nodo_raiz == NULL) {
		free(arbol);
		return;
	}

	abb_destruir_todo_nodo(arbol, destructor, arbol->nodo_raiz);
	free(arbol);
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL || funcion == NULL) {
		return 0;
	}

	size_t cant_funcion_invocada = 0;

	if (recorrido == 0) {
		abb_con_cada_elemento_inorden(arbol->nodo_raiz, funcion, aux,
					      &cant_funcion_invocada);
		return cant_funcion_invocada;
	}

	if (recorrido == 1) {
		abb_con_cada_elemento_preorden(arbol->nodo_raiz, funcion, aux,
					       &cant_funcion_invocada);
		return cant_funcion_invocada;
	}

	if (recorrido == 2) {
		abb_con_cada_elemento_postorden(arbol->nodo_raiz, funcion, aux,
						&cant_funcion_invocada);
		return cant_funcion_invocada;
	}

	return cant_funcion_invocada;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL || tamanio_array == 0) {
		return 0;
	}

	size_t nodos_guardados = 0;

	if (recorrido == 0) {
		abb_recorrer_inorden(arbol->nodo_raiz, &nodos_guardados, array,
				     tamanio_array);
		return nodos_guardados;
	}

	if (recorrido == 1) {
		abb_recorrer_preorden(arbol->nodo_raiz, &nodos_guardados, array,
				      tamanio_array);
		return nodos_guardados;
	}

	if (recorrido == 2) {
		abb_recorrer_postorden(arbol->nodo_raiz, &nodos_guardados,
				       array, tamanio_array);
		return nodos_guardados;
	}

	return 0;
}
