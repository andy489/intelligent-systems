import time
from functools import wraps


def timed(label=None):
    def decorator(fn):
        @wraps(fn)
        def wrapper(*args, **kwargs):
            start = time.perf_counter()
            result = fn(*args, **kwargs)
            elapsed_ms = (time.perf_counter() - start) * 1000
            print(f"[{label or fn.__name__}] took: {elapsed_ms:.2f} ms ≈ {elapsed_ms / 1000:.2f} s")
            return result
        return wrapper
    return decorator
