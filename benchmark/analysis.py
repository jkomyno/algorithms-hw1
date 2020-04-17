""""
The following script analyzes benchmarks for KruskalSimple, KruskalUnionFind (and variants) and
PrimBinaryHeap algorithms.

The aim of the script is to produce in output som graphical representation of
the benchmarks, in particular
- for each algorithm show how it scales with the size of the input
- for some (maybe fixed) input sizes show the compared performance of the
  implemented algorithms.
"""
from functools import reduce

import pandas as pd
import numpy as np
import glob
from itertools import chain
from tabulate import tabulate
from typing import List, Dict
import matplotlib.pyplot as plt

# number of floating point decimals displayed in output
N_DECIMALS = 3

# number of floating point decimals displayed in output for values that represent percentages
N_DECIMALS_PERCENTAGE = 2

KRUSKAL_SIMPLE = 'KruskalSimple'
KRUSKAL_UNION_FIND = 'KruskalUnionFind'
KRUSKAL_UNION_FIND_COMPRESSED = 'KruskalUnionFindCompressed'
PRIM_BINARY_HEAP = 'PrimBinaryHeap'

programs = [
    KRUSKAL_SIMPLE,
    KRUSKAL_UNION_FIND,
    KRUSKAL_UNION_FIND_COMPRESSED,
    PRIM_BINARY_HEAP
]

ms_programs = [
    'ms_kruskal_simple',
    'ms_kruskal_union_find',
    'ms_kruskal_union_find_compressed',
    'ms_prim_binary_heap'
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
    print(f'Comparison between {program_1} and {program_2}')

    df_comparison = compare_2_programs(dfs_dict, program_1, program_2)

    # tablefmt='latex' to print out \tabular{} LaTeX tables
    tablefmt = 'pretty'

    # pretty-print comparison DataFrame
    pretty_print_pandas(df_comparison, tablefmt)


def pretty_print_pandas(df: pd.DataFrame, tablefmt='pretty'):
    """
    Pretty-printer utility for Pandas DataFrames.
    :param df: DataFrame to pretty-print
    :param tablefmt: 'pretty' | 'psql' | 'latex'
    """
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


def plot_multiple_nodes_line(dfs: Dict[str, pd.DataFrame], title: str, max_nodes = 1000):
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


if __name__ == '__main__':
    # benchmark CSVs stored in DataFrames and grouped by program name
    generator = zip(programs, [read_csvs_of_program(program) for program in programs])
    dataframes: Dict[str, List[pd.DataFrame]] = dict(generator)

    # is some CSV is invalid, throws an AssertionError
    check_validity(dataframes.values())

    # for each CSV, only the minimum values for the 'ms' column are retained
    # len(dataframes_min) == len(programs)
    dataframes_min = minimize_ms_dataframes(dataframes)

    # compare multiple programs to show potential improvements
    print_comparison(dataframes_min, KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND)
    print_comparison(dataframes_min, KRUSKAL_SIMPLE, KRUSKAL_UNION_FIND_COMPRESSED)
    print_comparison(dataframes_min, KRUSKAL_SIMPLE, PRIM_BINARY_HEAP)
    print_comparison(dataframes_min, KRUSKAL_UNION_FIND, KRUSKAL_UNION_FIND_COMPRESSED)
    print_comparison(dataframes_min, PRIM_BINARY_HEAP, KRUSKAL_UNION_FIND)
    print_comparison(dataframes_min, PRIM_BINARY_HEAP, KRUSKAL_UNION_FIND_COMPRESSED)

    plot_simple_bar(dataframes_min[KRUSKAL_SIMPLE], x='ms', y='n', title='Kruskal Simple')
    plot_interesting_nodes_bar(dataframes_min[KRUSKAL_SIMPLE].round(1), title='Kruskal Simple')
    plot_multiple_nodes_line(dataframes_min, title='Plot multiple', max_nodes=1000)
