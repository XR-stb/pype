class Time:
    delta_time = 0.0
    time = 0.0

    def _tick(delta: float):
        Time.delta_time = delta
        Time.time += delta