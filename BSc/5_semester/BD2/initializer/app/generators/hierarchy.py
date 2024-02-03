director_roles = ["Prezes zarządu", "Dyrektor finansowy", "Reprezentant inwestorów"]
departments = ["Dział HR", "Dział sprzedaży", "Dział reklamy", "Dział rozwoju", "Dział finansów", "Dział produkcji"]
number_of_groups = 10 * len(departments)
number_of_teams = 10 * number_of_groups

def generate_hierarchy(employees):
    company = []
    num_of_employees = len(employees)
    start = 0
    end = len(director_roles)
    start, end = create_board_of_directors(company, employees, start, end)
    start, end = create_departments(company, employees, start, end)
    start, end = create_groups(company, employees, start, end)
    start, end = create_teams(company, employees, start, end)
    assign_to_teams(employees, start, num_of_employees)
    return company

def create_board_of_directors(company, employees, start, end):
    for i in range(start, end): # create board of directors
        board_member = {
            "nazwa_stanowiska": director_roles[i],
            "pracownik_id": i+1
        }
        company.append([("zarzad", board_member)])
    start = start + len(director_roles)
    end = end + len(departments)
    return start, end

def create_departments(company, employees, start, end):
    for i in range(start, end): # create company departments
        dep_number = i - len(director_roles)
        dep_name = departments[dep_number]
        board_member_id = 1
        if dep_name == "Dział finansów":
            board_member_id = 2
        dep = {
            "nazwa": departments[dep_number],
            "zarzad_id": board_member_id,
            "kierownik_id": i+1
        }
        company.append([("dzial", dep)])
        employees[i][1][1]["pracownik_id"] = board_member_id
    start = start + len(departments)
    end = end + number_of_groups
    return start, end

def create_groups(company, employees, start, end):
    for i in range(start, end): # create groups
        dep_id = (i % len(departments)) + 1
        group = {
            "nazwa": "Grupa" + str(i),
            "dzial_id": dep_id,
            "kierownik_id": i+1
        }
        company.append([("grupa", group)])
        employees[i][1][1]["pracownik_id"] = start + dep_id - len(departments)
    start = start + number_of_groups
    end = end + number_of_teams
    return start, end

def create_teams(company, employees, start, end):
    for i in range(start, end): # create teams
        group_id = (i % number_of_groups) + 1
        team = {
            "nazwa": "Zespół" + str(i),
            "grupa_id": group_id,
            "kierownik_id": i+1
        }
        company.append([("zespol", team)])
        employees[i][1][1]["pracownik_id"] = start + group_id - number_of_groups
    start = start + number_of_teams
    return start, end

def assign_to_teams(employees, start, end):
    for i in range(start, end):
        team_id = (i % number_of_teams) + 1
        employees[i][1][1]["pracownik_id"] = start + team_id - number_of_teams

