"""
Usage:
> python3 analysis.py [--latex] [--plot [main, table, other]] [--tables]

The following script analyzes benchmarks for KruskalSimple, KruskalUnionFind (and variants) and
PrimBinaryHeap algorithms.

The aim of the script is to produce in output som graphical representation of
the benchmarks, in particular
- for each algorithm show how it scales with the size of the input
- for some (maybe fixed) input sizes show the compared performance of the
  implemented algorithms.

Options:
- [latex]: export comparison tables in latex format
- [plot]: plot comparisons
"""

from functools import reduce
import sys
from io import StringIO

import pandas as pd
import numpy as np
import glob
from itertools import chain
from tabulate import tabulate
from typing import List, Dict
import matplotlib.pyplot as plt
from types import SimpleNamespace
from tably import Tably
import seaborn as sns

# number of floating point decimals displayed in output
N_DECIMALS = 3

# number of floating point decimals displayed in output for values that represent percentages
N_DECIMALS_PERCENTAGE = 2

# if the script is given the argument '--latex', turn on LaTeX table generation
FOR_LATEX = '--latex' in sys.argv
# if the script is given the argument '--tables', turn on tables comparisons
IS_TABLE_ENABLED = '--tables' in sys.argv
# if the script is given the argument '--plot', turn on comparison plots (with options)
IS_PLOT_ENABLED = '--plot' in sys.argv
IS_PLOT_MAIN_ENABLED = 'main' in sys.argv
IS_PLOT_TABLE_ENABLED = 'table' in sys.argv
IS_PLOT_OTHER_ENABLED = 'other' in sys.argv

KRUSKAL_SIMPLE = 'KruskalSimple'
KRUSKAL_UNION_FIND = 'KruskalUnionFind'
KRUSKAL_UNION_FIND_COMPRESSED = 'KruskalUnionFindCompressed'
PRIM_BINARY_HEAP = 'PrimBinaryHeap'
PRIM_K_HEAP = 'PrimKHeap'

programs = [
    KRUSKAL_SIMPLE,
    KRUSKAL_UNION_FIND,
    KRUSKAL_UNION_FIND_COMPRESSED,
    PRIM_BINARY_HEAP,
    PRIM_K_HEAP
]

ms_programs = [
    'ms_kruskal_simple',
    'ms_kruskal_union_find',
    'ms_kruskal_union_find_compressed',
    'ms_prim_binary_heap',
    'ms_prim_k_heap'
]


def read_csvs_of_program(program: str) -> List[pd.DataFrame]:
    """
    Given the name of an algorithm program, collects the list of CVS benchmarks recorded
    for that particular program.
    :param program: name of the program which benchmarks should be retrieved
    :return: list of benchmark CSV files
    """

    csv_files = glob.glob(f'./{program}*.csv')
    dataframes_per_program = []

    for csv_file in csv_files:
        dataframe = pd.read_csv(csv_file, sep=';', decimal='.', encoding='utf-8')
        dataframes_per_program.append(dataframe)

    return dataframes_per_program


def check_same_n_rows(dfs_flat: List[pd.DataFrame]):
    """
    Utility function
    :param dfs_flat:
    """

    dfs_size = map(lambda x: x.shape[0], dfs_flat)
    is_valid = len(set(dfs_size)) == 1
    if not is_valid:
        raise AssertionError('The CSV files have different number of rows')


def check_same_mst_outputs(dfs_flat: List[pd.DataFrame]):
    """

    :param dfs_flat:
    :return:
    """

    df_base = dfs_flat[0]

    for i in range(len(dfs_flat) - 1):
        df_next = dfs_flat[i + 1]

        result: pd.Series = df_base['output'].isin(df_next['output']).value_counts()
        try:
            result[True]
        except KeyError:
            raise AssertionError('The CSV files have different mst outputs')


def check_validity(dfs_list: List[List[pd.DataFrame]]):
    """
    Utility functions that checks whether the imported benchmark CSVs are valid
    :param dfs_list: list of list of CSVs represented as DataFrames
    """
    dfs_flat: List[pd.DataFrame] = list(chain.from_iterable(dfs_list))
    check_same_n_rows(dfs_flat)
    check_same_mst_outputs(dfs_flat)


