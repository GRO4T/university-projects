import pathlib
from random import randint, choices
import hashlib
import string


def generator():
    names_m = []
    names_f = []
    lasts_m = []
    lasts_f = []

    read_csv(f"{pathlib.Path(__file__).parent}/data/names_m.csv", names_m)
    read_csv(f"{pathlib.Path(__file__).parent}/data/names_f.csv", names_f)
    read_csv(f"{pathlib.Path(__file__).parent}/data/lasts_m.csv", lasts_m)
    read_csv(f"{pathlib.Path(__file__).parent}/data/lasts_f.csv", lasts_f)

    email_domains = ["@gmail.com", "@wp.pl", "@onet.pl", "@hotmail.com",
                     "@outlook.com", "@o2.pl", "@interia.pl", "@yahoo.com"]

    account_id = 1
    while True:
        employee = dict()

        sex = randint(0, 1)
        if sex == 0:
            employee["imie"] = names_m[randint(0, len(names_m)-1)].capitalize().replace("'", " ")
            employee["nazwisko"] = lasts_m[randint(0, len(lasts_m)-1)].capitalize().replace("'", " ")
        else:
            employee["imie"] = names_f[randint(0, len(names_f)-1)].capitalize().replace("'", " ")
            employee["nazwisko"] = lasts_f[randint(0, len(lasts_f)-1)].capitalize().replace("'", " ")

        employee["pesel"] = _generate_pesel()
        employee["email"] = f'{employee["imie"].lower()}.{employee["nazwisko"].lower()}.' \
                            f'{employee["pesel"][0:2]}{email_domains[randint(0, len(email_domains)-1)]}'
        employee["numer_telefonu"] = _generate_mobile()

        # password = ''.join(choices(string.ascii_lowercase + string.digits, k=randint(8, 32)))
        password =employee["imie"][0].lower() + employee["nazwisko"].lower() + employee["pesel"][0:2]
        account = {
            "nazwa_uzytkownika": employee["imie"][0].lower() + employee["nazwisko"].lower() + employee["pesel"][0:2],
            "haslo": hashlib.sha256(password.encode("utf-8")).hexdigest()
        }
        employee['konto_uzytkownika_id'] = account_id

        ret = [
            ("konto_uzytkownika", account),
            ("pracownik", employee)
        ]
        account_id = account_id + 1
        yield ret


def _generate_pesel():
    pesel = str(randint(50, 99)) + str(randint(1, 12)).zfill(2) \
            + str(randint(1, 28)).zfill(2) + str(randint(0, 99999)).zfill(5)
    return pesel


def _generate_mobile():
    mobile = str(randint(5, 8))
    for x in range(8):
        mobile = mobile + str(randint(0, 9))
    return mobile


def read_csv(filename, dest):
    with open(filename, encoding="utf_8") as f:
        data = f.read().splitlines()
        for x in data:
            dest.append(x.split(",")[0])


if __name__ == "__main__":
    generator = generator()
    for i in range(100):
        print(next(generator))
