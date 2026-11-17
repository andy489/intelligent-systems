import time
from functools import wraps


def timed(label=None):
    def decorator(fn):
        @wraps(fn)
        def wrapper(*args, **kwargs):
            start = time.perf_counter()
            result = fn(*args, **kwargs)
            elapsed_ms = int((time.perf_counter() - start) * 1000)
            h   = elapsed_ms // 3_600_000
            rem = elapsed_ms %  3_600_000
            m   = rem // 60_000
            rem = rem %  60_000
            s   = rem // 1000
            ms  = rem %  1000
            print(f"[{label or fn.__name__}] {h:02d}:{m:02d}:{s:02d}.{ms:03d}")
            return result
        return wrapper
    return decorator
