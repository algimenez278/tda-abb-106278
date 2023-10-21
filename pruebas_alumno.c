#include "pa2m.h"
#include "src/abb_estructura_privada.h"
#include "src/abb.h"

int comparar_nums(void *a, void *b)
{
	int *num1 = a;
	int *num2 = b;
	return (int)(*num1 - *num2);
}

int *crear_elemento(int num)
{
	int *elemento = calloc(1, sizeof(int));
	*elemento = num;
	return elemento;
}

void destruir_elemento(void *elemento)
{
	free(elemento);
}

void pruebas_crear(abb_t *abb)
{
	pa2m_afirmar(abb != NULL, "Creaste un arbol");
	pa2m_afirmar(abb->tamanio == 0, "Y su tamanio es 0");
	pa2m_afirmar(abb->nodo_raiz == NULL, "No tiene ningún elemento");
}

void pruebas_insertar_buscar(abb_t *abb, int *aux, int *e1, int *e2, int *e3,
			     int *e4, int *e7, int *e10, int *e11)
{
	printf("Luego de insertar varios elementos\n");
	pa2m_afirmar(abb->nodo_raiz->elemento == e1, "La raiz tiene el 1ro");
	pa2m_afirmar(abb->nodo_raiz->izquierda->elemento == e2,
		     "El 2do se encuentra en el lugar esperado");
	pa2m_afirmar(abb->nodo_raiz->derecha->elemento == e3,
		     "El 3ro se encuentra en el lugar esperado");
	pa2m_afirmar(abb->nodo_raiz->derecha->izquierda->elemento == e4,
		     "El 4to se encuentra en el lugar esperado");
	pa2m_afirmar(abb->nodo_raiz->izquierda->izquierda->elemento == e7,
		     "El 7mo se encuentra en el lugar esperado");
	pa2m_afirmar(
		abb->nodo_raiz->izquierda->izquierda->izquierda->elemento ==
			e10,
		"El 10mo se encuentra en el lugar esperado");
	pa2m_afirmar(abb->nodo_raiz->derecha->izquierda->derecha->izquierda
				     ->elemento == e11,
		     "El 11avo se encuentra en el lugar esperado");

	*aux = 50;
	pa2m_afirmar(abb_buscar(abb, aux) == e1, "Encontraste un elemento!");

	//esta era para chequear que estaba bien el comparador, y lo estaba...
	//el error era en abb_buscar()
	//asi que lo arregle, y corrieron bien todas las de busqueda :)
	pa2m_afirmar(abb->comparador(aux, e1) == 0, "Y son iguales!");

	*aux = 12;
	pa2m_afirmar(abb_buscar(abb, aux) == e2, "Encontraste un elemento!");

	*aux = 11;
	pa2m_afirmar(abb_buscar(abb, aux) == e7, "Encontraste un elemento!");

	*aux = 4;
	pa2m_afirmar(abb_buscar(abb, aux) == e10, "Encontraste un elemento!");

	*aux = 68;
	pa2m_afirmar(abb_buscar(abb, aux) == e3, "Encontraste un elemento!");
}

void pruebas_borrar_buscar(abb_t *abb, int *chau, int *e1, int *e2, int *e3,
			   int *e4, int *e5, int *e6, int *e8, int *e9,
			   int *e10, int *e11, int *e12)
{
	*chau = *e8;
	int *eliminado = abb_quitar(abb, chau);
	pa2m_afirmar(eliminado == e8, "Eliminaste un elemento bien!");

	*chau = *e9;
	eliminado = abb_quitar(abb, chau);
	pa2m_afirmar(eliminado == e9, "Eliminaste un elemento bien!");
	pa2m_afirmar(
		abb->nodo_raiz->derecha->izquierda->derecha->elemento == e11,
		"El hijo del eliminado se encuentra en el lugar esperado!");

	*chau = *e1;
	eliminado = abb_quitar(abb, chau);
	pa2m_afirmar(eliminado == e1, "Eliminaste la raiz bien!");
	printf("Eliminaste el %i\n", *eliminado);
	pa2m_afirmar(abb->nodo_raiz->elemento == e12,
		     "La raiz es reemplazada correctamente :)");

	*chau = *e12;
	eliminado = abb_quitar(abb, chau);
	pa2m_afirmar(eliminado == e12, "Eliminaste la raiz bien!");
	printf("Eliminaste el %i\n", *eliminado);
	pa2m_afirmar(abb->nodo_raiz->elemento == e5,
		     "La raiz es reemplazada correctamente :)");

	free(eliminado);
}

