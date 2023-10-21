# TDA ABB

## Repositorio de Abril Luna Gimenez - 106278 - algimenez@fi.uba.ar

- Para compilar:

```bash
línea de compilación
```

- Para ejecutar:

```bash
línea de ejecución
```

- Para ejecutar con valgrind:
```bash
línea con valgrind
```
---
##  Funcionamiento

El programa desarrolla las funciones necesarias para trabajar con un arbol binario. Crearlo, agregar elementos, buscarlos, quitarlos, etc.
Permite al usuario conocer el tamanio del arbol, su raiz, si el mismo esta vacio, guardar elementos del mismo en un array y recorrerlo.
En el archivo `abb.c` la función `abb_crear` utiliza `calloc` para reservar la memoria necesaria para un arbol e inicializar arbol->nodo_raiz y arbol->tamanio.
El programa luego de usar `calloc` verifica que se haya reservado la memoria correctamente, en caso contrario maneja el error, retornando NULL por ej.

La funcion `insertar_nodo_rec` tabaja recursivamente hasta encontrar el lugar correspondiente para el nodo a insertar, según el elemento enviado.

<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:

```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas
Incluír acá las respuestas a las preguntas del enunciado (si aplica).
