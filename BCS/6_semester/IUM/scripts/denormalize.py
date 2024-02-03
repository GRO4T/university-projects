import jsonlines
import tabulate


DATA_DIR = '../data_v2'


def import_jsonl(path):
    data = []

    with jsonlines.open(path) as reader:
        for obj in reader:
            data.append(obj)

    return data


if __name__ == '__main__':

    users = import_jsonl(f'{DATA_DIR}/users.jsonl')
    sessions = import_jsonl(f'{DATA_DIR}/sessions.jsonl')
    deliveries = import_jsonl(f'{DATA_DIR}/deliveries.jsonl')
    products = import_jsonl(f'{DATA_DIR}/products.jsonl')

    headers = []
    data = []

    for s in sessions:
        user = next(filter(lambda u: u['user_id'] == s['user_id'], users), {})
        product = next(filter(lambda p: p['product_id'] == s['product_id'], products), {})
        delivery = next(filter(lambda d: d['purchase_id'] == s['purchase_id'], deliveries), {})

        row = {**s, **user, **product, **delivery}
        data.append(row)

        if not headers and user and product and delivery:
            headers = row.keys()

    with jsonlines.open('../denormalized.jsonl', mode='w') as writer:
        for d in data:
            writer.write(d)

    rows = []
    for d in data:
        vals = [d.get(x, '-') for x in headers]
        rows.append(vals)

    with open(f'../tables/denormalized.txt', 'w') as f:
        f.write(tabulate.tabulate(rows, headers))
