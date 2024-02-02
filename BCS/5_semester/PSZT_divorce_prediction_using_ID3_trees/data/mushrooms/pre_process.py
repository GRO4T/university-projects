import pandas as pd

di = {"p": 1, "e": 0}

data_set = pd.read_csv("data/mushrooms/agaricus-lepiota.data", sep=",")
data_set = data_set.replace({"class": di})
data_set.to_csv("data/mushrooms/agaricus-lepiota.csv", sep=";", index=False)

