import networkx as nx
import pandas as pd
import math


def create_bipartite_graph(set_0, set_1, weights=None) -> nx.Graph:
    graph = nx.Graph()
    graph.add_nodes_from(set_0, bipartite=0)
    graph.add_nodes_from(set_1, bipartite=1)
    if weights is not None:
        edges = list(zip(set_0, set_1, weights))
        graph.add_weighted_edges_from(edges)
    else:
        edges = list(zip(set_0, set_1))
        graph.add_edges_from(edges)

    return graph


def create_bipartite_layout(set_0, set_1) -> dict:
    layout = {}
    layout.update((n, (1, i)) for i, n in enumerate(set_0))
    layout.update((n, (2, i)) for i, n in enumerate(set_1))

    return layout


def sum_node_weights(graph, nodes, columns) -> pd.DataFrame:
    weight_sum = [
        (n, sum(graph.edges[n, neighbor]["weight"] for neighbor in graph.neighbors(n)))
        for n in nodes
    ]
    return pd.DataFrame(weight_sum, columns=columns)


def get_entity_share(graph, nodes) -> dict:
    result = {}
    for substance_node in nodes:
        total_refund = 0.0
        refund_per_entity = []
        for entity_node in graph.neighbors(substance_node):
            refund = graph.edges[substance_node, entity_node]["weight"]
            refund_per_entity.append([entity_node, refund])
            total_refund += refund

        for i in range(len(refund_per_entity)):
            refund_per_entity[i][1] = round(refund_per_entity[i][1] / total_refund, 3)

        result[substance_node] = (refund_per_entity, total_refund)

    return result

    # {Substancja: ([(Producent, udzial), (producent2, udzial)], suma)}


def get_favored_producer(data):
    producer_wins = {}
    monopolists = 0
    for substance in data.values():
        if len(substance[0]) == 1:
            monopolists += 1
        else:
            producer_with_biggest_share = max(substance[0], key=lambda x: x[1])
            best_producer_per_substance = producer_with_biggest_share[0]
            producer_wins[best_producer_per_substance] = (
                producer_wins.get(best_producer_per_substance, 0) + 1
            )

    return (producer_wins, monopolists)


def get_substances(graph, nodes) -> dict:
    result = {entity_node: list(graph.neighbors(entity_node)) for entity_node in nodes}

    return result
