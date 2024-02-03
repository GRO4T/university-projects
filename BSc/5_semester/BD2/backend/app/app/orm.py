from sqlalchemy import CHAR, Column, Date, ForeignKey, String, Table
from sqlalchemy.dialects.mysql import INTEGER
from sqlalchemy.orm import relationship
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()
metadata = Base.metadata


class Kompetencja(Base):
    __tablename__ = 'kompetencja'

    id = Column(INTEGER(4), primary_key=True)
    nazwa = Column(String(255), nullable=False, unique=True)

    pracowniks = relationship('Pracownik', secondary='kompetencje_pracownika')


class KontoUzytkownika(Base):
    __tablename__ = 'konto_uzytkownika'

    id = Column(INTEGER(5), primary_key=True)
    nazwa_uzytkownika = Column(String(64), nullable=False, unique=True)
    haslo = Column(CHAR(64), nullable=False)


class Pracownik(Base):
    __tablename__ = 'pracownik'

    id = Column(INTEGER(5), primary_key=True)
    imie = Column(String(64), nullable=False)
    nazwisko = Column(String(64), nullable=False)
    pracownik_id = Column(ForeignKey('pracownik.id'), index=True)
    pesel = Column(CHAR(11), nullable=False, unique=True)
    email = Column(String(64), nullable=False, unique=True)
    numer_telefonu = Column(String(20), nullable=False)
    konto_uzytkownika_id = Column(ForeignKey('konto_uzytkownika.id'), nullable=False, unique=True)

    konto_uzytkownika = relationship('KontoUzytkownika')
    pracownik = relationship('Pracownik', remote_side=[id])


t_kompetencje_pracownika = Table(
    'kompetencje_pracownika', metadata,
    Column('kompetencja_id', ForeignKey('kompetencja.id'), primary_key=True, nullable=False),
    Column('pracownik_id', ForeignKey('pracownik.id'), primary_key=True, nullable=False, index=True)
)


class Nieobecnosci(Base):
    __tablename__ = 'nieobecnosci'

    id = Column(INTEGER(7), primary_key=True)
    poczatek = Column(Date, nullable=False)
    koniec = Column(Date, nullable=False)
    pracownik_id = Column(ForeignKey('pracownik.id'), nullable=False, index=True)

    pracownik = relationship('Pracownik')


class SlownikZastepstw(Base):
    __tablename__ = 'slownik_zastepstw'

    id = Column(INTEGER(6), primary_key=True)
    pracownik_kto = Column(ForeignKey('pracownik.id'), nullable=False, index=True)
    pracownik_kogo = Column(ForeignKey('pracownik.id'), nullable=False, index=True)

    pracownik = relationship('Pracownik', primaryjoin='SlownikZastepstw.pracownik_kogo == Pracownik.id')
    pracownik1 = relationship('Pracownik', primaryjoin='SlownikZastepstw.pracownik_kto == Pracownik.id')


class ZakresObowiazkow(Base):
    __tablename__ = 'zakres_obowiazkow'

    id = Column(INTEGER(7), primary_key=True)
    opis_obowiazku = Column(String(255), nullable=False)
    data_dodania = Column(Date, nullable=False)
    termin_wykonania = Column(Date, nullable=False)
    pracownik_id = Column(ForeignKey('pracownik.id'), nullable=False, index=True)

    pracownik = relationship('Pracownik')


class Zarzad(Base):
    __tablename__ = 'zarzad'

    id = Column(INTEGER(2), primary_key=True)
    nazwa_stanowiska = Column(String(127), nullable=False, unique=True)
    pracownik_id = Column(ForeignKey('pracownik.id'), nullable=False, index=True)

    pracownik = relationship('Pracownik')


class Dzial(Base):
    __tablename__ = 'dzial'

    id = Column(INTEGER(2), primary_key=True)
    nazwa = Column(String(255), nullable=False, unique=True)
    zarzad_id = Column(ForeignKey('zarzad.id'), nullable=False, index=True)
    kierownik_id = Column(ForeignKey('pracownik.id'), nullable=False, unique=True)

    kierownik = relationship('Pracownik')
    zarzad = relationship('Zarzad')


class Zastepstwo(Base):
    __tablename__ = 'zastepstwo'

    id = Column(INTEGER(8), primary_key=True)
    poczatek = Column(Date, nullable=False)
    koniec = Column(Date, nullable=False)
    nieobecnosci_id = Column(ForeignKey('nieobecnosci.id'), nullable=False, index=True)
    slowzast_id = Column(ForeignKey('slownik_zastepstw.id'), nullable=False, index=True)

    nieobecnosci = relationship('Nieobecnosci')
    slowzast = relationship('SlownikZastepstw')


class Grupa(Base):
    __tablename__ = 'grupa'

    id = Column(INTEGER(2), primary_key=True)
    nazwa = Column(String(255), nullable=False, unique=True)
    dzial_id = Column(ForeignKey('dzial.id'), nullable=False, index=True)
    kierownik_id = Column(ForeignKey('pracownik.id'), nullable=False, index=True)

    dzial = relationship('Dzial')
    kierownik = relationship('Pracownik')


class Zespol(Base):
    __tablename__ = 'zespol'

    id = Column(INTEGER(2), primary_key=True)
    nazwa = Column(String(255), nullable=False, unique=True)
    grupa_id = Column(ForeignKey('grupa.id'), nullable=False, index=True)
    kierownik_id = Column(ForeignKey('pracownik.id'), nullable=False, unique=True)

    grupa = relationship('Grupa')
    kierownik = relationship('Pracownik')

