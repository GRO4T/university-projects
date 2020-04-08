--------------------------------------------------------
--  File created - Thursday-April-02-2020   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Table PRZEBYTA_CHOROBA
--------------------------------------------------------

  CREATE TABLE "DKOLASKA"."PRZEBYTA_CHOROBA" 
   (	"ID_CHOROBY" NUMBER(10,0), 
	"NAZWA_CHOROBY" VARCHAR2(100 BYTE), 
	"DATA_WYSTAPIENIA_CHOROBY" DATE, 
	"DATA_WYZDROWIENIA" DATE, 
	"ID_PACJENTA" NUMBER(6,0), 
	"PESEL_PACJENTA" NUMBER(11,0)
   ) SEGMENT CREATION DEFERRED 
  PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 COMPRESS FOR OLTP LOGGING
  TABLESPACE "II_DATA" ;
--------------------------------------------------------
--  DDL for Index PRZEBYTA_CHOROBA_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "DKOLASKA"."PRZEBYTA_CHOROBA_PK" ON "DKOLASKA"."PRZEBYTA_CHOROBA" ("ID_CHOROBY") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS NOCOMPRESS LOGGING
  TABLESPACE "II_DATA" ;
--------------------------------------------------------
--  Constraints for Table PRZEBYTA_CHOROBA
--------------------------------------------------------

  ALTER TABLE "DKOLASKA"."PRZEBYTA_CHOROBA" ADD CONSTRAINT "PRZEBYTA_CHOROBA_PK" PRIMARY KEY ("ID_CHOROBY")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS NOCOMPRESS LOGGING
  TABLESPACE "II_DATA"  ENABLE;
 
  ALTER TABLE "DKOLASKA"."PRZEBYTA_CHOROBA" MODIFY ("ID_CHOROBY" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."PRZEBYTA_CHOROBA" MODIFY ("NAZWA_CHOROBY" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."PRZEBYTA_CHOROBA" MODIFY ("DATA_WYSTAPIENIA_CHOROBY" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."PRZEBYTA_CHOROBA" MODIFY ("ID_PACJENTA" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."PRZEBYTA_CHOROBA" MODIFY ("PESEL_PACJENTA" NOT NULL ENABLE);
--------------------------------------------------------
--  Ref Constraints for Table PRZEBYTA_CHOROBA
--------------------------------------------------------

  ALTER TABLE "DKOLASKA"."PRZEBYTA_CHOROBA" ADD CONSTRAINT "PRZEBYTA_CHOROBA_PACJENT" FOREIGN KEY ("ID_PACJENTA", "PESEL_PACJENTA")
	  REFERENCES "DKOLASKA"."PACJENT" ("ID_PACJENTA", "PESEL") ENABLE;