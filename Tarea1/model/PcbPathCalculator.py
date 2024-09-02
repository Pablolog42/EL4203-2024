import numpy as np
import scipy.special as sp

from Tarea1.model.Pcb import Pcb
from functools import cache

from Tarea1.tools import execution_time


class PcbPathCalculator:
    def __init__(self):
        pass

    @execution_time
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

        """

        match style:
            case "dinamica":
                print("Metodo de ejecución: Programacion dinamica")
                return self.__calculo_dinamico(pcb)

            case "combinatoria":
                print("Metodo de ejecución: Combinatoria")
                return self.__calculo_combinatoria(pcb)
            case _:
                raise ValueError("No se ha ingresado un estilo de calculo válido.")

    # Nota: se decora la funcion que utiliza recursividad para que utilice un cache simple (desde la libreria
    # integrada functools). Lo anterior lo hace
    # mucho mas eficiente a la hora de trabajar con recursividades profundas (para matrices grandes).
    @cache
    def __calculo_dinamico(self, pcb: Pcb) -> int:
        """
        Calcula en no de paths utilizando programación dinamica

        Args:
            pcb:
            style:

        Returns:

        """
        # Enfoque de programación dinámica usando NumPy
        matriz = pcb.matriz
        N = pcb.largo
        M = pcb.ancho

        # Caso base: solo hay una forma de llegar a las celdas de la primera fila y columna
        matriz[0, :] = 1
        matriz[:, 0] = 1

        # Llenar la tabla dp usando operaciones vectorizadas de NumPy
        for i in range(1, N):
            for j in range(1, M):
                matriz[i, j] = matriz[i - 1, j] + matriz[i, j - 1]

        return matriz[N - 1, M - 1]

    def __calculo_combinatoria(self, pcb: Pcb) -> int:
        """
        Calcula los paths posibles usando combinatoria

        Args:
            pcb:
            style:

        Returns:

        """
        # Enfoque combinatorio usando NumPy (ver calculo matematico en README)
        pasos_totales = pcb.largo + pcb.ancho - 2
        # El número de caminos es igual a la combinatoria de (largo+ancho-2) sobre de largo-1
        return int(sp.comb(pasos_totales, pcb.largo - 1, exact=True))