def get_row_index_with_max_edges(n_nodes_as_str: str) -> int:
    """
    Return the row index of any benchmark CSV file having the maximum amount of edges fixing a certain number of
    nodes. The number of nodes considered "inseresting" are:
    - 8k
    - 10k
    - 20k
    - 40k
    - 80k
    - 100k

    :param n_nodes_as_str: number of nodes represented as a string in k-thousands-notation
    :return: index with maximum edges
    """

    # removes the final 'k' and multiply by 1000
    n_nodes = int(n_nodes_as_str[:-1]) * 1000

    # these values correspond to the (line - 2) of the CSV line with the maximum amount of edges, given a fixed
    # number k of nodes
    obj = {
        8000: 47,    # 10758 edges
        10000: 49,   # 13342 edges
        20000: 53,   # 26826 edges
        40000: 57,   # 53447 edges
        80000: 60,   # 106914 edges
        100000: 66,  # 133525 edges
    }

    return obj[n_nodes]


def get_min_at_row(dfs: List[pd.DataFrame], row: int, column: str = 'ms') -> np.double:
    """
    Return the minimum value of a certain column among the given list of benchmark dataframes in a certain row
    :param dfs: list of benchmark dataframes
    :param row: index of the row to be selected
    :param column: column of the dataframe to be selected. Default: 'ms'
    :return: minimum value of column at row
    """

    values_at_row_column = [df[column].loc[row] for df in dfs]
    min_index, min_value = min(enumerate(values_at_row_column), key=lambda x: x[1])
    return min_index, min_value


def create_list_of_ms_at_row(dfs_list: List[pd.DataFrame], columns: List[str]) -> List[List[np.double]]:
    """
    :param dfs_list: list of benchmark dataframes
    :param columns: list of columns to be displayed in a table, aka list of "interesting" number of nodes
    :return: 2-d list of minimum execution times. For each column in columns, the row which stores the maximum
    number of edges wrt to the fixed number of nodes (column) is selected.
    """

    return [
        [
            df['ms'].loc[get_row_index_with_max_edges(n_nodes_as_str)]
            for n_nodes_as_str in columns
        ]
        for df in dfs_list
    ]


def compare_2_programs(dfs_dict: Dict[str, pd.DataFrame], program_1: str, program_2: str) -> pd.DataFrame:
    """
    Compare 2 programs, putting in evidence their difference in runtime execution and how much the 2nd program
    improves the execution time compared to the 1st program.
    :param dfs_dict: map of DataFrames minimized wrt to column 'ms'
    :param program_1: name of the 1st program to compare
    :param program_2: name of the 2nd program to compare
    :return: new DataFrame containing the comparison information between the given two programs
    """

    # subset of dfs_dict containing only the list of dataframes related to program_1 and program_2
    dfs_filter: Dict[str, pd.DataFrame] = dict((k, v) for k, v in dfs_dict.items() if k in [program_1, program_2])
    dfs_valid: List[pd.DataFrame] = [dfs_filter[program] for program in [program_1, program_2]]

    # comparison DataFrame columns
    columns = ['8k', '10k', '20k', '40k', '80k', '100k']

    # compute rows of program_1 and program_2
    program_rows_np = np.asarray(create_list_of_ms_at_row(dfs_valid, columns), dtype=np.double)

    # compute difference row
    diff_np = program_rows_np[0] - program_rows_np[1]

    # compute improvement row (expressed as a percentage number)
    improvement_percentage = np.around(100 * diff_np / program_rows_np[0], decimals=N_DECIMALS_PERCENTAGE).tolist()

    # remove numpy wrapper
    program_1_row, program_2_row = program_rows_np.tolist()
    diff = diff_np.tolist()

    data = [
        [program_1, *program_1_row],
        [program_2, *program_2_row],
        ['Difference', *diff],
        ['Improvement %', *improvement_percentage]
    ]

    return pd.DataFrame(data, columns=['', *columns])


def print_comparison(dfs_dict: Dict[str, pd.DataFrame], program_1: str, program_2: str):
    print(f'Comparison between {program_1} and {program_2}.')

    df_comparison = compare_2_programs(dfs_dict, program_1, program_2)

    # pretty-print comparison DataFrame
    pretty_print_pandas(df_comparison)


def pretty_print_pandas(df: pd.DataFrame, tablefmt='pretty'):
    """
    Pretty-printer utility for Pandas DataFrames.
    :param df: DataFrame to pretty-print
    :param tablefmt: 'pretty' | 'psql' | 'latex'
    """
    if FOR_LATEX:
        tablefmt = 'latex'  # print out \tabular{} LaTeX tables
    print(tabulate(df.round(N_DECIMALS), headers='keys', tablefmt=tablefmt, showindex=False))
    print('\n')


