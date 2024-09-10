# EL4203-2024

Repositorio Tareas EL4203 - Pablo González Aguilera

## Preguntas teóricas - Tarea 1

### ¿Qué es un paradigma de programación?

Un paradigma de programación es una ideología o "estilo" de programación, en donde se siguen ciertas reglas o principios
a la hroa de diseñar
la logica de los programas.

Existen distintos tipos de paradigmas de programcion, como por ejemplo:

- Imperativo
- Orientado a Objetos (OOP)
- Funcional
- Declarativo

Cada uno tiene ventajas y desventajas, y en general existen lenguajes de programación que están diseñados para
acomodar distintos paradigmas (Por ejemplo, Java está enfocado en OOP mientras que Haskell está enfocado estrictamente
en
programación funcional.) Se dice que Python es Multiparadigma pues admite multiples estilos de programacion en un mismo
codigo.

### ¿En qué se basa la programación orientada a objetos?

La programación orientada a objetos está basada en Objetos y Clases.

Las clases se pueden entender como _plantillas_ para crear Objetos, pues definen la estructura
y el comportamiento de estos.

Los objetos son entidades abstractas que combinan estado (atributos) y comportamiento (métodos).

Las interfaces son conjuntos de métodos que permiten caracterizar a un objeto por lo que **puede hacer** vs lo que es.
(Nota: en Python no existen las interfaces como tal (existe algo similar llamado "Protocolo"), pero en otros lenguajes
son fundamentales para OOP (por ejemplo Java))

Sus principios fundamentales son:

Encapsulación: Los detalles internos de un objeto están ocultos del exterior y solo se exponen interfaces públicas.

Herencia: Las clases pueden derivarse de otras clases, permitiendo reutilizar código.

Polimorfismo: Un objeto puede tomar diferentes formas, lo que permite a una interfaz común manipular objetos de
distintas clases.

Abstracción: Los detalles innecesarios se ocultan, presentando solo las funcionalidades esenciales.

Además de lo anterior, se suelen seguir metodologias
como [SOLID](https://www.digitalocean.com/community/conceptual-articles/s-o-l-i-d-the-first-five-principles-of-object-oriented-design-es)
para poder implementar de buena las ideas de la programación OOP.

### ¿Cuál es la diferencia entre recursividad e iteración, y cómo se relaciona esto con la notación big 𝑂?

- **Recursividad**: Un enfoque de resolución de problemas donde una función se llama a sí misma para resolver un
  problema, descomponiéndolo en subproblemas más pequeños.
- **Iteración**: Un enfoque de resolución de problemas en el que se utiliza un bucle para repetir un bloque de código
  hasta que se cumpla una condición.

En términos de notación Big O, tanto la recursividad como la iteración pueden tener diferentes complejidades temporales.
La recursividad puede implicar un coste adicional si las llamadas no se optimizan bien (como en recursiones profundas).
Para evitar lo anterior, se utilizan tecnicas como la _memorización_, en donde se guardan en un _caché_ las ultimas
llamadas para evitar recursiones de gran escala. (En particular, en este programa se implementó lo anterior en la clase
PcbCalculator con el decorador `@cache`)

### Explicar la diferencia de rendimiento entre 𝑂(1) y 𝑂(𝑛)

- **O(1): Tiempo "constante":** El tiempo de ejecución no varía con respecto al tamaño del input. Un ejemplo es acceder
  a un elemento específico en un arreglo por índice. Sin importar cuántos elementos haya, siempre tomará el mismo
  tiempo.

- **O(n): Tiempo "Lineal"** El tiempo de ejecución crece **proporcionalmente** con el tamaño del input. Un ejemplo es
  recorrer un arreglo para buscar un elemento. A mayor cantidad de elementos, mayor será el tiempo necesario.

En esta tarea, el método de programación dinámica presenta complejidad temporal promedio O(n) y el metodo combinatorio
presenta complejidad temporal O(1)

### ¿Cómo se calcula el orden en un programa que funciona por etapas?

Dependera si las etapas son anidadas (una dentro de otra) o secuenciales (una despeus de otra).

Si las etapas se ejecutan de manera anidada, se multiplican las complejidades de cada una. Por
ejemplo, si una etapa O(n) se ejecuta dentro de otra etapa O(n), la complejidad resultante será O(n²).

Si las etapas se ejecutan secuencialmente, la complejidad del programa será del orden de la etapa de mayor orden dentro
de la secuencia, sin importar que existan etapas de orden menor (**depende del peor eslabon en la cadena**)

### ¿Cómo se puede determinar la complejidad temporal de un algoritmo recursivo?

Para esto se utiliza el llamado **Teorema Maestro**, que proporciona una **cota superior asintótica** para ecuaciones de
recurrencia. (ver [Teorema Maestro](https://es.wikipedia.org/wiki/Teorema_maestro))

## Grafico - Tarea 1

![Grafico Comparativo.png](Tarea1%2FGrafico%20Comparativo.png)