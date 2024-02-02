from app import get_session
from flask import jsonify
import app.schemas as schemas
import app.orm as orm
from app.request_models import CreateAbsenceRequest, GetPresentTimePeriodRequest, CreateSubRequest, GetAbleToSubstituteRequest, AddSubDictRequest
from app.request_models import GetSkillsRequest
from sqlalchemy import or_, and_
from app.exception import NoEntryInSubDict


def pack_in_json(query_result, convert_to_schema):
    with get_session() as session:
        return jsonify([convert_to_schema(res).dict() for res in query_result])

def get_all_employees():
    with get_session() as session:
        return pack_in_json(session.query(orm.Pracownik), schemas.Pracownik.from_orm)

def get_employee_by_id(id):
    with get_session() as session:
        return pack_in_json(session.query(orm.Pracownik).filter_by(id=id), schemas.Pracownik.from_orm)

def get_employee_by_username(username):
    with get_session() as session:
        return pack_in_json(
            session.query(orm.Pracownik).join(orm.Pracownik.konto_uzytkownika).filter_by(nazwa_uzytkownika=username),
            schemas.Pracownik.from_orm
        )

def get_all_departments():
    with get_session() as session:
        return pack_in_json(session.query(orm.Dzial), schemas.Dzial.from_orm)

def get_all_subs_for_emp(emp_id):
    with get_session() as session:
        query_result = session.query(orm.Pracownik.imie, orm.Pracownik.nazwisko, orm.Zastepstwo.poczatek, orm.Zastepstwo.koniec).\
            join(orm.SlownikZastepstw, orm.Zastepstwo.slowzast_id == orm.SlownikZastepstw.id).\
            join(orm.Pracownik, orm.SlownikZastepstw.pracownik_kogo == orm.Pracownik.id).\
            filter(orm.SlownikZastepstw.pracownik_kto == emp_id).all()
        response = [{"imie": res.imie, "nazwisko": res.nazwisko, "poczatek": res.poczatek, "koniec": res.koniec} for res in query_result]
        return jsonify(response)

def get_all_abs_for_emp(emp_id):
    with get_session() as session:
        query_result = session.query(orm.Nieobecnosci).\
            filter(orm.Nieobecnosci.pracownik_id == emp_id).all()
        return pack_in_json(query_result, schemas.Nieobecnosci.from_orm)

def get_employee_by_subordinate(subordinate_id):
    with get_session() as session:
        superior_id = session.query(orm.Pracownik).filter_by(id=subordinate_id).one().pracownik_id
        query_result = session.query(orm.Pracownik).filter_by(id=superior_id)
        return pack_in_json(query_result, schemas.Pracownik.from_orm)

def get_employee_by_superior(superior_id):
    with get_session() as session:
        return pack_in_json(
            session.query(orm.Pracownik).filter_by(pracownik_id=superior_id).all(),
            schemas.Pracownik.from_orm
        )

def insert_absence(request: CreateAbsenceRequest):
    with get_session() as session:
        absence = orm.Nieobecnosci(poczatek=request.poczatek, koniec=request.koniec, pracownik_id=request.id)
        try:
            session.add(absence)
            session.commit()
        except Exception as e:
            print(e)
            session.rollback()
            raise

def delete_absence(id):
    with get_session() as session:
        del_rows = 0
        try:
            session.query(orm.Zastepstwo).filter_by(nieobecnosci_id=id).delete()
            del_rows = session.query(orm.Nieobecnosci).filter_by(id=id).delete()
            session.commit()
        except Exception as e:
            print(e)
            session.rollback()
        return del_rows

def get_all_subs_for_abs(abs_id):
    with get_session() as session:
        return pack_in_json(
            session.query(orm.Zastepstwo).filter_by(nieobecnosci_id=abs_id).all(),
            schemas.Zastepstwo.from_orm
        )

def get_subordinate_abs_and_subs(superior_id):
    with get_session() as session:
        response_data = []
        suboridanates = session.query(orm.Pracownik).filter_by(pracownik_id=superior_id).all()
        for suboridinate in suboridanates:
            emp_data = schemas.Pracownik.from_orm(suboridinate).dict()
            absences = session.query(orm.Nieobecnosci).filter_by(pracownik_id=suboridinate.id).all()
            emp_data["absences"] =  [schemas.Nieobecnosci.from_orm(row).dict() for row in absences]
            for i in range(len(absences)):
                subs = session.query(orm.Zastepstwo).filter_by(nieobecnosci_id=absences[i].id).all()
                sub_data = []
                for j in range(len(subs)):
                    sub_data.append(schemas.Zastepstwo.from_orm(subs[j]).dict())
                    substitute = session.query(orm.Pracownik).join(
                        orm.SlownikZastepstw,
                        orm.SlownikZastepstw.pracownik_kto == orm.Pracownik.id
                    ).filter(orm.SlownikZastepstw.id == subs[j].slowzast_id).one()
                    sub_data[j]["imie"] = substitute.imie
                    sub_data[j]["nazwisko"] = substitute.nazwisko
                emp_data["absences"][i]["substitutions"] = sub_data
            response_data.append(emp_data)
        return jsonify(response_data)

