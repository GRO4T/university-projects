import random
import datetime

competencies = ["Programowanie w Javie", "Analiza wymagań", "Projektowanie systemów", "Programowanie C++",
                "Znajomość j.angielskiego", "Znajomość j.francuskiego", "Znajomość j.niemieckiego",
                "Obsługa taśmy produkcyjnej", "Prawo jazdy kat.B"]

responsibilities = ["Nadzór plac budowlanych", "Sporządzenie analizy wymagań biznesowych",
                    "Implementacja serwera webowego", "Nadzór nad działaniem systemu informatycznego",
                    "Konserwacja budynku", "Prace wykończeniowe"]

def generate_misc(employees):
    misc = generate_emp_competencies(employees)
    misc = misc + generate_absences_and_substitiutions(employees)
    misc = misc + generate_responsibilities(employees)
    return misc

def generate_emp_competencies(employees):
    emp_competencies = []
    for comp in competencies:
        emp_competencies.append([("kompetencja", {"nazwa": comp})])
    for i in range(len(employees)):
        emp_competencies.append([("kompetencje_pracownika", {
            "kompetencja_id": random.randint(1, len(competencies)),
            "pracownik_id": i+1
          })])
    return emp_competencies

def generate_absences_and_substitiutions(employees):
    abs_and_subs = []
    num_of_emps = len(employees)
    count = 0
    x = 1
    for i in range(1, num_of_emps - 1):
        abs_and_subs.append([("slownik_zastepstw", {
            "pracownik_kto": i,
            "pracownik_kogo": i+1
        })])
        r = random.randint(1, 11)
        for j in range(r):
            count += 1
            f = random.randint(0, 1)
            if f == 1:
                month = 1+j
                start = datetime.datetime(2021, month, random.randint(16, 28))
                end = datetime.datetime(2021, month+1, random.randint(1, 15))
            else:
                month = 1+j
                start = datetime.datetime(2020, month, random.randint(16, 28))
                end = datetime.datetime(2020, month+1, random.randint(1, 15))
            abs_and_subs.append([("nieobecnosci", {
                "poczatek": start,
                "koniec": end,
                "pracownik_id": i+1
            })])
            abs_and_subs.append([("zastepstwo", {
                "poczatek": start,
                "koniec": end,
                "nieobecnosci_id": count,
                "slowzast_id": x
            })])
        x += 1
    return abs_and_subs

def generate_responsibilities(employees):
    resp = []
    for i in range(len(employees)):
        month = random.randint(3, 11)
        start = datetime.datetime(2020, month, random.randint(1, 30))
        end = datetime.datetime(2020, month+1, random.randint(1, 30))
        resp.append([("zakres_obowiazkow", {
            "opis_obowiazku": random.choice(responsibilities),
            "data_dodania": start,
            "termin_wykonania": end,
            "pracownik_id": i+1
        })])
    return resp
