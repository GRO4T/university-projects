from . import pracownik
from .hierarchy import generate_hierarchy
from .misc import generate_misc

from typing import List, Tuple, Dict

def generate(n):
    employees = generate_accounts_and_employees(n)
    company = generate_hierarchy(employees)
    misc = generate_misc(employees)
    return employees + company + misc

def generate_accounts_and_employees(n: int) -> List[List[Tuple[str, Dict]]]:
    """Outer list contains lists of related records represented as tuples (table name, record data)"""
    data = []
    emp_gen = pracownik.generator()
    for x in range(n):
        item = next(emp_gen)
        data.append(item)
    return data

if __name__ == "__main__":
    for x in range(10):
        print(generate_accounts_and_employees(1))
