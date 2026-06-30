from ctypes import *
from typing import List
import pandas as pd

"""
typedef struct Node {
    char id[20];
    double x, y;
} Node;
"""


class Node(Structure):
    _fields_ = [("id", c_char * 20), ("x", c_double), ("y", c_double)]


NodePtr = POINTER(Node)