def get_subordinate_present_in_period(request: GetPresentTimePeriodRequest):
    with get_session() as session:
        query_result = session.query(orm.Pracownik).\
            join(orm.Nieobecnosci, orm.Nieobecnosci.pracownik_id == orm.Pracownik.id).\
            filter(
                and_(
                    or_(
                        orm.Nieobecnosci.koniec < request.poczatek, orm.Nieobecnosci.poczatek > request.koniec
                    ),
                    orm.Pracownik.pracownik_id == request.id_przelozonego
                )
            ).all()
        return pack_in_json(query_result, schemas.Pracownik.from_orm)

def insert_substitution(request: CreateSubRequest):
    with get_session() as session:
        absence = session.query(orm.Nieobecnosci).filter_by(id=request.id_nieobecnosci).one()
        sub_dict = session.query(orm.SlownikZastepstw).filter(
            and_(
                orm.SlownikZastepstw.pracownik_kogo == absence.pracownik_id,
                orm.SlownikZastepstw.pracownik_kto == request.id_pracownika
            )
        ).one_or_none()

        if sub_dict is None:
            raise NoEntryInSubDict()

        sub = orm.Zastepstwo(
            poczatek=absence.poczatek, koniec=absence.koniec, nieobecnosci_id=absence.id, slowzast_id=sub_dict.id
        )
        try:
            session.add(sub)
            session.commit()
        except Exception as e:
            print(e)
            session.rollback()
            raise

def delete_substitution(id):
    with get_session() as session:
        del_rows = 0
        try:
            del_rows = session.query(orm.Zastepstwo).filter_by(id=id).delete()
            session.commit()
        except Exception as e:
            print(e)
            session.rollback()
        return del_rows

def get_able_to_substitute(request: GetAbleToSubstituteRequest):
    with get_session() as session:
        absence = session.query(orm.Nieobecnosci).filter_by(id=request.id_nieobecnosci).one()
        emp_absent = session.query(orm.Pracownik).filter_by(id=absence.pracownik_id).one()
        query_result = session.query(orm.Pracownik).\
            join(orm.SlownikZastepstw, orm.Pracownik.id == orm.SlownikZastepstw.pracownik_kto).\
            filter(orm.SlownikZastepstw.pracownik_kogo == emp_absent.id).all()
        return pack_in_json(query_result, schemas.Pracownik.from_orm)

def insert_sub_dict(request: AddSubDictRequest):
    with get_session() as session:
        sub_dict = orm.SlownikZastepstw(pracownik_kto=request.kto, pracownik_kogo=request.kogo)
        try:
            session.add(sub_dict)
            session.commit()
        except Exception as e:
            session.rollback()
            print(e)
            raise

def delete_sub_dict(id):
    with get_session() as session:
        del_rows = 0
        sub_dict = session.query(orm.SlownikZastepstw).filter_by(id=id).one()
        absences = session.query(orm.Nieobecnosci).filter_by(pracownik_id=sub_dict.pracownik_kogo).all()
        try:
            for ab in absences:
                session.query(orm.Zastepstwo).filter_by(nieobecnosci_id=ab.id).delete()
                session.query(orm.Nieobecnosci).filter_by(id=ab.id).delete()
            del_rows = session.query(orm.SlownikZastepstw).filter_by(id=id).delete()
            session.commit()
        except Exception as e:
            print(e)
            session.rollback()
        return del_rows

def get_all_skills(request: GetSkillsRequest):
    with get_session() as session:
        emp = session.query(orm.Pracownik).filter_by(id=request.id_pracownika).one()
        skills_id = session.execute(f"SELECT * FROM kompetencje_pracownika WHERE pracownik_id={request.id_pracownika}")
        result = []
        for skill_id in skills_id.fetchall():
            print(skill_id)
            skill = session.query(orm.Kompetencja).filter_by(id=skill_id[0]).one()
            result.append(schemas.Kompetencja.from_orm(skill).dict())
        return jsonify(result)

def get_sub_dict_by_substitute(id):
    with get_session() as session:
        return pack_in_json(
            session.query(orm.SlownikZastepstw).filter_by(pracownik_kto=id).all(),
            schemas.SlownikZastepstw.from_orm
        )

def get_sub_dict_by_absent(id):
    with get_session() as session:
        return pack_in_json(
            session.query(orm.SlownikZastepstw).filter_by(pracownik_kogo=id).all(),
            schemas.SlownikZastepstw.from_orm
        )

def get_sub_dict_by_superior(id):
    with get_session() as session:
        subordinates = session.query(orm.Pracownik).filter_by(pracownik_id=id).all()
        result = []
        for subordinate in subordinates:
            sub_dicts = session.query(orm.SlownikZastepstw).filter(
                or_(
                    orm.SlownikZastepstw.pracownik_kto == subordinate.id,
                    orm.SlownikZastepstw.pracownik_kogo == subordinate.id
                )
            ).all()
            for sub_dict in sub_dicts:
                sub_result = schemas.SlownikZastepstw.from_orm(sub_dict).dict()
                emp1 = session.query(orm.Pracownik).filter_by(id=sub_dict.pracownik_kto).one()
                emp2 = session.query(orm.Pracownik).filter_by(id=sub_dict.pracownik_kogo).one()
                sub_result["pracownik_kto_imie"] = emp1.imie
                sub_result["pracownik_kto_nazwisko"] = emp1.nazwisko
                sub_result["pracownik_kogo_imie"] = emp2.imie
                sub_result["pracownik_kogo_nazwisko"] = emp2.nazwisko
                if sub_result not in result:
                    result.append(sub_result)
        return jsonify(result)

