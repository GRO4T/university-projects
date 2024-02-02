from app import app, get_session
from app.orm import KontoUzytkownika
import app.crud as crud
import app.schemas as schemas
from app.request_models import CreateAbsenceRequest, GetPresentTimePeriodRequest, CreateSubRequest
from app.request_models import GetAbleToSubstituteRequest, AddSubDictRequest, GetSkillsRequest
from app.exception import NoEntryInSubDict
from flask_pydantic import validate
from flask import request
from http import HTTPStatus

from flask_jwt import JWT, jwt_required, current_identity
from werkzeug.security import safe_str_cmp
import hashlib
from app.orm import *


@app.route('/api/test')
def index():
    return { 'status': 'OK' }


def auth(username, password):
    with get_session() as session:
        user = session.query(KontoUzytkownika).filter_by(nazwa_uzytkownika=username).first()
        if user is not None and user.haslo == hashlib.sha256(password.encode()).hexdigest():
            return user

def identity(payload):
    with get_session() as session:
        user_id = payload['identity']
        return session.query(KontoUzytkownika).filter_by(id=user_id).first()

jwt = JWT(app, auth, identity)

@app.route('/api/auth_test')
@jwt_required()
def auth_test():
    return "%s" % current_identity


@app.route('/api/department', methods=['GET'])
def department():
    return crud.get_all_departments()

@app.route('/api/employee', methods=['GET'])
def employee():
    return crud.get_all_employees()

@app.route('/api/employee/by-id/<id>', methods=['GET'])
def employee_by_id(id):
    return crud.get_employee_by_id(id)

@app.route('/api/employee/by-username/<username>', methods=['GET'])
def employee_by_username(username):
    return crud.get_employee_by_username(username)

@app.route('/api/substitution/by-substitute/<id>', methods=['GET'])
def subs_by_substitute(id):
    return crud.get_all_subs_for_emp(id)

@app.route('/api/substitution', methods=['POST'])
@validate(body=CreateSubRequest)
def add_substitution():
    try:
        crud.insert_substitution(request.body_params)
    except NoEntryInSubDict as e:
        return {"msg": str(e)}, HTTPStatus.CONFLICT
    except Exception as e:
        print(e)
        return {}, HTTPStatus.CONFLICT
    return {}, HTTPStatus.OK

@app.route('/api/substitution/<id>', methods=['DELETE'])
def delete_substitution(id):
    affected_rows = crud.delete_substitution(id)
    return {"affected rows": affected_rows}, HTTPStatus.OK

@app.route('/api/absence/by-emp-id/<id>', methods=['GET'])
def abs_by_emp_id(id):
    return crud.get_all_abs_for_emp(id)

@app.route('/api/employee/by-subordinate/<id>', methods=['GET'])
def employee_by_subordinate(id):
    return crud.get_employee_by_subordinate(id)

@app.route('/api/employee/by-superior/<id>', methods=['GET'])
def employee_by_superior(id):
    return crud.get_employee_by_superior(id)

@app.route('/api/absence', methods=['POST'])
@validate(body=CreateAbsenceRequest)
def add_absence():
    try:
        crud.insert_absence(request.body_params)
    except Exception as e:
        print(e)
        return {}, HTTPStatus.CONFLICT
    return {}, HTTPStatus.OK

@app.route('/api/absence/<id>', methods=['DELETE'])
def delete_absence(id):
    affected_rows = crud.delete_absence(id)
    return {"affected rows": affected_rows}, HTTPStatus.OK

@app.route('/api/employee/<id>/subordinate-abs-subs', methods=['GET'])
def subordinate_abs_and_subs(id):
    return crud.get_subordinate_abs_and_subs(id)

@app.route('/api/employee/present-timeperiod', methods=['GET'])
@validate(body=GetPresentTimePeriodRequest)
def subordinate_present():
    return crud.get_subordinate_present_in_period(request.body_params)

@app.route('/api/employee/able-to-substitute', methods=['POST'])
@validate(body=GetAbleToSubstituteRequest)
def get_able_to_substitute():
    return crud.get_able_to_substitute(request.body_params)

@app.route('/api/sub-dict', methods=['POST'])
@validate(body=AddSubDictRequest)
def add_sub_dict():
    try:
        crud.insert_sub_dict(request.body_params)
    except Exception as e:
        print(e)
        return {}, HTTPStatus.CONFLICT
    return {}, HTTPStatus.OK

@app.route('/api/sub-dict/<id>', methods=['DELETE'])
def delete_sub_dict(id):
    affected_rows = crud.delete_sub_dict(id)
    return {"affected rows": affected_rows}, HTTPStatus.OK

@app.route('/api/sub-dict/by-substitute/<id>', methods=['GET'])
def get_sub_dict_by_substitute(id):
    return crud.get_sub_dict_by_substitute(id)

@app.route('/api/sub-dict/by-absent/<id>', methods=['GET'])
def get_sub_dict_by_absent(id):
    return crud.get_sub_dict_by_absent(id)

@app.route('/api/sub-dict/by-superior/<id>')
def get_sub_dict_by_superior(id):
    return crud.get_sub_dict_by_superior(id)

@app.route('/api/skills', methods=['POST'])
@validate(body=GetSkillsRequest)
def get_skills():
    return crud.get_all_skills(request.body_params)
