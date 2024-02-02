CREATE OR REPLACE TRIGGER insert_nieobecnosc 
BEFORE INSERT ON nieobecnosci FOR EACH ROW 
BEGIN
	DECLARE is_overlapping_absence INT;
    
	IF NEW.poczatek >= NEW.koniec THEN 
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: początek musi być przed koniec'; 
    END IF;
	
    SET is_overlapping_absence = (SELECT COUNT(*) FROM nieobecnosci WHERE pracownik_id = NEW.pracownik_id AND koniec >= NEW.poczatek AND poczatek <= NEW.poczatek);
    IF is_overlapping_absence > 0 THEN
    	SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: nie można dodać nieobecności dla kogoś kto już jest nieobecny';
    END IF;
END;
CREATE OR REPLACE TRIGGER update_nieobecnosc 
BEFORE UPDATE ON nieobecnosci FOR EACH ROW 
BEGIN
	DECLARE is_overlapping_absence INT;
    
	IF NEW.poczatek >= NEW.koniec THEN 
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: początek musi być przed koniec'; 
    END IF;
	
    SET is_overlapping_absence = (SELECT COUNT(*) FROM nieobecnosci WHERE pracownik_id != OLD.pracownik_id AND pracownik_id = NEW.pracownik_id AND koniec >= NEW.poczatek AND poczatek <= NEW.poczatek);
    IF is_overlapping_absence > 0 THEN
    	SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: nie można dodać nieobecności dla kogoś kto już jest nieobecny';
    END IF;
END;

CREATE OR REPLACE TRIGGER insert_zastepstwo
BEFORE INSERT ON zastepstwo FOR EACH ROW
BEGIN
    DECLARE nieob_pocz DATE;
    DECLARE nieob_kon DATE;
    IF NEW.poczatek >= NEW.koniec THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: poczatek musi być przed koniec';
    END IF;
    SELECT poczatek, koniec INTO nieob_pocz, nieob_kon FROM nieobecnosci WHERE id = NEW.nieobecnosci_id;
    IF NEW.poczatek < nieob_pocz OR NEW.koniec > nieob_kon THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: zastępstwo musi być w ramach czasowych nieobecności';
    END IF;
END;
CREATE OR REPLACE TRIGGER update_zastepstwo
BEFORE UPDATE ON zastepstwo FOR EACH ROW
BEGIN
    DECLARE nieob_pocz DATE;
    DECLARE nieob_kon DATE;
    IF NEW.poczatek >= NEW.koniec THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: poczatek musi być przed koniec';
    END IF;
    SELECT poczatek, koniec INTO nieob_pocz, nieob_kon FROM nieobecnosci WHERE id = NEW.nieobecnosci_id;
    IF NEW.poczatek < nieob_pocz OR NEW.koniec > nieob_kon THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: zastępstwo musi być w ramach czasowych nieobecności';
    END IF;
END;

CREATE OR REPLACE TRIGGER insert_zakres_obowiazkow
BEFORE INSERT ON zakres_obowiazkow FOR EACH ROW
BEGIN
    IF NEW.data_dodania >= NEW.termin_wykonania THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: data_dodania musi być przed termin_wykonania';
    END IF;
END;
CREATE OR REPLACE TRIGGER update_zakres_obowiazkow
BEFORE UPDATE ON zakres_obowiazkow FOR EACH ROW
BEGIN
    IF NEW.data_dodania >= NEW.termin_wykonania THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: data_dodania musi być przed termin_wykonania';
    END IF;
END;

CREATE OR REPLACE TRIGGER new_password_different
BEFORE UPDATE ON konto_uzytkownika
FOR EACH ROW
BEGIN
  IF NEW.haslo = OLD.haslo
  THEN
   SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: Nowe hasło jest identyczne ze starym!';
  END IF;
END;

CREATE OR REPLACE TRIGGER insert_zarzad
BEFORE INSERT ON zarzad FOR EACH ROW 
BEGIN
	DECLARE has_superior INT;
   	SET has_superior = (SELECT COUNT(*) FROM pracownik WHERE pracownik.id = NEW.pracownik_id AND pracownik.pracownik_id IS NOT NULL);
   	IF has_superior > 0 THEN
    	SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: Członek zarządu nie może mieć przełożonego!';
    END IF;
