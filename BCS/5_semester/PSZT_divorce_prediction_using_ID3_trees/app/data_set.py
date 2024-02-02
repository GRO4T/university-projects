"""
Author: Damian Kolaska, Rafał Kulus
"""

import pandas as pd
from sklearn.model_selection import train_test_split


class DataSet:
    def __init__(self, path, class_name, test_size=0.2, prune_size=0.2):
        self.data_set = pd.read_csv(path, sep=";")
        self.train_set = []
        self.prune_set = []
        self.test_set = []
        df1 = self.data_set.pop(class_name)
        self.data_set[class_name] = df1
        self.resplit_dataset(test_size, prune_size)

    def resplit_dataset(self, test_size, prune_size):
        temp_set, self.test_set = train_test_split(self.data_set, test_size=test_size)
        self.train_set, self.prune_set = train_test_split(temp_set, test_size=prune_size)
