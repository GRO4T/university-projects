from flask import Flask
from flask_sqlalchemy import SQLAlchemy
import os

DB_USERNAME, DB_PASSWORD, DB_DATABASE = os.environ['DB_USERNAME'], os.environ['DB_PASSWORD'], os.environ['DB_DATABASE']

app = Flask(__name__)

app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://{}:{}@db/{}'.format(DB_USERNAME, DB_PASSWORD, DB_DATABASE)
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

app.config['SECRET_KEY'] = 'admin123'
app.config['JWT_AUTH_URL_RULE'] = "/api/auth"

db = SQLAlchemy(app)

from sqlalchemy.orm import sessionmaker, scoped_session
Session = scoped_session(sessionmaker(bind=db.engine, autocommit=False))

@app.teardown_appcontext
def shutdown_session(exception=None):
    Session.remove()

class get_session():
    def __init__(self):
        self.session = Session()
    def __enter__(self):
        return self.session
    def __exit__(self, *cos):
        self.session.close()

from app import routes
