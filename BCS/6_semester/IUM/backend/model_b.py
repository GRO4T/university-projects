from sklearn.ensemble import RandomForestClassifier
import numpy as np


def get_model_b(data):
    X = data[['product_id', 'event_count', 'hour', 'price_log', 'user_id']]
    y = data['end_with_purchase']
    X = np.array(X)
    y = np.array(y).flatten()
    model = RandomForestClassifier(max_depth=20, n_estimators=50)
    model.fit(X, y)
    return model
