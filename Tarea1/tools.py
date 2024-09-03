import time
from collections import defaultdict
from functools import wraps


# Diccionario global para almacenar los tiempos de ejecución
tiempos_ejecucion = defaultdict(list)

def execution_time(func):
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
        print(f"La función {func.__name__} tardó {tiempo_ejecucion:.9f} segundos en ejecutarse.")
        return resultado
    return wrapper
