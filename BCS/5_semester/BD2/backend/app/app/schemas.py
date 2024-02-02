import app.orm as orm
from pydantic_sqlalchemy import sqlalchemy_to_pydantic

Pracownik = sqlalchemy_to_pydantic(orm.Pracownik)
Dzial = sqlalchemy_to_pydantic(orm.Dzial)
Zastepstwo = sqlalchemy_to_pydantic(orm.Zastepstwo)
Nieobecnosci = sqlalchemy_to_pydantic(orm.Nieobecnosci)
Kompetencja = sqlalchemy_to_pydantic(orm.Kompetencja)
SlownikZastepstw = sqlalchemy_to_pydantic(orm.SlownikZastepstw)
