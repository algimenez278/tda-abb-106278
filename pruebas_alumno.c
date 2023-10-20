#include "pa2m.h"
#include "src/abb_estructura_privada.h"
#include "src/abb.h"

int comparar_nums(void *a, void *b){
	int *num1 = a;
	int *num2 = b;
	return (int)(*num1 - *num2);
}

int *crear_elemento(int num){
	int *elemento= calloc(1, sizeof(int));
	*elemento=num;
	return elemento;
}

void destruir_elemento(void* elemento){
	free(elemento);
}



void pruebas_crear(abb_t * abb)
{
	pa2m_afirmar(abb != NULL, "creaste un arbol");
	pa2m_afirmar(abb->tamanio == 0, "y su tamanio es 0");
}

void pruebas_insertar_buscar(abb_t * abb, int* aux, int* e1, int* e2, int* e3){
	
	*aux= 50;
	pa2m_afirmar(abb_buscar(abb, aux) == e1, "encontraste un elemento!");

	//esta era para chequear que estaba bien el comparador, y lo estaba...
	//el error era en abb_buscar()
	//asi que lo arregle, y corrieron bien todas las de busqueda :)
	pa2m_afirmar(abb->comparador(aux, e1) == 0, "y son iguales!");

	*aux= 12;
	pa2m_afirmar(abb_buscar(abb, aux) == e2, "encontraste un elemento!");

	*aux= 68;
	pa2m_afirmar(abb_buscar(abb, aux) == e3, "encontraste un elemento!");
}


int main()
{
	pa2m_nuevo_grupo(
		"\n======================== XXX ========================");
	abb_t *arbolito= abb_crear(comparar_nums);

	pa2m_nuevo_grupo(
		"\n==================== PRUEBAS_CREAR ====================");
	pruebas_crear(arbolito);

	int *e1= crear_elemento(50);
	int *e2= crear_elemento(12);
	int *e3= crear_elemento(68);
	int *e4= crear_elemento(59);
	int *e5= crear_elemento(35);
	int *aux= crear_elemento(0);

	abb_insertar(arbolito, e1);
	abb_insertar(arbolito, e2);
	abb_insertar(arbolito, e3);
	abb_insertar(arbolito, e4);
	abb_insertar(arbolito, e5);


	pa2m_nuevo_grupo(
		"\n=============== PRUEBAS_INSERTAR/BUSCAR ===============");

	pruebas_insertar_buscar(arbolito, aux, e1, e2, e3);

	int *chau= crear_elemento(0);
	chau= abb_quitar(arbolito, aux);
	printf("%i", *chau);
	pa2m_afirmar(abb_quitar(arbolito, aux) == e5, "encontraste un elemento!");




//esto para no perder memoria en nada :)
	abb_destruir_todo(arbolito, destruir_elemento);
	destruir_elemento(aux);
	destruir_elemento(chau);




	return pa2m_mostrar_reporte();
}
