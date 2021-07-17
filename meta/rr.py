import time

class Process:
    def __init__(self, pid, burst_time):
        self.pid = pid
        self.burst_time = burst_time


process_queue = []
