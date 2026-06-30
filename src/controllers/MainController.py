from ctypes import *
from pathlib import Path
from typing import List
import matplotlib.pyplot as plt
import pandas as pd
import sys
import os

# Models import
from models.Graph import Graph, GraphPtr
from models.Node import Node
from models.Solution import Solution

# Utils import
from utils.Utils import Utils


class MainController:
    TEMPERATURE = 1000000.0
    MIN_TEMPERATURE = 0.01
    COOLING_RATE = 0.995
    MAX_TIME = 240
    MAX_ON_SAME = 100000
    EARTH_DEGREE_TO_KM = 111.32

    @classmethod
    def run(
        cls,
        input: str,
        output: str,
        id_field: str,
        name_field: str,
        x_field: str,
        y_field: str,
    ) -> None:
        # Opening the DLL file with the algorithm
        try:
            Utils.log("Opening the C compiled file...")

            output_path: Path = Path(output)
            so_file_path: Path = Path(r"./core/target/annealing.so")
            annealing: CDLL = CDLL(so_file_path)
        except Exception as e:
            Utils.error(
                f"There was an error opening the compiled C file, did you already compile it? ({e})"
            )
            sys.exit(1)

        # Setting up the methods of the DLL
        try:
            Utils.log("Setting up all the methods inside of the file...")

            annealing.simulated_annealing.argtypes = [
                Graph,
                c_double,
                c_double,
                c_double,
                c_double,
                c_uint,
            ]
            annealing.simulated_annealing.restype = Solution

            annealing.create_node.argtypes = [c_char_p, c_double, c_double]
            annealing.create_node.restype = Node

            annealing.create_graph.argtypes = [c_int]
            annealing.create_graph.restype = Graph

            annealing.add_node.argtypes = [GraphPtr, Node, c_int]
            annealing.add_node.restype = None

            annealing.free_graph.argtypes = [GraphPtr]
            annealing.free_graph.restype = None

        except Exception as e:
            Utils.error(f"There was an error setting up the methods! ({e})")
            sys.exit(1)

        # Opening the dataset
        try:
            Utils.log("Opening the dataset...")

            input_path: Path = Path(input)
            dataframe: pd.DataFrame = pd.read_csv(input_path)

            plt.scatter(dataframe[x_field], dataframe[y_field])
            plt.xlabel("X Axis")
            plt.ylabel("Y Axis")
            plt.title("Input Cartesian Map")
            plt.savefig(os.path.join(output_path, "input_map.png"))
        except:
            Utils.error(
                f"There was an error opening the dataset, does the file really exist? ({e})"
            )
            sys.exit(1)

        # Converting the data
        try:
            Utils.log("Converting the data from the dataset...")

            df_index: pd.Index = dataframe.index
            graph: Graph = annealing.create_graph(len(df_index))

            for i in df_index:
                id = str(dataframe[id_field][i]).encode("utf-8")
                node = annealing.create_node(
                    id, dataframe[x_field][i], dataframe[y_field][i]
                )
                annealing.add_node(pointer(graph), node, i)

        except Exception as e:
            Utils.error(
                f"There was an error converting the data, check the convertion parameters! ({e})"
            )
            sys.exit(1)

        # Executing the algorithm
        try:
            Utils.log("Executing the algorithm...")
            solution: Solution = annealing.simulated_annealing(
                graph,
                cls.TEMPERATURE,
                cls.MIN_TEMPERATURE,
                cls.COOLING_RATE,
                cls.MAX_TIME,
                cls.MAX_ON_SAME,
            )
        except Exception as e:
            Utils.error(f"There was an error executing the algorithm! ({e})")
            sys.exit(1)

        # Generating the output
        try:
            Utils.log("Generating the solution output...")

            best_route: List[int] = solution.route[: graph.size]
            best_route = [
                int(graph.nodes[node_index].id.decode("UTF-8"))
                for node_index in best_route
            ]

            dataframe.set_index(id_field, inplace=True)

            formatted_route: List[str] = []
            formatted_data: List[str] = []

            for node in best_route:
                formatted_route.append(f"{dataframe.at[node, name_field]} ({node})")
                formatted_data.append(
                    {"x": dataframe.at[node, x_field], "y": dataframe.at[node, y_field]}
                )

            route_dataframe: pd.DataFrame = pd.DataFrame(formatted_data)

            plt.plot(route_dataframe["x"], route_dataframe["y"], "-o")
            plt.xlabel("X Axis")
            plt.ylabel("Y Axis")
            plt.title("Outoput Cartesian Map")
            plt.savefig(os.path.join(output_path, "output_map.png"))

            with open(os.path.join(output_path, "best_route.txt"), "w") as output_file:
                output_file.write("\n\t V \n".join(formatted_route))
                output_file.write("\n")
                output_file.write(
                    f"\ntotal-cost = {solution.cost * cls.EARTH_DEGREE_TO_KM} km\n"
                )  # We need to convert grades to km
        except Exception as e:
            Utils.error(f"There was an error generating the output! ({e})")
            sys.exit(1)
