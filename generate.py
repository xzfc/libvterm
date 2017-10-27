#!/usr/bin/env python3

import random

def produce(x):
    if isinstance(x, int):
        return x
    if isinstance(x, tuple):
        return random.randint(x[0], x[1])
    if isinstance(x, list):
        return produce(random.choice(x))
    if callable(x):
        return x()
    if hasattr(x, 'foo'):
        return x.foo()
    raise Exception(':<')

def rand_u8():
    return random.randint(0, 255)

def alt(start):
    def fn():
        if random.randint(0,1):
            return "{}:5:{}".format(start, rand_u8())
        else:
            return "{}:2:{}:{}:{}".format(start, rand_u8(), rand_u8(), rand_u8())
    return fn

codes = [0, 1, 7, 22, 27, (30,37), alt(38), 39, (40,47), alt(48), 49, (90,97), (100, 107)]

for i in range(100000):
    print(format(produce(codes)))
