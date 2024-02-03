import numpy as np
from sklearn.metrics import accuracy_score


class ModelA:
    def __init__(self, data, top_n):
        self.bestsellers = data[data.event_type == 1].groupby(['product_id']).count().reset_index() \
                .loc[:, ['product_id', 'session_id']].rename(columns={'session_id': 'purchase_count'}) \
                .sort_values(by=['purchase_count'], ascending=False)
        self.bestsellers = np.array(self.bestsellers)
        self.top_n = top_n

    def predict(self, product_id) -> int:
        for product in self.bestsellers[:self.top_n]:
            if product_id == product[0]:
                return 1
        return 0
    
    def fit(self, data) -> list:
        res = [self.predict(x) for x in data]
        return res

    def evaluate(self, y, y_pred):
        print(f"acc: {accuracy_score(y, y_pred)}")
