--------------------------------------------------------
--  File created - Thursday-April-02-2020   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Table LEKARZ
--------------------------------------------------------

  CREATE TABLE "DKOLASKA"."LEKARZ" 
   (	"ID_LEKARZA" NUMBER(8,0), 
	"NAZWISKO" VARCHAR2(100 BYTE), 
	"IMIE" VARCHAR2(100 BYTE), 
	"SPECJALIZACJA" VARCHAR2(100 BYTE)
   ) SEGMENT CREATION DEFERRED 
  PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 COMPRESS FOR OLTP LOGGING
  TABLESPACE "II_DATA" ;
--------------------------------------------------------
--  DDL for Index LEKARZ_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "DKOLASKA"."LEKARZ_PK" ON "DKOLASKA"."LEKARZ" ("ID_LEKARZA") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS NOCOMPRESS LOGGING
  TABLESPACE "II_DATA" ;
--------------------------------------------------------
--  Constraints for Table LEKARZ
--------------------------------------------------------

  ALTER TABLE "DKOLASKA"."LEKARZ" ADD CONSTRAINT "LEKARZ_PK" PRIMARY KEY ("ID_LEKARZA")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS NOCOMPRESS LOGGING
  TABLESPACE "II_DATA"  ENABLE;
 
  ALTER TABLE "DKOLASKA"."LEKARZ" MODIFY ("ID_LEKARZA" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."LEKARZ" MODIFY ("NAZWISKO" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."LEKARZ" MODIFY ("IMIE" NOT NULL ENABLE);
