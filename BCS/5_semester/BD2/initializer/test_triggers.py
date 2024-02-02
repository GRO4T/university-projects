import pymysql
from app.connection import TEST_CONN
import datetime

DATA_SIZE = 1000

def sql_and_assert(cursor, sql, val):
    passed = True
    try:
        cursor.execute(sql)
    except Exception as e:
        print(e)
        passed = False
    assert passed == val

def get_unit_id(cursor, unit_name, condition):
    sql = "SELECT id FROM {} {}".format(unit_name, condition)
    cursor.execute(sql)
    return cursor.fetchone()[0]

def get_unit_superior_id(cursor, unit_name, condition):
    sql = "SELECT kierownik_id FROM {} {}".format(unit_name, condition)
    cursor.execute(sql)
    return cursor.fetchone()[0]

def get_emp_id(cursor, condition):
    sql = "SELECT id FROM pracownik {}".format(condition)
    cursor.execute(sql)
    return cursor.fetchone()[0]

def get_emp_superior_id(cursor, emp_id):
    sql = "SELECT pracownik_id FROM pracownik WHERE id = {}".format(emp_id)
    cursor.execute(sql)
    return cursor.fetchone()[0]

def test_insert_zarzad():
    with TEST_CONN.cursor() as cursor:
        # próba dodania jako członka zarządu kierownika działu
        sql = "SELECT kierownik_id FROM dzial"
        cursor.execute(sql)
        department_manager = cursor.fetchone()[0]
        sql = "INSERT INTO zarzad (nazwa_stanowiska, pracownik_id) VALUES('zarzad_test_bad', {})".format(department_manager)
        sql_and_assert(cursor, sql, False)
        # dodanie nowego pracownika i mianowanie go członkiem zarządu
        sql = "INSERT INTO konto_uzytkownika VALUES(99999, 'user', 'haslo')"
        cursor.execute(sql)
        sql = "INSERT INTO pracownik VALUES (99999, 'Jan', 'Kowalski', NULL, '99052312345', 'test@gmail.com', '123456789', 99999)"
        cursor.execute(sql)
        sql = "INSERT INTO zarzad (nazwa_stanowiska, pracownik_id) VALUES('zarzad_test_bad', 99999)"
        sql_and_assert(cursor, sql, True)
        TEST_CONN.rollback()

def test_update_zarzad():
    with TEST_CONN.cursor() as cursor:
        # przypisanie roli członka zarządu kierownikowi działu
        sql = "SELECT kierownik_id FROM dzial"
        cursor.execute(sql)
        department_manager = cursor.fetchone()[0]
        sql = "UPDATE zarzad SET pracownik_id = {} LIMIT 1".format(department_manager)
        sql_and_assert(cursor, sql, False)
        TEST_CONN.rollback()

def test_insert_dzial():
    with TEST_CONN.cursor() as cursor:
        # dodanie działu, którego kierownikiem jest lider zespołu
        manager_of_other_unit = get_unit_superior_id(cursor, "zespol", "LIMIT 1")
        board_id = get_unit_id(cursor, "zarzad", "LIMIT 1")
        sql = "INSERT INTO dzial (nazwa, zarzad_id, kierownik_id) VALUES('dzial_test_bad', {}, {})".format(board_id, manager_of_other_unit)
        sql_and_assert(cursor, sql, False)
        # dodanie działu, którego kierownikiem jest pracownik zespołu
        normal_employee = get_emp_id(cursor, "ORDER BY ID DESC LIMIT 1")    # może działać niepoprawnie jeśli generator nie będzie
                                                                            # przydzielał pracowników w kolejności
        sql = "INSERT INTO dzial (nazwa, zarzad_id, kierownik_id) VALUES('dzial_test_good', {}, {})".format(board_id, normal_employee)
        sql_and_assert(cursor, sql, True)
        TEST_CONN.rollback()

