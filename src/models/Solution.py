from ctypes import *

"""
typedef struct Solution {
    int* route;
    double cost;
} Solution;
"""


class Solution(Structure):
    _fields_ = [("route", POINTER(c_int)), ("cost", c_double)]


SolutionPtr = POINTER(Solution)
