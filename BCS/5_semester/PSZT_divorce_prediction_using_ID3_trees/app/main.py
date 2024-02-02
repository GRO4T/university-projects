"""
Author: Damian Kolaska, Rafał Kulus
"""

import sys
import id3 as id3
from data_set import DataSet


def test(test_set, tree):
    acc, mse, me = id3.test(tree, test_set)
    rel_acc = 1 - me / class_range
    print(f"correctly classified: {acc}")
    print(f"mean squared error: {mse}")
    print(f"absolute mean error: {me}")
    print(f"relative acc: {rel_acc}")
    return acc


if __name__ == "__main__":
    if len(sys.argv) != 6:
        print("\nUsage: python3 program_name data_path class_col_name test_size prune_size runs")
        print("\ntest_size and prune_size are in range (0, 1)")
        print("\ndata_set is divided into temp_set and test_set")
        print("temp_set is then divided into training_set and prune_set\n")
        sys.exit()

    data_path = sys.argv[1]
    class_name = sys.argv[2]
    test_size = float(sys.argv[3])
    prune_size = float(sys.argv[4])
    runs = int(sys.argv[5])

    data = DataSet(data_path, class_name)
    class_range = len(id3.get_attr_values(data.data_set)[class_name])
    results_path = f'./benchmarks/t{sys.argv[3]}_p{sys.argv[4]}.csv'

    print("\ndata set: " + data_path)
    print("set size:          " + str(len(data.data_set.index)))
    print("------------------ ")
    print("training set size: " + str(len(data.train_set.index)))
    print("prune set size:    " + str(len(data.prune_set.index)))
    print("test set size:     " + str(len(data.test_set.index)))

    with open(results_path, 'w') as file:
        for i in range(runs):
            data.resplit_dataset(test_size=test_size, prune_size=prune_size)
            id3_tree = id3.build_id3(data.train_set, data.data_set)
            c45_tree = id3.build_c45(data.prune_set, data.data_set, id3_tree)
            print(f"\nrun {i}")
            print("ID3")
            acc_id3 = test(data.test_set, id3_tree)
            print("C45")
            acc_c45 = test(data.test_set, c45_tree)
            file.write(f'{acc_id3} {acc_c45}\n')