def minimize_ms_dataframes_helper(dfs: List[pd.DataFrame]) -> pd.DataFrame:
    """
    Create a new in-memory DataFrame that, for each row of all the dataframes in dfs, keeps only the row with the
    minimum 'ms' value.
    :param dfs: List of DataFrames for a single program
    :return: new DataFrame minimized row-wise wrt the value of the 'ms' column
    """
    columns = ['ms', 'output', 'n', 'm', 'filename']
    min_df = pd.DataFrame(columns=columns)

    n_rows = dfs[0].shape[0]
    for row in range(n_rows):
        min_index, _ = get_min_at_row(dfs, row, column='ms')
        data = dfs[min_index].loc[row]
        min_df = min_df.append(data)

    return min_df.round(decimals=N_DECIMALS)


def minimize_ms_dataframes(dfs_dict: Dict[str, List[pd.DataFrame]]) -> Dict[str, pd.DataFrame]:
    """
    For each list of DataFrames stored in dfs_dict keys, retain only the rows which value at column 'ms' is minimum.
    :param dfs_dict: original map of list of benchmark DataFrames for each program
    :return: minimized map of 1 dataframe for each program wrt to column 'ms'
    """
    dfs_flat_min = [minimize_ms_dataframes_helper(dfs) for dfs in dfs_dict.values()]
    return dict(zip(programs, dfs_flat_min))


def keep_only_interesting_nodes(df: pd.DataFrame, columns: List[str]) -> pd.DataFrame:
    return pd.DataFrame([
        df[columns].loc[get_row_index_with_max_edges('8k')],
        df[columns].loc[get_row_index_with_max_edges('10k')],
        df[columns].loc[get_row_index_with_max_edges('20k')],
        df[columns].loc[get_row_index_with_max_edges('40k')],
        df[columns].loc[get_row_index_with_max_edges('80k')],
        df[columns].loc[get_row_index_with_max_edges('100k')],
    ], columns=columns)


def plot_simple_bar(df: pd.DataFrame, x: str, y: str, title: str):
    ax = plt.gca()
    df.plot(x, y, kind='bar', logy=True, ax=ax)
    plt.title(title)
    plt.legend()
    plt.xlabel(x)
    plt.ylabel(y)
    plt.show()


def plot_interesting_nodes_bar(df: pd.DataFrame, title: str):
    columns = ['n', 'ms']
    new_df = keep_only_interesting_nodes(df, columns)
    plot_simple_bar(new_df, *columns, title=title)


def plot_multiple_nodes_line(dfs: Dict[str, pd.DataFrame], title: str, max_nodes=1000):
    ms_dict = dict(zip(programs, ms_programs))
    dfs_renamed = [
        dfs[program].drop(columns=['output', 'filename', 'm']).rename(columns={'ms': ms_dict[program]}, inplace=False)
        for program in programs
    ]

    merged_df = reduce(lambda x, y: pd.merge(x, y, on=['n'], how='outer'), dfs_renamed)

    merged_df = merged_df[(merged_df['n'] <= max_nodes)]

    ax = plt.gca()

    for ms_program in ms_programs:
        merged_df.plot(kind='line', x='n', y=ms_program, ax=ax)

    plt.title(title)
    plt.legend()
    plt.xlabel('n')
    plt.ylabel('ms')
    plt.show()


def dataframe_to_stringio(df: pd.DataFrame) -> StringIO:
    csv_buffer = StringIO()

    # removes the number of vertexes/edges
    df.drop(columns=['n', 'm']) \
        .rename(columns={'output': 'MST', 'filename': 'File'}) \
        .to_csv(csv_buffer, index=None, sep=';')

    csv_buffer.seek(0)
    return csv_buffer


def export_dataframes_min_to_latex(dfs: Dict[str, pd.DataFrame]):
    dfs_list = dfs.values()
    csv_list = list(map(dataframe_to_stringio, dfs_list))

    for i, program in enumerate(dfs.keys()):
        tably_instance = Tably(SimpleNamespace(
            files=[csv_list[i]],
            align=['l', 'l', 'c'],
            caption=f'Risultati di {program}',
            no_indent=False,
            skip=0,
            label=f'table:{program}-results',
            no_header=False,
            outfile=f'../report/{program}.min.tex',
            separate_outfiles=None,
            preamble=False,
            sep='semi',
            units='-',
            no_escape=False,
            fragment=False,
            fragment_skip_header=False,
            replace=True,
        ))
        tably_instance.run()


