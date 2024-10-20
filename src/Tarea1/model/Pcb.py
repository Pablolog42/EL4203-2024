from dataclasses import dataclass
import numpy as np


class Pcb:
    """
    Clase que representa una PCB, incluyendo su matriz de dimensiones NxM.

    Parameters:
        largo: int
        ancho: int
        matriz: tuple
    """
    def __init__(self, largo: int = 0, ancho: int = 0):
        self.largo: int = largo
        self.ancho: int = ancho
        self.matriz = np.zeros((largo, ancho), dtype=int)
