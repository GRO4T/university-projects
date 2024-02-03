### Launch Turtlesim

```
roslaunch turtlesim siu.launch
```

### Convert coordinates

#### top-left -> bottom-left

```python
with open("./data/scenariusz.csv", "r") as in_file, open(
    "./data/scenariusz2.csv", "w"
) as out_file:
    for line in in_file.readlines():
        scen_id, id, x_min, x_max, y_min, y_max, x_g, y_g = line.split(";")
        new_y_min = str(1080 - int(y_min))
        new_y_max = str(1080 - int(y_max))
        new_y_g = str(1080 - int(y_g))
        out_file.write(
            f"{scen_id};{id};{x_min};{x_max};{new_y_min};{new_y_max};{x_g};{new_y_g}\n"
        )

```

#### pixels -> metres

```python
with open("./data/scenariusz.csv", "r") as in_file, open(
    "./data/scenariusz2.csv", "w"
) as out_file:
    for line in in_file.readlines():
        scen_id, id, x_min, x_max, y_min, y_max, x_g, y_g = line.split(";")
        x_min = round(int(x_min) / 22, 2)
        x_max = round(int(x_max) / 22, 2)
        y_min = round(int(y_min) / 22, 2)
        y_max = round(int(y_max) / 22, 2)
        x_g = round(int(x_g) / 22, 2)
        y_g = round(int(y_g) / 22, 2)
        out_file.write(f"{scen_id};{id};{x_min};{x_max};{y_min};{y_max};{x_g};{y_g}\n")
```

#### Load routes

```python
routes = {}

with open("data/routes.csv", encoding="utf-8-sig") as f:
    for line in f.readlines():
        route_id, rest = line.split(";", maxsplit=1)
        routes[route_id] = tuple(rest.strip().split(";"))

print(routes)

```