import jsonlines
import json
import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split


def read_jsonl(path):
    data = []

    with jsonlines.open(path) as reader:
        for obj in reader:
            data.append(obj)

    return pd.DataFrame(data).convert_dtypes()

sessions = read_jsonl('./sessions.jsonl')

sessions_sample = sessions.groupby('session_id').count().reset_index().session_id.sample(frac=0.1).reset_index().session_id
sessions_sample = np.array(sessions_sample)
sessions_simulation = []
sessions_model = []

for index, row in sessions.iterrows():
    if row.session_id in sessions_sample:
        sessions_simulation.append(row.to_dict())
    else:
        sessions_model.append(row.to_dict())

sessions_simulation = pd.DataFrame(sessions_simulation)
sessions_model = pd.DataFrame(sessions_model)

with open('simulation/sessions.jsonl', 'w') as file:
    file.write(sessions_simulation.to_json(orient='records', lines=True))

with open('model/sessions.jsonl', 'w') as file:
    file.write(sessions_model.to_json(orient='records', lines=True))