def plot_line(dfs: Dict[str, pd.DataFrame], x: str, y: str, title: str, options_f=[]):
    """
    Plots `dfs` data.
    :dfs: a dictionary with the benchmark dataframe to plot.
    :x: the x axis name.
    :y: the y axis name.
    :title: a string representing the plot title.
    :options_f: a list of functions to apply to the result of sns.lineplot (matplotlib.axis).
    """
    plt.figure(figsize=(14, 7))
    for k, v in dfs.items():
        g = sns.lineplot(v[x], v[y], label=k)
        for fo in options_f:
            fo(g)
    plt.title(title)
    plt.show()


def names_to_vs(names: List[str]) -> str:
    """
    Return a string made by names joined by "vs".
    """
    return reduce(lambda x, y: x + ' vs ' + y, names)


def names_to_dfs(names: List[str], dfs) -> Dict[str, List[pd.DataFrame]]:
    """
    Return a dict where for each name the key is one of the name given and the
    value is the corresponding benchmark dataframe.
    """
    return reduce(lambda x, y: {**x, **y}, map(lambda n: {n: dfs[n]}, names))


def names_to_plot(names: List[str], dfs: pd.DataFrame):
    """
    Plot given programs.
    """
    benchmark_subset = names_to_dfs(names, dfs)
    title = names_to_vs(names)
    plot_line(benchmark_subset, x='n', y='ms', title=title)


def names_to_plot_logy(names: List[str], dfs: pd.DataFrame):
    """
    Plot given programs with log scaled y axis.
    """
    benchmark_subset = names_to_dfs(names, dfs)
    title = names_to_vs(names) + ' (log y scaled)'
    def log_scale(g): return g.set_yscale('log')
    plot_line(benchmark_subset, x='n', y='ms',
              title=title, options_f=[log_scale])


def split_df(df: pd.DataFrame, pred):
    """
    Return a pair whose first element is the orginal dataset where the predicate
    `pred` holds, while the second is the orginal dataset where the predicate
    `pred` does not hold.
    :param df: A dataframe.
    :param pred: A predicate.
    """
    df1 = pd.DataFrame([df.loc[i] for i in range(len(df)) if pred(df.loc[i])], columns=df.columns)
    df2 = pd.DataFrame([df.loc[i] for i in range(len(df)) if not pred(df.loc[i])], columns=df.columns)
    # df2 = [x for x in df[x] if x['n'] >= threshold]
    return (df1, df2)


def split_and_plot(names: List[str], dfs: Dict[str, pd.DataFrame], pred, to_print='both'):
    """
    Split dataframes from given names w.r.t. split predicate and plot them.
    :param names: A list of algorithms names.
    :param dfs: A dictionary of dataframes.
    :param pred: A predicate over df benchmark row.
    :param split_print: What to plot, can be either 'left', 'right' or 'both'.
    """
    d1, d2 = {}, {}
    for i in range(len(names)):
        name = names[i]
        df1, df2 = split_df(dfs[name], pred=pred)
        d1 = {**d1, **{name: df1}}
        d2 = {**d2, **{name: df2}}

    if to_print == 'left' or to_print == 'both':
        names_to_plot(names, d1)
    if to_print == 'right' or to_print == 'both':
        names_to_plot(names, d2)


