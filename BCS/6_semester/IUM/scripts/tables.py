import jsonlines
import csv


DATA_DIR = '../data_v2'


def save_as_table(path):
    data = []

    with jsonlines.open(path) as reader:
        for obj in reader:
            data.append(obj)

    header = data[0].keys()
    rows = [x.values() for x in data]

    filename = path[path.rfind('/')+1:]
    new_filename = f'{filename[:filename.rfind(".")]}.csv'
    sep = ';'

    with open(f'../tables/{new_filename}', 'w', newline='') as f:
        f.write(f'sep={sep}\n')
        writer = csv.writer(f, delimiter=sep)
        writer.writerow(header)
        for r in rows:
            writer.writerow(r)


if __name__ == '__main__':
    save_as_table(f'{DATA_DIR}/users.jsonl')
    save_as_table(f'{DATA_DIR}/sessions.jsonl')
    save_as_table(f'{DATA_DIR}/deliveries.jsonl')
    save_as_table(f'{DATA_DIR}/products.jsonl')
