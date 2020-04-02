--------------------------------------------------------
--  File created - Thursday-April-02-2020   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Table DOKUMENT
--------------------------------------------------------

  CREATE TABLE "DKOLASKA"."DOKUMENT" 
   (	"NAZWA_DOKUMENTU" VARCHAR2(100 BYTE), 
	"NUMER_DOKUMENTU" VARCHAR2(30 BYTE), 
	"ORGAN_WYDAJACY" VARCHAR2(100 BYTE), 
	"ID_PACJENTA" NUMBER(6,0), 
	"PESEL_PACJENTA" NUMBER(11,0)
   ) SEGMENT CREATION DEFERRED 
  PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 COMPRESS FOR OLTP LOGGING
  TABLESPACE "II_DATA" ;
--------------------------------------------------------
--  DDL for Index DOKUMENT_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "DKOLASKA"."DOKUMENT_PK" ON "DKOLASKA"."DOKUMENT" ("NAZWA_DOKUMENTU", "NUMER_DOKUMENTU") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS NOCOMPRESS LOGGING
  TABLESPACE "II_DATA" ;
--------------------------------------------------------
--  Constraints for Table DOKUMENT
--------------------------------------------------------

  ALTER TABLE "DKOLASKA"."DOKUMENT" ADD CONSTRAINT "DOKUMENT_PK" PRIMARY KEY ("NAZWA_DOKUMENTU", "NUMER_DOKUMENTU")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS NOCOMPRESS LOGGING
  TABLESPACE "II_DATA"  ENABLE;
 
  ALTER TABLE "DKOLASKA"."DOKUMENT" MODIFY ("NAZWA_DOKUMENTU" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."DOKUMENT" MODIFY ("NUMER_DOKUMENTU" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."DOKUMENT" MODIFY ("ORGAN_WYDAJACY" NOT NULL ENABLE);
--------------------------------------------------------
--  Ref Constraints for Table DOKUMENT
--------------------------------------------------------

  ALTER TABLE "DKOLASKA"."DOKUMENT" ADD CONSTRAINT "DOKUMENT_FK_PACJENT" FOREIGN KEY ("ID_PACJENTA", "PESEL_PACJENTA")
	  REFERENCES "DKOLASKA"."PACJENT" ("ID_PACJENTA", "PESEL") ENABLE;
