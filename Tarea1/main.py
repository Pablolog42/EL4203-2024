from Tarea1.model.Pcb import Pcb
from Tarea1.model.PcbPathCalculator import PcbPathCalculator

import time
from functools import wraps

largo = 16
ancho = 20

pcb = Pcb(largo=largo, ancho=ancho)
calculator = PcbPathCalculator()

a = calculator.calculate_paths(pcb, "dinamica")

b = calculator.calculate_paths(pcb, "combinatoria")

print(a, b)
