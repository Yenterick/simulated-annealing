from controllers.MainController import MainController
import optparse

if __name__ == "__main__":
    parser = optparse.OptionParser()
    parser.add_option(
        "--input",
        "-i",
        help="input dataset to process with simulated annealing",
        default="../examples/datos-geograficos-de-los-municipios-de-valle-del-cauca-20260630.csv",
    )
    parser.add_option(
        "--output", "-o", help="output path to generate the results", default="./target"
    )
    parser.add_option(
        "--id_field",
        "-d",
        help="the name of the field with the node id in the dataset",
        default="codigo-de-municipio",
    )
    parser.add_option(
        "--name_field",
        "-n",
        help="the name of the field with the node name in the dataset",
        default="municipio",
    )
    parser.add_option(
        "--x_field",
        "-x",
        help="the name of the field with the x coord in the dataset",
        default="longitud",
    )
    parser.add_option(
        "--y_field",
        "-y",
        help="the name of the field with the y coord in the dataset",
        default="latitud",
    )

    options, _ = parser.parse_args()

    MainController.run(
        options.input,
        options.output,
        options.id_field,
        options.name_field,
        options.x_field,
        options.y_field,
    )