def test_update_dzial():
    with TEST_CONN.cursor() as cursor:
        # próba przypisania roli kierownika działu liderowi zespołu
        manager_of_other_unit = get_unit_superior_id(cursor, "zespol", "LIMIT 1")
        sql = "UPDATE dzial SET kierownik_id = {} LIMIT 1".format(manager_of_other_unit)
        sql_and_assert(cursor, sql, False)
        # przypisanie roli kierownika działu pracownikowi zespołu
        normal_employee = get_emp_id(cursor, "ORDER BY ID DESC LIMIT 1")    # może działać niepoprawnie jeśli generator nie będzie
                                                                            # przydzielał pracowników w kolejności
        sql = "UPDATE dzial SET kierownik_id = {} LIMIT 1".format(normal_employee)
        sql_and_assert(cursor, sql, True)
        TEST_CONN.rollback()

def test_insert_grupa():
    with TEST_CONN.cursor() as cursor:
        # próba dodania grupy, której liderem jest lider zespołu
        manager_of_other_unit = get_unit_superior_id(cursor, "zespol", "LIMIT 1")
        dep_id = get_unit_id(cursor, "dzial", "LIMIT 1")
        sql = "INSERT INTO grupa (nazwa, dzial_id, kierownik_id) VALUES('dzial_test_bad', {}, {})".format(dep_id, manager_of_other_unit)
        sql_and_assert(cursor, sql, False)
        # dodanie grupy, której liderem jest pracownik zespołu
        normal_employee = get_emp_id(cursor, "ORDER BY ID DESC LIMIT 1")    # może działać niepoprawnie jeśli generator nie będzie
                                                                            # przydzielał pracowników w kolejności
        sql = "INSERT INTO grupa (nazwa, dzial_id, kierownik_id) VALUES('dzial_test_good', {}, {})".format(dep_id, normal_employee)
        sql_and_assert(cursor, sql, True)
        TEST_CONN.rollback()

def test_update_grupa():
    with TEST_CONN.cursor() as cursor:
        # próba przypisania roli lidera grupy liderowi zespołu
        manager_of_other_unit = get_unit_superior_id(cursor, "zespol", "LIMIT 1")
        sql = "UPDATE grupa SET kierownik_id = {} LIMIT 1".format(manager_of_other_unit)
        sql_and_assert(cursor, sql, False)
        # przypisanie roli lidera grupy pracownikowi zespołu
        normal_employee = get_emp_id(cursor, "ORDER BY ID DESC LIMIT 1")    # może działać niepoprawnie jeśli generator nie będzie
                                                                            # przydzielał pracowników w kolejności
        sql = "UPDATE grupa SET kierownik_id = {} LIMIT 1".format(normal_employee)
        sql_and_assert(cursor, sql, True)
        TEST_CONN.rollback()

def test_insert_zespol():
    with TEST_CONN.cursor() as cursor:
        # próba dodania zespołu, którego liderem jest lider grupy
        manager_of_other_unit = get_unit_superior_id(cursor, "grupa", "LIMIT 1")
        group_id = get_unit_id(cursor, "grupa", "LIMIT 1")
        sql = "INSERT INTO zespol (nazwa, grupa_id, kierownik_id) VALUES('dzial_test_bad', {}, {})".format(group_id, manager_of_other_unit)
        sql_and_assert(cursor, sql, False)
        # dodanie zespołu, której liderem jest pracownik zespołu
        normal_employee = get_emp_id(cursor, "ORDER BY ID DESC LIMIT 1")    # może działać niepoprawnie jeśli generator nie będzie
                                                                            # przydzielał pracowników w kolejności
        sql = "INSERT INTO zespol (nazwa, grupa_id, kierownik_id) VALUES('dzial_test_bad', {}, {})".format(group_id, normal_employee)
        sql_and_assert(cursor, sql, True)
        TEST_CONN.rollback()

def test_update_zespol():
    with TEST_CONN.cursor() as cursor:
        # próba przypisania roli lidera zespołu liderowi grupy
        manager_of_other_unit = get_unit_superior_id(cursor, "grupa", "LIMIT 1")
        sql = "UPDATE zespol SET kierownik_id = {} LIMIT 1".format(manager_of_other_unit)
        sql_and_assert(cursor, sql, False)
        # przypisanie roli lidera zespołu liderowi grupy
        normal_employee = get_emp_id(cursor, "ORDER BY ID DESC LIMIT 1")    # może działać niepoprawnie jeśli generator nie będzie
                                                                            # przydzielał pracowników w kolejności
        sql = "UPDATE zespol SET kierownik_id = {} LIMIT 1".format(normal_employee)
        sql_and_assert(cursor, sql, True)
        TEST_CONN.rollback()