END;
CREATE OR REPLACE TRIGGER update_zarzad
BEFORE UPDATE ON zarzad FOR EACH ROW 
BEGIN
	DECLARE has_superior INT;
   	SET has_superior = (SELECT COUNT(*) FROM pracownik WHERE pracownik.id = NEW.pracownik_id AND pracownik.pracownik_id IS NOT NULL);
   	IF has_superior > 0 THEN
    	SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: Członek zarządu nie może mieć przełożonego!';
    END IF;
END;

CREATE OR REPLACE TRIGGER insert_substitute_different
BEFORE INSERT ON slownik_zastepstw FOR EACH ROW 
BEGIN
  IF NEW.pracownik_kto = NEW.pracownik_kogo THEN
   SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: Nie można zastąpić pracownika tą samą osobą!';
  END IF;
END;
CREATE OR REPLACE TRIGGER update_substitute_different
BEFORE UPDATE ON slownik_zastepstw FOR EACH ROW 
BEGIN
  IF NEW.pracownik_kto = NEW.pracownik_kogo THEN
   SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: Nie można zastąpić pracownika tą samą osobą!';
  END IF;
END;

CREATE OR REPLACE TRIGGER insert_superior_different
BEFORE INSERT ON pracownik FOR EACH ROW
BEGIN
  IF NEW.id = NEW.pracownik_id THEN
   SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: Nie można być przełożonym dla samego siebie!';
  END IF;
END;
CREATE OR REPLACE TRIGGER update_superior_different
BEFORE UPDATE ON pracownik FOR EACH ROW
BEGIN
  IF NEW.id = NEW.pracownik_id THEN
   SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: Nie można być przełożonym dla samego siebie!';
  END IF;
END;

CREATE OR REPLACE PROCEDURE CheckManagers(IN kierownik_id INT)
BEGIN
	DECLARE is_manager_of_other_thing INT;
   	SET is_manager_of_other_thing = (
        SELECT COUNT(*) FROM 
        	(SELECT id FROM grupa WHERE kierownik_id = grupa.kierownik_id
        	UNION ALL
        	SELECT id FROM dzial WHERE kierownik_id = dzial.kierownik_id
        	UNION ALL
        	SELECT id FROM zespol WHERE kierownik_id = zespol.kierownik_id) temp);
   	IF is_manager_of_other_thing > 0 THEN
    	SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: Nie można być kierownikiem kilku jednostek jednocześnie!';
    END IF;
END;

CREATE OR REPLACE TRIGGER insert_grupa
BEFORE INSERT ON grupa FOR EACH ROW 
BEGIN
	CALL CheckManagers(NEW.kierownik_id);
END;

CREATE OR REPLACE TRIGGER update_grupa
BEFORE UPDATE ON grupa FOR EACH ROW 
BEGIN
	CALL CheckManagers(NEW.kierownik_id);
END;

CREATE OR REPLACE TRIGGER insert_dzial
BEFORE INSERT ON dzial FOR EACH ROW 
BEGIN
	CALL CheckManagers(NEW.kierownik_id);
END;

CREATE OR REPLACE TRIGGER update_dzial
BEFORE UPDATE ON dzial FOR EACH ROW 
BEGIN
	CALL CheckManagers(NEW.kierownik_id);
END;

CREATE OR REPLACE TRIGGER insert_zespol
BEFORE INSERT ON zespol FOR EACH ROW 
BEGIN
	CALL CheckManagers(NEW.kierownik_id);
END;

CREATE OR REPLACE TRIGGER update_zespol
BEFORE UPDATE ON zespol FOR EACH ROW 
BEGIN
	CALL CheckManagers(NEW.kierownik_id);
END;

CREATE TRIGGER `insert_wpis_slownik` BEFORE INSERT ON `slownik_zastepstw`
 FOR EACH ROW BEGIN
    DECLARE has_identical INT;
       SET has_identical = (SELECT COUNT(*) FROM slownik_zastepstw WHERE slownik_zastepstw.pracownik_kto = NEW.pracownik_kto AND slownik_zastepstw.pracownik_kogo = NEW.pracownik_kogo);
       IF has_identical > 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Błąd: Nie można dodać istniejącego wpisu!';
    END IF;
END;
