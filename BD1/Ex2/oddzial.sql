--------------------------------------------------------
--  File created - Thursday-April-02-2020   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Table ODDZIAL_NFZ
--------------------------------------------------------

  CREATE TABLE "DKOLASKA"."ODDZIAL_NFZ" 
   (	"NAZWA_ODDZIALU" VARCHAR2(100 BYTE), 
	"COLUMN1" VARCHAR2(100 BYTE), 
	"COLUMN2" VARCHAR2(100 BYTE)
   ) SEGMENT CREATION DEFERRED 
  PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 COMPRESS FOR OLTP LOGGING
  TABLESPACE "II_DATA" ;
--------------------------------------------------------
--  DDL for Index ODDZIAL_NFZ_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "DKOLASKA"."ODDZIAL_NFZ_PK" ON "DKOLASKA"."ODDZIAL_NFZ" ("NAZWA_ODDZIALU") 
  PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS NOCOMPRESS LOGGING
  TABLESPACE "II_DATA" ;
--------------------------------------------------------
--  Constraints for Table ODDZIAL_NFZ
--------------------------------------------------------

  ALTER TABLE "DKOLASKA"."ODDZIAL_NFZ" ADD CONSTRAINT "ODDZIAL_NFZ_PK" PRIMARY KEY ("NAZWA_ODDZIALU")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS NOCOMPRESS LOGGING
  TABLESPACE "II_DATA"  ENABLE;
 
  ALTER TABLE "DKOLASKA"."ODDZIAL_NFZ" MODIFY ("NAZWA_ODDZIALU" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."ODDZIAL_NFZ" MODIFY ("COLUMN1" NOT NULL ENABLE);
 
  ALTER TABLE "DKOLASKA"."ODDZIAL_NFZ" MODIFY ("COLUMN2" NOT NULL ENABLE);