def test_update_nieobecnosc():
    with TEST_CONN.cursor() as cursor:
        # próba modyfikacji nieobecności ustawiając błędne ramy czasowe
        sql = "SELECT pracownik_kogo FROM slownik_zastepstw LIMIT 1"
        cursor.execute(sql)
        emp_id = cursor.fetchone()[0]
        start = "2020-05-18"
        end = "2020-04-18"
        sql = "UPDATE nieobecnosci SET poczatek = '{}', koniec = '{}' LIMIT 1".format(start, end)
        sql_and_assert(cursor, sql, False)
        # prawidłowa modyfikacja
        start = "2020-05-18"
        end = "2020-06-18"
        sql = "UPDATE nieobecnosci SET poczatek = '{}', koniec = '{}' WHERE pracownik_id = {}".format(start, end, emp_id)
        sql_and_assert(cursor, sql, True)

def test_insert_zastepstwo():
    with TEST_CONN.cursor() as cursor:
        # koniec przed początkiem
        sql = "SELECT id, poczatek, koniec, pracownik_id FROM nieobecnosci LIMIT 1"
        cursor.execute(sql)
        abs_id, start, end, emp_id = cursor.fetchone()
        sql = "SELECT id FROM slownik_zastepstw WHERE pracownik_kogo = {}".format(emp_id)
        cursor.execute(sql)
        sub_id = cursor.fetchone()[0]
        sql = "INSERT INTO zastepstwo (poczatek, koniec, nieobecnosci_id, slowzast_id) VALUES ('{}', '{}', {}, {})".format(end, start, abs_id, sub_id)
        sql_and_assert(cursor, sql, False)
        # poprawne
        start = start + datetime.timedelta(days=1)
        sql = "INSERT INTO zastepstwo (poczatek, koniec, nieobecnosci_id, slowzast_id) VALUES ('{}', '{}', {}, {})".format(start, end, abs_id, sub_id)
        sql_and_assert(cursor, sql, True)
        # zastępstwo poza ramami czasowymi
        start = start - datetime.timedelta(days=1)
        end = end + datetime.timedelta(days=1)
        sql = "INSERT INTO zastepstwo (poczatek, koniec, nieobecnosci_id, slowzast_id) VALUES ('{}', '{}', {}, {})".format(start, end, abs_id, sub_id)
        sql_and_assert(cursor, sql, False)

def test_update_zastepstwo():
    with TEST_CONN.cursor() as cursor:
        # koniec przed początkiem
        sql = "SELECT poczatek, koniec FROM nieobecnosci WHERE id = 1"
        cursor.execute(sql)
        start, end = cursor.fetchone()
        sql = "UPDATE zastepstwo SET poczatek = '{}', koniec = '{}' WHERE nieobecnosci_id = 1".format(end, start)
        sql_and_assert(cursor, sql, False)
        # zastępstwo poza ramami czasowymi
        start = start - datetime.timedelta(days=1)
        end = end + datetime.timedelta(days=1)
        sql = "UPDATE zastepstwo SET poczatek = '{}', koniec = '{}' WHERE nieobecnosci_id = 1".format(start, end)
        sql_and_assert(cursor, sql, False)
        # poprawne
        sql = "SELECT poczatek, koniec FROM nieobecnosci WHERE id = 1"
        cursor.execute(sql)
        start, end = cursor.fetchone()
        if (start != end):
            start = start + datetime.timedelta(days=1)
        sql = "UPDATE zastepstwo SET poczatek = '{}', koniec = '{}' WHERE nieobecnosci_id = 1".format(start, end)
        sql_and_assert(cursor, sql, True)

def test_insert_zakres_obowiazkow():
    with TEST_CONN.cursor() as cursor:
        # koniec przed początkiem
        early = "2020-04-18"
        late = "2020-05-18"
        sql = "INSERT INTO zakres_obowiazkow(opis_obowiazku, data_dodania, termin_wykonania, pracownik_id) VALUES ('TEST', '{}', '{}', 1)".format(late, early)
        sql_and_assert(cursor, sql, False)
        # poprawne
        sql = "INSERT INTO zakres_obowiazkow(opis_obowiazku, data_dodania, termin_wykonania, pracownik_id) VALUES ('TEST', '{}', '{}', 1)".format(early, late)
        sql_and_assert(cursor, sql, True)

