import time


def calmer(f=None, *, max_rate=1):
    def deco(f):
        last_call = 0

        def inner(cur_time):
            nonlocal last_call
            bound_rate = (1 / max_rate) * 1e3  # из секунд в милисекунды
            cur_time_ms = cur_time * 1e-9  #
            if cur_time_ms - last_call < bound_rate:
                time.sleep((bound_rate - (cur_time_ms - last_call)) * 1e-3)
            last_call = cur_time_ms
            return f(cur_time)

        return inner

    if f is None:
        return deco

    return deco(f)


@calmer(max_rate=2)
def timer(start_time):

    passed_msc = (time.time_ns() - start_time) / (10**6)
    print(f"current time: {start_time:.2f}")
    print(f"passed msc since last call: {passed_msc:.2f}")


if __name__ == "__main__":
    start_time = time.time_ns()
    for i in range(10):
        timer(start_time)
        start_time = time.time_ns()
