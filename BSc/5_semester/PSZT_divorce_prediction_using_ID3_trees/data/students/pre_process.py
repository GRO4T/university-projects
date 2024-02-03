import pandas as pd

data_set = pd.read_csv("data/students/student-mat.csv", sep=";", index_col=0)
print(data_set.keys())
data_set.to_csv("data/students/preprocessed/student-mat.csv", sep=";", index=False)

data_set = pd.read_csv("data/students/student-por.csv", sep=";", index_col=0)
data_set.to_csv("data/students/preprocessed/student-por.csv", sep=";", index=False)