def test_update_zakres_obowiazkow():
    with TEST_CONN.cursor() as cursor:
        # koniec przed początkiem
        early = "2020-04-18"
        late = "2020-05-18"
        sql = "UPDATE zakres_obowiazkow SET data_dodania = '{}', termin_wykonania = '{}' WHERE id = 1".format(late, early)
        sql_and_assert(cursor, sql, False)
        # poprawne
        sql = "UPDATE zakres_obowiazkow SET data_dodania = '{}', termin_wykonania = '{}' WHERE id = 1".format(early, late)
        sql_and_assert(cursor, sql, True)

def test_new_password_different():
    with TEST_CONN.cursor() as cursor:
        sql = "SELECT haslo FROM konto_uzytkownika WHERE id = 1"
        cursor.execute(sql)
        passwd = cursor.fetchone()[0]
        # to samo haslo
        sql = "UPDATE konto_uzytkownika SET haslo = '{}' WHERE id = 1".format(passwd)
        sql_and_assert(cursor, sql, False)
        # inne haslo
        sql = "UPDATE konto_uzytkownika SET haslo = '{}' WHERE id = 1".format('__Test__')
        sql_and_assert(cursor, sql, True)
"""
def test_insert_substitute_different():
    with TEST_CONN.cursor() as cursor:
        # ten sam zastępujacy co zastepowany
        sql = "INSERT INTO slownik_zastepstw(pracownik_kto, pracownik_kogo) VALUES ({}, {})".format(1, 1)
        sql_and_assert(cursor, sql, False)
        # poprawne
        sql = "INSERT INTO slownik_zastepstw(pracownik_kto, pracownik_kogo) VALUES ({}, {})".format(1, 2)
        sql_and_assert(cursor, sql, True)
        """

def test_update_substitute_different():
    with TEST_CONN.cursor() as cursor:
        # ten sam zastępujacy co zastepowany
        sql = "UPDATE slownik_zastepstw SET pracownik_kto = {}, pracownik_kogo = {} WHERE id = 1".format(1, 1)
        sql_and_assert(cursor, sql, False)
        # poprawne
        sql = "UPDATE slownik_zastepstw SET pracownik_kto = {}, pracownik_kogo = {} WHERE id = 1".format(1, 2)
        sql_and_assert(cursor, sql, True)

def test_insert_superior_different():
    with TEST_CONN.cursor() as cursor:
        sql = "INSERT INTO konto_uzytkownika(nazwa_uzytkownika, haslo) VALUES ('JunitTester11', 'xdxdxdxdxd')"
        cursor.execute(sql)
        sql = "SELECT id FROM konto_uzytkownika ORDER BY ID DESC LIMIT 1"
        cursor.execute(sql)
        new_id = cursor.fetchone()[0]
        # przełożony samego siebie
        sql = "INSERT INTO pracownik(id, imie, nazwisko, pracownik_id, pesel, email, numer_telefonu, konto_uzytkownika_id) VALUES ({}, '{}', '{}', {}, '{}', '{}', '{}', {})".format(99999, "Junit", "Tester", 99999, "1049710247", "e.mail@gmail.com","111222333", new_id)
        sql_and_assert(cursor, sql, False)
        # poprawne
        sql = "INSERT INTO pracownik(id, imie, nazwisko, pracownik_id, pesel, email, numer_telefonu, konto_uzytkownika_id) VALUES ({}, '{}', '{}', {}, '{}', '{}', '{}', {})".format(99999, "Junit", "Tester", 1, "1049710247", "e.mail@gmail.com","111222333", new_id)
        sql_and_assert(cursor, sql, True)

def test_update_superior_different():
    with TEST_CONN.cursor() as cursor:
        # przełożony samego siebie
        sql = "UPDATE pracownik SET pracownik_id = {} WHERE id = 1".format(1)
        sql_and_assert(cursor, sql, False)
        # poprawne
        sql = "UPDATE pracownik SET pracownik_id = {} WHERE id = 1".format(99999)
        sql_and_assert(cursor, sql, True)
