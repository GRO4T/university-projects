CREATE INDEX RECEPTA_IDX_DATA ON recepta(data_wystawienia);
SELECT /*+ INDEX(RECEPTA RECEPTA_IDX_DATA) */ * FROM recepta WHERE LOWER(tresc) LIKE '%leki%';
SELECT /*+ NO_INDEX(RECEPTA RECEPTA_IDX_DATA) */ * FROM recepta WHERE data_wystawienia = '20-MAY-2020';

create or replace function randomString(len number)
return varchar2
    is
        str varchar2(256);
        str2 varchar2(256);
    begin
        with strings as (
          select 'leki' as s from dual union all
          select 'szkla' as s from dual union all
          select 'krople' as s from dual union all
          select 'ziola' as s from dual
         )
        select
           (select s
            from (select s from strings order by dbms_random.value) s
            where rownum = 1
           ) as RandomString
        into str    
        from dual;
        
        str2 := DBMS_RANDOM.string('x', 10);
        str := str || str2;

return str;
end;
/

create or replace function genPesel
return number
    is
        pesel number;
    begin
        with numeryPesel as (
          select 4250920012 as s from dual union all
          select 63011800512 as s from dual
         )
        select
           (select s
            from (select s from numeryPesel order by dbms_random.value) s
            where rownum = 1
           ) as RandomString
        into pesel    
        from dual;
return pesel;
end;
/

create or replace function genIdLekarza
return number
    is
        id number;
    begin
        with idLekarza as (
          select 91276345 as s from dual union all
          select 45359321 as s from dual union all
          select 83681822 as s from dual union all
          select 1 as s from dual union all
          select 2 as s from dual union all
          select 4 as s from dual union all
          select 5 as s from dual union all
          select 6 as s from dual union all
          select 91285354 as s from dual
         )
        select
           (select s
            from (select s from idLekarza order by dbms_random.value) s
            where rownum = 1
           ) as randomId
        into id    
        from dual;
return id;
end;
/



create or replace PROCEDURE rowgen2(no_row IN NUMBER, first_id IN NUMBER)
AS
BEGIN
    DECLARE
        data_wystawienia recepta.data_wystawienia%type;
        tresc recepta.tresc%type;
        pesel_pacjenta recepta.pesel_pacjenta%type;
        id_pacjenta recepta.id_pacjenta%type;
        id_lekarza recepta.id_lekarza%type;
    BEGIN
        DBMS_OUTPUT.ENABLE;
      FOR i IN first_id..no_row+first_id-1
      LOOP
        data_wystawienia := sysdate-DBMS_RANDOM.value()*365;
        tresc := randomString(8);
        pesel_pacjenta := genPesel();
        SELECT id_pacjenta INTO id_pacjenta FROM pacjent WHERE pesel = pesel_pacjenta;
        id_lekarza := genIdLekarza();
        --DBMS_OUTPUT.PUT_LINE('id lekarza ' || id_lekarza);
        --DBMS_OUTPUT.PUT_LINE('id ' || id_pacjenta);
        --DBMS_OUTPUT.PUT_LINE('pesel ' || pesel_pacjenta);
        --DBMS_OUTPUT.PUT_LINE('tresc ' || tresc);
        --DBMS_OUTPUT.PUT_LINE(data_wystawienia);
        INSERT INTO recepta VALUES (i, data_wystawienia, tresc, pesel_pacjenta, id_pacjenta, id_lekarza);
      END LOOP;  
    END;
END rowgen2;
/



CALL rowgen2(1000, 110);
SELECT * FROM pacjent;