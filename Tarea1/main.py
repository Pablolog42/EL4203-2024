from Tarea1.model.Pcb import Pcb
from Tarea1.model.PcbPathCalculator import PcbPathCalculator

import matplotlib

# Este es un work-arround porque el output del grafico
# me salia en blanco al usar otro motor
matplotlib.use('Agg')

import matplotlib.pyplot as plt

MAX_TESTING_LENGTH = 30
TESTING_WIDTH = 5

# Se prueba con largos entre 1 y MAX_TESTING_LENGTH
largos = [i for i in range(1, MAX_TESTING_LENGTH)]

# Para los benchmarks, se mantendrá un ancho fijo de 5 unidades
# Lo anterior puede see cambiado
anchos = [TESTING_WIDTH for _ in range(1, MAX_TESTING_LENGTH)]

# Creo una lista de PCB con distintos largos y anchos para probar
pcb_test_list = [Pcb(largo=largo, ancho=ancho) for largo, ancho in zip(largos, anchos)]

calculator = PcbPathCalculator()


# Agrego el decorador interno para guardar los tiempos en el objeto calculator
@calculator.execution_time
def dynamic_test(test_pcb: Pcb): return calculator.calculate_paths(test_pcb, "dinamica")


@calculator.execution_time
def combinatoric_test(test_pcb: Pcb): return calculator.calculate_paths(test_pcb, "combinatoria")


for pcb in pcb_test_list:
    dynamic_test(pcb)
    combinatoric_test(pcb)

print(calculator.dict_tiempos_ejec)

# Ahora, nos interesa graficar el largo (nuestra variable) vs el tiempo de ejecucion
# Para cada uno de los tipos de calculo (combinatorio y utilizando progr. dinamica)

# largos contiene la lista de largos
# Los tiempos están guardados dentrodel diccionario del objeto calculadora
lista_tiempos_dinamico = calculator.dict_tiempos_ejec["dynamic_test"]
lista_tiempos_combinatoria = calculator.dict_tiempos_ejec["combinatoric_test"]


# Grneracion de grafico comparativo
def generar_grafico(guardar_como_png: bool = False) -> None:
    """
    Genera grafico comparativo y lo retorna como imagen PNG en el mismo directorio activo
    """
    # Generación Gráficos
    plt.figure(figsize=(10, 6))
    plt.scatter(largos, lista_tiempos_dinamico, label="Tiempos Approach Programación Dinámica")
    plt.scatter(largos, lista_tiempos_combinatoria, label="Tiempos Approach Combinatoria")
    plt.xlabel(f'Largo de PCB (Con Ancho M={TESTING_WIDTH} fijo)')
    plt.ylabel('Tiempo de ejecución (segundos)')
    plt.title('Comparación de tiempos de ejecución')
    plt.legend()
    plt.grid(True)

    # Guardar la figura como archivo PNG
    plt.savefig("Grafico Comparativo", bbox_inches='tight')


# Guarda grafico como png en el directorio activo actual
generar_grafico(guardar_como_png=True)
