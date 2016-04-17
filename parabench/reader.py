import pandas as pd

def read_file(csvfile):
    dataframe = pd.read_csv(csvfile)
    dataframe.index.names = ['rank']
    return dataframe

def read_files(csv_files):
    dataframes = [read_file(csvfile) for csvfile in csv_files]
    return dataframes
