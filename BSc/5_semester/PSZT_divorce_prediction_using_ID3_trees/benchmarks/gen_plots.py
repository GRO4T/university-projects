"""
Author: Rafał Kulus
"""

import os
import pandas as pd
import matplotlib.pyplot as plt

min_val = 1


def make_plot(df, tree_type, group_type, group_val, set_name):
    global min_val
    dff = df.filter(regex=tree_type)
    title = f'{tree_type} {group_type}={group_val}'
    boxplot = dff.boxplot()
    boxplot.set_title(title)
    boxplot.set_xlabel(f'Rozmiar zbioru {set_name}')
    boxplot.set_ylabel(f'Dokładność modelu')
    plt.ylim(min_val, 1)
    # plt.show()
    plt.savefig(f'{path}/plots/{title}.png')
    boxplot.clear()


if __name__ == '__main__':
    path = os.path.dirname(__file__)
    _, _, filenames = next(os.walk(path))
    filenames = list(filter(lambda f: f[-4:] == '.csv', filenames))

    dfs = []
    for f in filenames:
        p_pos = f.find('_p')
        csv_pos = f.find('.csv')
        t = f[1:p_pos]
        p = f[p_pos+2:csv_pos]
        df = pd.read_csv(f'{path}/{f}', header=None, sep=' ')
        dfs.append((t, p, df))
        for key, val in df.iteritems():
            for x in val.array:
                min_val = min(min_val, x)

    by_t = []
    included_ts = set()
    for x in dfs:
        if x[0] not in included_ts:
            included_ts.add(x[0])
            gdf = pd.DataFrame()
            for y in dfs:
                if y[0] == x[0]:
                    y[2].columns = [f'ID3 p={y[1]}', f'Pruned p={y[1]}']
                    gdf = pd.concat([gdf, y[2]], axis=1)
            by_t.append((x[0], gdf))

    by_p = []
    included_ps = set()
    for x in dfs:
        if x[1] not in included_ps:
            included_ps.add(x[1])
            gdf = pd.DataFrame()
            for y in dfs:
                if y[1] == x[1]:
                    y[2].columns = [f'ID3 t={y[0]}', f'Pruned t={y[0]}']
                    gdf = pd.concat([gdf, y[2]], axis=1)
            by_p.append((x[1], gdf))

    for x in by_t:
        make_plot(x[1], 'ID3', 't', x[0], 'obcinającego')
        make_plot(x[1], 'Pruned', 't', x[0], 'obcinającego')

    for x in by_p:
        make_plot(x[1], 'ID3', 'p', x[0], 'testowego')
        make_plot(x[1], 'Pruned', 'p', x[0], 'testowego')