if __name__ == '__main__':
    # benchmark CSVs stored in DataFrames and grouped by program name
    generator = zip(programs, [read_csvs_of_program(program) for program in programs])
    dataframes: Dict[str, List[pd.DataFrame]] = dict(generator)

    # is some CSV is invalid, throws an AssertionError
    check_validity(dataframes.values())

    # for each CSV, only the minimum values for the 'ms' column are retained
    # len(dataframes_min) == len(programs)
    dataframes_min = minimize_ms_dataframes(dataframes)

    if IS_TABLE_ENABLED:
        # compare multiple programs to show potential improvements
        print_comparison(dataframes_min, KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND)
        print_comparison(dataframes_min, KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND_COMPRESSED)
        print_comparison(dataframes_min, KRUSKAL_SIMPLE, PRIM_BINARY_HEAP)
        print_comparison(dataframes_min, KRUSKAL_UNION_FIND, KRUSKAL_UNION_FIND_COMPRESSED)
        print_comparison(dataframes_min, PRIM_BINARY_HEAP, KRUSKAL_UNION_FIND)
        print_comparison(dataframes_min, PRIM_BINARY_HEAP, KRUSKAL_UNION_FIND_COMPRESSED)
        print_comparison(dataframes_min, PRIM_BINARY_HEAP, PRIM_K_HEAP)
        print_comparison(dataframes_min, PRIM_K_HEAP, KRUSKAL_UNION_FIND)
        print_comparison(dataframes_min, PRIM_K_HEAP, KRUSKAL_UNION_FIND_COMPRESSED)

    # export minimized in-memory CSV files to LaTeX tables (they will still require some manual work tho)
    # export_dataframes_min_to_latex(dataframes_min)

    if IS_PLOT_ENABLED:

        if IS_PLOT_OTHER_ENABLED:
            split_and_plot([KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND, PRIM_BINARY_HEAP], dataframes_min, pred=lambda x: x['n'] <= 2000, to_print='left')
            split_and_plot([KRUSKAL_UNION_FIND, PRIM_BINARY_HEAP], dataframes_min, pred=lambda x: x['n'] <= 2000)
            split_and_plot([KRUSKAL_UNION_FIND, PRIM_BINARY_HEAP], dataframes_min, pred=lambda x: x['n'] < 20000)

        if IS_PLOT_MAIN_ENABLED:
            names_to_plot(programs, dataframes_min)   # all
            names_to_plot_logy(programs, dataframes_min)  # all logy
            names_to_plot([p for p in programs if p != KRUSKAL_SIMPLE], dataframes_min)  # all but naive
            names_to_plot([KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND, PRIM_BINARY_HEAP], dataframes_min)  # the three
            names_to_plot_logy([KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND, PRIM_BINARY_HEAP], dataframes_min)  # the three logy
            names_to_plot([KRUSKAL_UNION_FIND, PRIM_BINARY_HEAP], dataframes_min)  # the two
            names_to_plot([KRUSKAL_UNION_FIND, KRUSKAL_UNION_FIND_COMPRESSED], dataframes_min)  # kruskal versions
            names_to_plot([PRIM_BINARY_HEAP, PRIM_K_HEAP], dataframes_min)  # prim versions
            names_to_plot([PRIM_BINARY_HEAP, PRIM_K_HEAP], dataframes_min)  # prim2 vs primK
        
        if IS_PLOT_TABLE_ENABLED:
            names_to_plot_logy([KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND], dataframes_min)
            names_to_plot_logy([KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND_COMPRESSED], dataframes_min)
            names_to_plot_logy([KRUSKAL_SIMPLE, PRIM_BINARY_HEAP], dataframes_min)
            names_to_plot([KRUSKAL_UNION_FIND, KRUSKAL_UNION_FIND_COMPRESSED], dataframes_min)
            names_to_plot_logy([KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND], dataframes_min)
            names_to_plot_logy([KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND_COMPRESSED], dataframes_min)
            names_to_plot([PRIM_BINARY_HEAP, KRUSKAL_UNION_FIND], dataframes_min)
            names_to_plot([PRIM_BINARY_HEAP, KRUSKAL_UNION_FIND_COMPRESSED], dataframes_min)
            names_to_plot([PRIM_BINARY_HEAP, PRIM_K_HEAP], dataframes_min)
            names_to_plot([PRIM_K_HEAP, KRUSKAL_UNION_FIND], dataframes_min)
            names_to_plot([PRIM_K_HEAP, KRUSKAL_UNION_FIND_COMPRESSED], dataframes_min)
            names_to_plot([PRIM_BINARY_HEAP, PRIM_8_HEAP], dataframes_min)
            names_to_plot([PRIM_K_HEAP, PRIM_8_HEAP], dataframes_min)

    # plot_simple_bar(dataframes_min[KRUSKAL_SIMPLE], x='ms', y='n', title='Kruskal Simple')
    # plot_interesting_nodes_bar(dataframes_min[KRUSKAL_SIMPLE].round(1), title='Kruskal Simple')
    # plot_multiple_nodes_line(dataframes_min, title='Plot multiple', max_nodes=1000)
