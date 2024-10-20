import time
from collections import defaultdict

import scipy.special as sp

from src.Tarea1.model.Pcb import Pcb
from functools import cache, wraps


class PcbPathCalculator:
    def __init__(self):
        # Diccionario de clase para almacenar los tiempos de ejecución
        self.dict_tiempos_ejec = defaultdict(list)

    def execution_time(self, func):
        """
        Decorador standard que actua como wrapper e indica el tiempo de ejecución de uan funcion.
        Args:
            func: La función a ejecutar
        """

        @wraps(func)
        def wrapper(*args, **kwargs):
            inicio = time.perf_counter()
            resultado = func(*args, **kwargs)
            fin = time.perf_counter()
            tiempo_ejecucion = fin - inicio
            # Guardar el tiempo de ejecución en el diccionario
            self.dict_tiempos_ejec[func.__name__].append(tiempo_ejecucion)
            print(f"La función {func.__name__} tardó {tiempo_ejecucion:.9f} segundos en ejecutarse.")
            return resultado

        return wrapper

    def calculate_paths(self, pcb: Pcb, style: str) -> int:
        """
        Función que retorna el número de paths calculados, de distintas formas
        Args:
            pcb: Objeto PCB sobre el cual se realizaran los calculos.
            style: Definira si se utiliza programación dinamica o la formula matemática combinatoria
                "dinamica" -> Approach programación dinamica
                "combinatoria" -> Approach combinatorio (matematica)
        Returns:
            El número de posibles paths calculados siguiendo las reglas especificadas, como int.

        Raises:
            ValueError: No se ha ingresado un estilo de calculo válido.
            ValueError: Si existe overflow en el calculo

        """

        match style:
            case "dinamica":
                # print("Metodo de ejecución: Programacion dinamica")
                try:
                    return self.__calculo_dinamico(pcb)
                except RuntimeError:
                    raise ValueError("El tamaño seleccionado es demasiado grande para ser calculado por programación "
                                     "dynamic (Overflow)")
            case "combinatoria":
                # print("Metodo de ejecución: Combinatoria")
                return self.__calculo_combinatoria(pcb)
            case _:
                raise ValueError("No se ha ingresado un estilo de calculo válido.")

    # Nota: se decora la funcion que utiliza recursividad para que utilice un cache simple (desde la libreria
    # integrada functools). Lo anterior lo hace
    # mucho mas eficiente a la hora de trabajar con recursividades profundas (para matrices grandes).
    @cache
    def __calculo_dinamico(self, pcb: Pcb) -> int:
        """
        Calcula en no de paths utilizando programación dinamica (llena una matriz prograsivamente utilizando recursividad)

        """
        # Enfoque de programación dinámica
        matriz = pcb.matriz
        N = pcb.largo
        M = pcb.ancho

        # Caso base: solo hay una forma de llegar a las celdas de la primera fila y columna
        matriz[0, :] = 1
        matriz[:, 0] = 1

        # Llenar la tabla matriz a partir de los items vistos anteriormente (adyacentes)
        for i in range(1, N):
            for j in range(1, M):
                matriz[i, j] = matriz[i - 1, j] + matriz[i, j - 1]

        return matriz[N - 1, M - 1]

    def __calculo_combinatoria(self, pcb: Pcb) -> int:
        """
        Calcula los paths posibles usando combinatoria
        """
        # Enfoque combinatorio u
        pasos_totales = pcb.largo + pcb.ancho - 2
        # El número de caminos es igual a la combinatoria de (largo+ancho-2) sobre de largo-1
        # lo anterior se debe a que solamente se puede ir para la derecha una vez iniciado el camino (no hay backtracking)
        return int(sp.comb(pasos_totales, pcb.largo - 1, exact=True))
