#!/usr/bin/env python3

import sys
import getopt
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

GROUP_BY_COLUMN = 'threads'
VALUES_COLUMN = 'execution_time'


def main(argv):
    options = read_options_from_cli(argv[1:])
    print(f'options: {options}')

    df = read_csv_and_agg(options['input'])

    plot(df,
         title=options['title'],
         mean_label=options['mean_label'],
         xlabel=options['x'],
         ylabel=options['y'],
         pdf=options['output_pdf'],
         png=options['output_png'],
         graphics=options['graphics'])


def read_csv_and_agg(filename):
    df = pd.read_csv(filename)

    df = df[[GROUP_BY_COLUMN, VALUES_COLUMN]]

    df = df.groupby(GROUP_BY_COLUMN).agg([np.mean, np.std])

    return df


def plot(df, title=None, mean_label=None, xlabel=None, ylabel=None, pdf=None, png=None, graphics=True):

    main_column = df.columns.values.tolist()[0][0]

    df[main_column].plot(y='mean',
                         yerr='std',
                         ecolor='black',
                         capsize=5,
                         label=mean_label)

    plt.title(title)

    plt.xlabel(xlabel)
    plt.ylabel(ylabel)

    plt.xticks(df[main_column].index.values.tolist())

    # plt.xlim(left=0)
    plt.ylim(bottom=0)

    plt.grid(True)

    if pdf != None:
        plt.savefig(pdf)

    if png != None:
        plt.savefig(png)

    if graphics:
        plt.show()

    plt.close()


def print_usage(*args, **kwargs):
    print(f"""Usage: {sys.argv[0]} [OPTION]...

DESCRIPTION
	Create a report based on the output of test_time.sh.

	-i [input]
		set the name of the input file.

	-m [label], --mean-label=[label]
		label shown in the legend
	
	--no-graphics
		disable the graphical interface

	-o [output]
		'-o output' if the same as setting '--out-pdf output.pdf' and '--out-png output.png'.

	--out-pdf=[output]
		set the name of the pdf output file. Override -o if provided.

	--out-png=[output]
		set the name of the png output file. Override -o if provided.

	-t [title], --title=[title]
		title of the graph

	-x [label], --xlabel=[label]
		label on x axis
	
	-y [label], --ylabel=[label]
		label on y axis
""", *args, **kwargs)


def read_options_from_cli(args):
    input = None
    output_pdf = None
    output_png = None
    graphics = True
    title=None
    mean_label=None
    x=None
    y=None

    try:
        opts, args = getopt.getopt(
            args, 'hi:m:o:t:x:y:', ['help', 'mean-label=', 'out-pdf=', 'out-png=', 'no-graphics', 'title=', 'xlabel=', 'ylabel='])
    except getopt.GetoptError:
        print_usage(file=sys.stderr)
        sys.exit(1)

    # parse options
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            print_usage()
            sys.exit()
        elif opt in ('-i'):
            input = arg
        elif opt in ('-o'):
            if output_pdf == None:
                output_pdf = f'{arg}.pdf'
            if output_png == None:
                output_png = f'{arg}.png'
        elif opt in ('-m', '--mean-label'):
            mean_label = arg
        elif opt in ('--no-graphics'):
            graphics = False
        elif opt in ('--out-pdf'):
            output_pdf = arg
        elif opt in ('--out-png'):
            output_png = arg
        elif opt in ('-t', '--title'):
            title = arg
        elif opt in ('-x', '--xlabel'):
            x = arg
        elif opt in ('-y', '--ylabel'):
            y = arg

    if input == None:
        print("No input file provided. Use the -i option", file=sys.stderr)
        sys.exit(1)

    return {
        'input': input,
        'output_pdf': output_pdf,
        'output_png': output_png,
        'graphics': graphics,
        'title': title,
        'mean_label': mean_label,
        'x': x,
        'y': y
    }


if __name__ == '__main__':
    main(sys.argv)
