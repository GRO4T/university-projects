"""
Author: Damian Kolaska
"""

import numpy as np
from numpy import log2 as log
import copy

eps = np.finfo(float).eps


def find_entropy(df):
    class_name = df.keys()[-1]  # To make the code generic, changing target variable class name
    entropy = 0
    values = df[class_name].unique()  # Get classes in data set
    for value in values:
        fraction = df[class_name].value_counts()[value] / len(df[class_name])
        entropy += -fraction * np.log2(fraction)
    return entropy


def find_entropy_attribute(df, attribute):
    class_name = df.keys()[-1]  # To make the code generic, changing target variable class name
    target_variables = df[class_name].unique()  # Get classes in data set
    variables = df[attribute].unique()  # Get attribute possible values
    entropy2 = 0
    for variable in variables:  # For each possible value
        entropy = 0
        for target_variable in target_variables:  # For each class
            num = len(df[attribute][df[attribute] == variable][df[class_name] == target_variable])
            den = len(df[attribute][df[attribute] == variable])
            fraction = num / (den + eps)
            entropy += -fraction * log(fraction + eps)
            fraction2 = den / len(df)
            entropy2 += -fraction2 * entropy
            return abs(entropy2)


def find_winner(df):
    inf_gain = []
    for key in df.keys()[:-1]:
        inf_gain.append(find_entropy(df) - find_entropy_attribute(df, key))
    return df.keys()[:-1][np.argmax(inf_gain)]


def get_subtable(df, attr, attr_value):
    return df[df[attr] == attr_value].reset_index(drop=True)


def get_attr_values(df):
    att_values = {}
    for column in df:
        att_values[column] = []
        for value in df[column]:
            att_values[column].append(value) if value not in att_values[column] else att_values[column]
    return att_values


def build_id3(train_set, original_data_set):
    att_values = get_attr_values(original_data_set)
    return __build_id3(train_set, att_values)


def __build_id3(df, attr_values, tree=None):
    class_name = df.keys()[-1]
    # Get attribute with maximum information gain
    best_attr = find_winner(df)
    # Create an empty dictionary to create tree
    if tree is None:
        tree = {best_attr: {}}

    # We make loop to construct a tree by calling this function recursively.
    # In this we check if the subset is pure and stops if it is pure.

    for attr_value in attr_values[best_attr]:
        subtable = get_subtable(df, best_attr, attr_value) # split data frame by each attribute value

        # if there no such value for this attribute assign most frequent class
        if len(subtable) == 0:
            most_freq_class = df[class_name].value_counts().idxmax()
            tree[best_attr][attr_value] = most_freq_class
            continue

        class_values, counts = np.unique(subtable[class_name], return_counts=True)

        if len(counts) == 1:  # Checking purity of subset
            tree[best_attr][attr_value] = class_values[0]
        else:
            tree[best_attr][attr_value] = __build_id3(subtable, attr_values)  # Calling the function recursively

    return tree


def build_c45(prune_set, original_data_set, id3_tree):
    id3_copy = copy.deepcopy(id3_tree)
    attr_values = get_attr_values(original_data_set)
    return __build_c45(prune_set, prune_set, id3_copy, attr_values)


def __build_c45(df, sub_df, tree, attr_values):
    class_name = df.keys()[-1]
    if type(tree) is not dict:  # leaf
        return tree
    attr_name = list(tree.keys())[0]
    for attr_value in attr_values[attr_name]:
        subtable = get_subtable(sub_df, attr_name, attr_value)
        tree[attr_name][attr_value] = __build_c45(df, subtable, tree[attr_name][attr_value], attr_values)
        # evaluate
        if sub_df[class_name].empty:
            return tree
        most_freq_class = sub_df[class_name].value_counts().idxmax()
        acc, mse, me = test(tree, df)
        p_tree = copy.deepcopy(tree)  # p for "pruning"
        p_tree[attr_name] = most_freq_class
        p_acc, p_mse, p_me = test(p_tree, df)
        if p_acc >= acc:
            return most_freq_class
    return tree


def test(tree, df):
    class_name = df.keys()[-1]  # To make the code generic, changing target variable class name
    data_len = df.shape[0]
    correct = 0
    mse = 0
    me = 0
    for index, row in df.iterrows():
        predicted_class = get_class(tree, row)
        real_class = row[class_name]
        if predicted_class == real_class:
            correct += 1
        mse += (predicted_class - real_class) ** 2
        me += abs(predicted_class - real_class)
    mse = mse / data_len
    me = me / data_len
    return correct / data_len, mse, me


def get_class(tree, row):
    if (type(tree)) is not dict:
        return tree
    attr_name = list(tree.keys())[0]
    try:
        subtree = tree[attr_name][row[attr_name]]
    except IndexError:
        return tree[attr_name]

    while type(subtree) is dict:
        attr_name = list(subtree.keys())[0]
        subtree = subtree[attr_name][row[attr_name]]
    return subtree