void pruebas_insertar_recorrer(abb_t *abb, int *aux, int *e2, int *e3, int *e4,
			       int *e5, int *e7, int *e10, int *e11, int *e12,
			       int *e13, int *e14, int *e15)
{
	printf("Luego de insertar 3 elementos más\n");
	*aux = 33;
	pa2m_afirmar(abb_buscar(abb, aux) == e13, "Encontraste el 1ro!");

	*aux = 8;
	pa2m_afirmar(abb_buscar(abb, aux) == e14, "Encontraste el 2do!");

	*aux = 2;
	pa2m_afirmar(abb_buscar(abb, aux) == e15, "Encontraste el 3ro!");

	int *vector_de_arbol[8];
	size_t guardados =
		abb_recorrer(abb, INORDEN, (void **)vector_de_arbol, 8);
	pa2m_afirmar(
		guardados == 8,
		"Se recorre INORDEN y se guardaron todos los elementos posibles :)");

	pa2m_afirmar(vector_de_arbol[0] == e15,
		     "El 1er elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[1] == e10,
		     "El 2do elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[2] == e14,
		     "El 3er elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[3] == e7,
		     "El 4to elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[4] == e2,
		     "El 5to elemento del array es el correcto");

	guardados = abb_recorrer(abb, PREORDEN, (void **)vector_de_arbol, 8);
	pa2m_afirmar(
		guardados == 8,
		"Se recorre PREORDEN y se guardaron todos los elementos posibles :)");

	pa2m_afirmar(vector_de_arbol[0] == e5,
		     "El 1er elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[1] == e2,
		     "El 2do elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[2] == e7,
		     "El 3er elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[3] == e10,
		     "El 4to elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[4] == e15,
		     "El 5to elemento del array es el correcto");

	guardados = abb_recorrer(abb, POSTORDEN, (void **)vector_de_arbol, 8);
	pa2m_afirmar(
		guardados == 8,
		"Se recorre POSTORDEN y se guardaron todos los elementos posibles :)");

	pa2m_afirmar(vector_de_arbol[0] == e15,
		     "El 1er elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[1] == e14,
		     "El 2do elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[2] == e10,
		     "El 3er elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[3] == e7,
		     "El 4to elemento del array es el correcto");
	pa2m_afirmar(vector_de_arbol[4] == e13,
		     "El 5to elemento del array es el correcto");
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== XXX ========================");
	abb_t *arbolito = abb_crear(comparar_nums);

	pa2m_nuevo_grupo(
		"\n==================== PRUEBAS_CREAR ====================");
	pruebas_crear(arbolito);

	int *e1 = crear_elemento(50);
	int *e2 = crear_elemento(12);
	int *e3 = crear_elemento(68);
	int *e4 = crear_elemento(59);
	int *e5 = crear_elemento(35);
	int *e6 = crear_elemento(22);
	int *e7 = crear_elemento(11);
	int *e8 = crear_elemento(70);
	int *e9 = crear_elemento(63);
	int *e10 = crear_elemento(4);
	int *e11 = crear_elemento(60);
	int *e12 = crear_elemento(41);

	int *aux = crear_elemento(0);
	int *chau = crear_elemento(0);

	abb_insertar(arbolito, e1);
	abb_insertar(arbolito, e2);
	abb_insertar(arbolito, e3);
	abb_insertar(arbolito, e4);
	abb_insertar(arbolito, e5);
	abb_insertar(arbolito, e6);
	abb_insertar(arbolito, e7);
	abb_insertar(arbolito, e8);
	abb_insertar(arbolito, e9);
	abb_insertar(arbolito, e10);
	abb_insertar(arbolito, e11);
	abb_insertar(arbolito, e12);

	pa2m_nuevo_grupo(
		"\n=============== PRUEBAS LUEGO DE INSERTAR Y BUSCAR ===============");

	pruebas_insertar_buscar(arbolito, aux, e1, e2, e3, e4, e7, e10, e11);

	pa2m_nuevo_grupo(
		"\n=============== PRUEBAS BORRAR Y BUSCAR ===============");
	pruebas_borrar_buscar(arbolito, chau, e1, e2, e3, e4, e5, e7, e8, e9,
			      e10, e11, e12);

	pa2m_nuevo_grupo(
		"\n=============== PRUEBAS VOLVER A INSERTAR Y RECORRER ===============");
	int *e13 = crear_elemento(33);
	int *e14 = crear_elemento(8);
	int *e15 = crear_elemento(2);

	abb_insertar(arbolito, e13);
	abb_insertar(arbolito, e14);
	abb_insertar(arbolito, e15);

	pruebas_insertar_recorrer(arbolito, aux, e2, e3, e4, e5, e7, e10, e11,
				  e12, e13, e14, e15);

	//esto para no perder memoria en nada :)
	free(e1);
	free(e8);
	free(e9);
	abb_destruir_todo(arbolito, destruir_elemento);
	destruir_elemento(aux);
	destruir_elemento(chau);

	return pa2m_mostrar_reporte();
}
