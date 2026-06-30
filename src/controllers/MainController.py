from ctypes import *
from pathlib import Path
import sys

# Models import
from models.Graph import Graph
from models.Node import Node
from models.Solution import Solution

# Utils import
from utils.Utils import Utils

class MainController:
    @classmethod
    def run(cls, input: str, output: str) -> None:
        # Opening the DLL file with the algorithm
        try:
            Utils.log("Opening the C compiled file...")
            so_file = Path(r"./core/target/annealing.so")
            annealing = CDLL(so_file)
        except:
            Utils.error("There was an error opening the compiled C file, did you already compile it?");
            sys.exit(1)