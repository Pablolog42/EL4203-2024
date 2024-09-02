import time
from functools import wraps


def execution_time(func):
    """
    Decorador que actua como wrapper e indica el tiempo de ejecución de uan funcion.
    Args:
        func: La función a ejecutar
    """

    @wraps(func)
    def wrapper(*args, **kwargs):
        start_time = time.time()
        result = func(*args, **kwargs)
        end_time = time.time()
        print(f"Tiempo de ejecución de {func.__name__}: {end_time - start_time} segundos")
        return result

    return wrapper
