--DROP TABLE  produs;
--DROP TABLE livrare;
--DROP TABLE  oprire_transport;
--DROP TABLE transport;
--DROP TABLE oprire;
--DROP TABLE  sofer;
--DROP TABLE  vehicul; 
--DROP TABLE  angajat;
--DROP TABLE  functie;
--DROP TABLE  depozit; 
--DROP TABLE  magazin;
--DROP TABLE  lutier;
--DROP TABLE  locatie;

CREATE TABLE locatie
(cod_locatie NUMBER(10) NOT NULL,
judet VARCHAR2(20),
oras VARCHAR2(20),
strada VARCHAR2(20) ,
numar_cladire NUMBER(10),
apartament NUMBER(10),
CONSTRAINT pk_cod_locatie PRIMARY KEY (cod_locatie)
);

CREATE TABLE lutier
(cod_lutier NUMBER(10) NOT NULL,
nume VARCHAR2(20),
prenume VARCHAR2(20),
nr_tel  NUMBER(10) ,
email VARCHAR2(20),
cod_locatie NUMBER(10) NOT NULL,
CONSTRAINT pk_cod_lutier PRIMARY KEY (cod_lutier),
CONSTRAINT fk_cod_locatie FOREIGN KEY (cod_locatie) 
REFERENCES locatie(cod_locatie)
);

CREATE TABLE magazin
(cod_magazin NUMBER(10) NOT NULL,
nume VARCHAR2(20),
ora_deschidere NUMBER(4),
ora_inchidere NUMBER(4),
cod_locatie NUMBER(10) NOT NULL,
CONSTRAINT pk_cod_magazin PRIMARY KEY (cod_magazin),
CONSTRAINT fk1_cod_locatie FOREIGN KEY (cod_locatie) 
REFERENCES locatie(cod_locatie),
CONSTRAINT chk_ora_deschidere CHECK (ora_deschidere >= 0 AND ora_deschidere < 2400),
CONSTRAINT chk_ora_inchidere CHECK (ora_inchidere >= 0 AND ora_inchidere < 2400)
);

CREATE TABLE depozit
(cod_depozit NUMBER(10) NOT NULL,
nume VARCHAR2(20),
cod_locatie NUMBER(10) NOT NULL,
CONSTRAINT pk_cod_depozit PRIMARY KEY (cod_depozit),
CONSTRAINT fk2_cod_locatie FOREIGN KEY (cod_locatie) 
REFERENCES locatie(cod_locatie)
);

CREATE TABLE functie
(cod_functie NUMBER(10) NOT NULL,
nume VARCHAR2(20),
salariu NUMBER(10),
CONSTRAINT pk_cod_functie PRIMARY KEY (cod_functie),
CONSTRAINT chk_salariu CHECK (salariu > 0)
);

CREATE TABLE angajat
(cod_angajat NUMBER(10) NOT NULL,
nume VARCHAR2(20),
prenume VARCHAR2(20),
data_angajare DATE,
cod_magazin NUMBER(10) NOT NULL,
cod_functie NUMBER(10) NOT NULL,
CONSTRAINT pk_cod_angajat PRIMARY KEY (cod_angajat),
CONSTRAINT fk1_cod_magazin FOREIGN KEY (cod_magazin) 
REFERENCES magazin(cod_magazin),
CONSTRAINT fk_cod_functie FOREIGN KEY (cod_functie) 
REFERENCES functie (cod_functie)
);

CREATE TABLE vehicul
(nr_inmat VARCHAR2(20) NOT NULL,
categorie VARCHAR2(20) NOT NULL,
ani_vechime NUMBER(3),
cod_depozit NUMBER(10) NOT NULL,
CONSTRAINT pk_nr_inmat PRIMARY KEY (nr_inmat),
CONSTRAINT fk1_cod_depozit FOREIGN KEY (cod_depozit) 
REFERENCES depozit(cod_depozit)
);

CREATE TABLE sofer
(cod_sofer NUMBER(10) NOT NULL,
nume VARCHAR2(20),
prenume VARCHAR2(20),
telefon NUMBER(20),
categorie_carnet VARCHAR2(20) NOT NULL,
cod_depozit NUMBER(10) NOT NULL,
CONSTRAINT pk_cod_sofer PRIMARY KEY (cod_sofer),
CONSTRAINT fk2_cod_depozit FOREIGN KEY (cod_depozit) 
REFERENCES depozit(cod_depozit)
);

CREATE TABLE oprire
(localitate VARCHAR2(20) NOT NULL,
CONSTRAINT pk_localitate PRIMARY KEY (localitate)
);

CREATE TABLE transport
(cod_transport NUMBER(10) NOT NULL,
nr_inmat VARCHAR2(20) NOT NULL,
cod_sofer NUMBER(10) NOT NULL,
data_plecare DATE,
ora_plecare NUMBER(4),
CONSTRAINT pk_cod_transport PRIMARY KEY (cod_transport),
CONSTRAINT fk1_cod_sofer FOREIGN KEY (cod_sofer) 
REFERENCES sofer(cod_sofer),
CONSTRAINT fk1_nr_inmat FOREIGN KEY (nr_inmat) 
REFERENCES vehicul(nr_inmat),
CONSTRAINT chk1_ora_plecare CHECK (ora_plecare >= 0 AND ora_plecare < 2400)
);

CREATE TABLE oprire_transport
(cod_transport NUMBER(10) NOT NULL,
localitate VARCHAR2(20) NOT NULL,
include_vama VARCHAR2(2),
durata VARCHAR2(20),
CONSTRAINT pk_cod_transport_localitate PRIMARY KEY (cod_transport, localitate),
CONSTRAINT fk_cod_transport FOREIGN KEY (cod_transport) 
REFERENCES transport(cod_transport),
CONSTRAINT fk_localitate FOREIGN KEY (localitate) 
REFERENCES oprire(localitate),
CONSTRAINT chk_include_vama CHECK (LOWER(include_vama) LIKE 'da' OR LOWER(include_vama) LIKE 'nu')
);

CREATE TABLE livrare
(cod_livrare NUMBER(10) NOT NULL,
cod_transport NUMBER(10) NOT NULL,
cod_magazin NUMBER(10) NOT NULL,
status_livrare VARCHAR2(20) NOT NULL,
CONSTRAINT pk_cod_livrare PRIMARY KEY (cod_livrare),
CONSTRAINT fk1_cod_transport FOREIGN KEY (cod_transport) 
REFERENCES transport(cod_transport),
CONSTRAINT fk2_cod_magazin FOREIGN KEY (cod_magazin) 
REFERENCES magazin(cod_magazin)
);

CREATE TABLE produs
(cod_produs NUMBER(10) NOT NULL,
cod_depozit NUMBER(10),
cod_magazin NUMBER(10),
cod_lutier NUMBER(10),
cod_livrare NUMBER(10),
tip VARCHAR2(20),
nume VARCHAR2(20),
pret NUMBER(10),
greutate NUMBER(10),
material_predominant VARCHAR2(20),
producator VARCHAR2(20), 
CONSTRAINT pk_cod_produs PRIMARY KEY (cod_produs),
CONSTRAINT fk4_cod_depozit FOREIGN KEY (cod_depozit) 
REFERENCES depozit(cod_depozit),
CONSTRAINT fk4_cod_magazin FOREIGN KEY (cod_magazin) 
REFERENCES magazin(cod_magazin),
CONSTRAINT fk4_cod_lutier FOREIGN KEY (cod_lutier) 
REFERENCES lutier(cod_lutier),
CONSTRAINT fk4_cod_livrare FOREIGN KEY (cod_livrare) 
REFERENCES livrare(cod_livrare),
CONSTRAINT chk4_pret CHECK (pret >= 0),
CONSTRAINT chk4_greutate CHECK (greutate >= 0),
CONSTRAINT chk_singur_loc CHECK (DECODE(cod_depozit, NULL, 0, 1) +  DECODE(cod_magazin, NULL, 0, 1) + DECODE(cod_lutier, NULL, 0, 1) + DECODE(cod_livrare, NULL, 0, 1) <= 1)
);

ALTER TABLE produs
MODIFY cod_depozit DEFAULT NULL;
ALTER TABLE produs
MODIFY cod_magazin DEFAULT NULL;
ALTER TABLE produs
MODIFY cod_lutier DEFAULT NULL;
ALTER TABLE produs
MODIFY cod_livrare DEFAULT NULL;
ALTER TABLE livrare 
MODIFY status_livrare DEFAULT 'pe drum';

INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(1, 'Ilfov', 'Bucuresti', 'Strada Soarelui', 2, 6);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(2, 'Ilfov', 'Bucuresti', 'Calea Victoriei', 44, 15);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(3, 'Ilfov', 'Bucuresti', 'Strada Aurel Vlaicu', 63, 22);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(4, 'Ilfov', 'Bucuresti', 'Strada Dealului', 1, 19);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(5, 'Sibiu', 'Sibiu', 'Strada Ion Creanga', 23, 55);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(6, 'Sibiu', 'Sibiu', 'Strada Morii', 50, 14);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(7, 'Sibiu', 'Sibiu', 'Strada Rasaritului', 33, 33);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(8, 'Constanta', 'Constanta', 'Strada Brezei', 53, 84);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(9, 'Arges', 'Pitesti', 'Calea Mihai Viteazul', 52, 45);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(10, 'Dolj', 'Craiova', 'Aleea Dumbravei', 12, 14);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(11, 'Constanta', 'Constanta', 'Aleea Scoicilor', 1, 38);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(12, 'Arges', 'Topoloveni', 'S-duta Topolovenilor', 2, 4);
INSERT INTO locatie(cod_locatie, judet, oras, strada, numar_cladire, apartament) VALUES(13, 'Dolj', 'Craiova', 'Via Sarii', 1, 8);
--SELECT * FROM locatie;

INSERT INTO lutier(cod_lutier, nume, prenume, nr_tel, email, cod_locatie) VALUES(1, 'Ionescu', 'Gigel', '0732103441', 'igigleandr@yahoo.com', 1);
INSERT INTO lutier(cod_lutier, nume, prenume, nr_tel, email, cod_locatie) VALUES(6, 'Ionescu', 'Andrei', '0732214562', 'iandreii@yahoo.com', 1);
INSERT INTO lutier(cod_lutier, nume, prenume, nr_tel, email, cod_locatie) VALUES(2, 'Popescu', 'Marcel', '0730286884', 'tituspoma@yahoo.com', 2);
INSERT INTO lutier(cod_lutier, nume, prenume, nr_tel, email, cod_locatie) VALUES(3, 'Stroe', 'Andrei', '0734940265', 'stroe23@yahoo.com', 3);
INSERT INTO lutier(cod_lutier, nume, prenume, nr_tel, email, cod_locatie) VALUES(4, 'Stoici', 'Alexandru', '0735273197', 'stoalex2@yahoo.com', 4);
INSERT INTO lutier(cod_lutier, nume, prenume, nr_tel, email, cod_locatie) VALUES(5, 'Aioanei', 'Marius', '0730784294', 'aioaneity@google.com', 5);
--SELECT * FROM lutier;

INSERT INTO magazin(cod_magazin, nume, ora_deschidere, ora_inchidere, cod_locatie) VALUES(1, 'Geo+Alina Soarelui', 800, 1900, 1);
INSERT INTO magazin(cod_magazin, nume, ora_deschidere, ora_inchidere, cod_locatie) VALUES(2, 'Geo+Alina Morii', 800, 2000, 6);
INSERT INTO magazin(cod_magazin, nume, ora_deschidere, ora_inchidere, cod_locatie) VALUES(3, 'Geo+Alina Tanta', 1200, 1600, 8);
INSERT INTO magazin(cod_magazin, nume, ora_deschidere, ora_inchidere, cod_locatie) VALUES(4, 'Geo+Alina Viteazul', 800, 1900, 9);
INSERT INTO magazin(cod_magazin, nume, ora_deschidere, ora_inchidere, cod_locatie) VALUES(5, 'Geo+Alina Dumbravii', 700, 1800, 10);
--SELECT * FROM magazin;

INSERT INTO depozit(cod_depozit, nume, cod_locatie) VALUES (1, 'Ilfov Stocks', 1);
INSERT INTO depozit(cod_depozit, nume, cod_locatie) VALUES (2, 'Depozit Dealului', 4);
INSERT INTO depozit(cod_depozit, nume, cod_locatie) VALUES (3, 'ShineDepo', 7);
INSERT INTO depozit(cod_depozit, nume, cod_locatie) VALUES (4, 'Magazia lui Marin', 11);
INSERT INTO depozit(cod_depozit, nume, cod_locatie) VALUES (5, 'Depozit Blocului', 12);
INSERT INTO depozit(cod_depozit, nume, cod_locatie) VALUES (6, 'Depozit Blocului', 12);
INSERT INTO depozit(cod_depozit, nume, cod_locatie) VALUES (7, 'Depozit599', 13);
--SELECT* FROM depozit;

INSERT INTO functie(cod_functie, nume, salariu) VALUES (1, 'Director Stocuri', 4400);
INSERT INTO functie(cod_functie, nume, salariu) VALUES (2, 'Agent Vanzari', 2800);
INSERT INTO functie(cod_functie, nume, salariu) VALUES (3, 'Manager Vanzari', 3700);
INSERT INTO functie(cod_functie, nume, salariu) VALUES (4, 'Specialist Electr.', 3200);
INSERT INTO functie(cod_functie, nume, salariu) VALUES (5, 'Resp. Intretinere', 2300);
--SELECT * FROM functie;

--SELECT sysdate
--FROM dual;
INSERT INTO angajat(cod_angajat, cod_magazin, cod_functie, nume, prenume, data_angajare) VALUES (1, 1, 1, 'Pavelescu', 'Tiberiu', '22-04-2015');
INSERT INTO angajat(cod_angajat, cod_magazin, cod_functie, nume, prenume, data_angajare) VALUES (2, 2, 1, 'Raif', 'Andreea', '27-10-2020');
INSERT INTO angajat(cod_angajat, cod_magazin, cod_functie, nume, prenume, data_angajare) VALUES (3, 3, 1, 'Marcelo', 'Juan', '10-11-2018');
INSERT INTO angajat(cod_angajat, cod_magazin, cod_functie, nume, prenume, data_angajare) VALUES (4, 4, 1, 'Chitoi', 'Dragos', '16-04-2017');
INSERT INTO angajat(cod_angajat, cod_magazin, cod_functie, nume, prenume, data_angajare) VALUES (5, 5, 1, 'Pravale', 'Diana', '25-08-2012');
--SELECT * FROM angajat;

--SELECT * FROM locatie;
--SELECT* FROM depozit;
INSERT INTO vehicul(nr_inmat, categorie, ani_vechime, cod_depozit) VALUES ('DJ 131 ANA', 'BE',7,1);
INSERT INTO vehicul(nr_inmat, categorie, ani_vechime, cod_depozit) VALUES ('IF 50 PTR', 'C',4,3);
INSERT INTO vehicul(nr_inmat, categorie, ani_vechime, cod_depozit) VALUES ('IF 237 MSU', 'C', 1, 3);
INSERT INTO vehicul(nr_inmat, categorie, ani_vechime, cod_depozit) VALUES ('IF 09 TMY', 'C', 3, 4);
INSERT INTO vehicul(nr_inmat, categorie, ani_vechime, cod_depozit) VALUES ('SB 672 CMN', 'C', 5, 5);
INSERT INTO vehicul(nr_inmat, categorie, ani_vechime, cod_depozit) VALUES ('CT 116 MLG', 'BE', 4,6);
INSERT INTO vehicul(nr_inmat, categorie, ani_vechime, cod_depozit) VALUES ('AG 198 IBY', 'C', 3, 7);
--SELECT * FROM vehicul;

--DESC sofer;
INSERT INTO sofer(cod_sofer, nume, prenume, telefon, categorie_carnet, cod_depozit) VALUES (1, 'Pirvu', 'George', 0773721895, 'BE', 1);
INSERT INTO sofer(cod_sofer, nume, prenume, telefon, categorie_carnet, cod_depozit) VALUES (2, 'Lupu', 'Toma', 0743265830, 'C', 3);
INSERT INTO sofer(cod_sofer, nume, prenume, telefon, categorie_carnet, cod_depozit) VALUES (3, 'Lupu', 'Vasile', 0243195285, 'C', 3);
INSERT INTO sofer(cod_sofer, nume, prenume, telefon, categorie_carnet, cod_depozit) VALUES (4, 'Niculescu', 'Petra', 0218933829, 'C', 4);
INSERT INTO sofer(cod_sofer, nume, prenume, telefon, categorie_carnet, cod_depozit) VALUES (5, 'Amzu', 'Nicu', 0246942666, 'C', 5);
INSERT INTO sofer(cod_sofer, nume, prenume, telefon, categorie_carnet, cod_depozit) VALUES (6, 'Cismiu', 'Andra', 0770883829, 'BE', 6);
INSERT INTO sofer(cod_sofer, nume, prenume, telefon, categorie_carnet, cod_depozit) VALUES (7, 'Lica', 'Tanase', 0040012421691, 'C', 7);
--SELECT * FROM sofer;

--SELECT m.nume, m.cod_locatie, l.judet, l.oras, l.strada
--FROM magazin m, locatie l
--WHERE m.cod_locatie = l.cod_locatie
--UNION
--SELECT d.nume, d.cod_locatie, l.judet, l.oras, l.strada
--FROM depozit d, locatie l
--WHERE d.cod_locatie = l.cod_locatie
--UNION
--SELECT lut.nume, lut.cod_locatie, l.judet, l.oras, l.strada
--FROM lutier lut, locatie l
--WHERE lut.cod_locatie = l.cod_locatie
--ORDER BY judet;

INSERT INTO oprire(localitate) VALUES ('Bais'); 
INSERT INTO oprire(localitate) VALUES ('Slatina'); 
INSERT INTO oprire(localitate) VALUES ('Lehliu Gara'); 
INSERT INTO oprire(localitate) VALUES ('Fetesti'); 
INSERT INTO oprire(localitate) VALUES ('Medgidia');
INSERT INTO oprire(localitate) VALUES ('Ramnicu Valcea'); 
INSERT INTO oprire(localitate) VALUES ('Alexandria'); 
--SELECT * FROM oprire;
--
--SELECT s.cod_sofer, s.nume||' '||s.prenume AS "sofer", s.categorie_carnet, d.nume AS "depozit", d.cod_locatie, l.judet, l.oras, l.strada, v.nr_inmat
--FROM sofer s, depozit d, locatie l, vehicul v
--WHERE d.cod_locatie = l.cod_locatie
--AND s.cod_depozit = d.cod_depozit
--AND v.cod_depozit = d.cod_depozit;
--
--SELECT * 
--FROM depozit d, locatie l
--WHERE d.cod_locatie = l.cod_locatie;
--
--SELECT s.cod_sofer, s.nume||' '||s.prenume AS "sofer", d.cod_depozit, d.nume AS "depozit", d.cod_locatie, l.judet, l.oras, l.strada
--FROM sofer s, locatie l, depozit d
--WHERE s.cod_depozit = d.cod_depozit
--AND d.cod_locatie = l.cod_locatie;

INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(1, 'AG 198 IBY', 7, '25-05-2021', 700);
INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(2, 'AG 198 IBY', 7, '06-06-2021', 800);
INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(3, 'DJ 131 ANA', 1, '30-05-2021', 930);
INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(4, 'DJ 131 ANA', 1, '17-06-2021', NULL);
INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(5, 'AG 198 IBY', 7, '28-06-2021', NULL);
INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(6, 'DJ 131 ANA', 7, '19-05-2021', NULL);
INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(7, 'IF 09 TMY', 4, '28-06-2021', NULL);
INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(8, 'SB 672 CMN', 5, '01-07-2021', 1200);
INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(9, 'SB 672 CMN', 5, '01-07-2021', 1930);
INSERT INTO transport(cod_transport, nr_inmat, cod_sofer, data_plecare, ora_plecare) VALUES(10, 'CT 116 MLG', 6, '01-07-2021', 2230);
--SELECT * FROM transport;

INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (1, 'Bais', 'nu', NULL);
INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (1, 'Slatina', 'nu', 'aprox. 30 min.');
INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (2, 'Slatina', 'nu', 'aprox. 1h');
INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (3, 'Lehliu Gara', 'nu', NULL);
INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (3, 'Fetesti', 'nu', 'aprox. 30 min.');
INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (4, 'Lehliu Gara', 'nu', 'in jur de doua ore');
INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (4, 'Fetesti', 'nu', 'o ora');
INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (4, 'Medgidia', 'nu', '2 h - 2h 30');
INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (5, 'Ramnicu Valcea', 'nu',  '2 ore cel mult 3');
INSERT INTO oprire_transport(cod_transport, localitate, include_vama, durata) VALUES (6, 'Alexandria', 'nu', 'in jur de 1h, 1h30');
--SELECT * FROM oprire_transport;

--SELECT *
--FROM magazin m, locatie l
--WHERE m.cod_locatie = l.cod_locatie;
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(1, 1, 4);
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(2, 2, 4);
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(3, 3, 3);
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(4, 4, 3);
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(5, 5, 5);
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(6, 6, 1);
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(7, 7, 1);
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(8, 8, 2);
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(9, 9, 4);
INSERT INTO livrare(cod_livrare, cod_transport, cod_magazin) VALUES(10, 10, 1);
--SELECT * FROM livrare;

INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(1, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', NULL, 4, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(2, 'instrument', 'chitara el. fretless', 4400, NULL, 'artar', 'eBanmaltez', NULL, 4, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(3, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', NULL, 4, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(4, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', NULL, 4, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(5, 'instrument', 'chitara el. DMustang', 1600, NULL, 'stejar', 'DnM Raider', NULL, 4, NULL, NULL);    
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(6, 'instrument', 'chitara el. :Haver', 1600, 3.6, 'stejar', 'Hoover', NULL, 4, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(7, 'instrument', 'drum set full iron', 1600, 34, 'otel', 'Jerryhmann', NULL, 4, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(8, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', NULL, 3, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(9, 'instrument', 'drum set full iron', 1600, 34, 'otel', 'Jerryhmann', NULL, 2, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(10, 'instrument', 'chitara el. DMustang', 1600, NULL, 'stejar', 'DnM Raider', NULL, 2, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(11, 'instrument', 'chitara el. JumpingW', 2590, 3, 'frasin', 'Harvey Benington', NULL, 1, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(12, 'instrument', 'chitara el. JumpingW', 2590, 3, 'frasin', 'Harvey Benington', NULL, 1, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(13, 'audio', 'combo chitara 400V', 5000, 22, 'fier', 'GraySun', NULL, 3, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(14, 'inregistrare/audio', 'interfata RedPumpKin', 2800, 1, 'otel', 'Mathers', NULL, 2, NULL, NULL);

INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(15, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 4, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(16, 'instrument', 'chitara el. fretless', 4400, NULL, 'artar', 'eBanmaltez', 4, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(17, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 4, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(18, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 4, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(19, 'instrument', 'chitara el. DMustang', 1600, NULL, 'stejar', 'DnM Raider', 4, NULL, NULL, NULL);    
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(20, 'instrument', 'chitara el. :Haver', 1600, 3.6, 'stejar', 'Hoover', 4, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(30, 'instrument', 'drum set full iron', 1600, 34, 'otel', 'Jerryhmann', 4, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(31, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 3, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(32, 'instrument', 'drum set full iron', 1600, 34, 'otel', 'Jerryhmann', 2, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(33, 'instrument', 'chitara el. DMustang', 1600, NULL, 'stejar', 'DnM Raider', 2, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(34, 'instrument', 'chitara el. JumpingW', 2590, 3, 'frasin', 'Harvey Benington', 1, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(35, 'instrument', 'chitara el. JumpingW', 2590, 3, 'frasin', 'Harvey Benington', 1, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(36, 'audio', 'combo chitara 400V', 5000, 22, 'fier', 'GraySun', 3, NULL, NULL, NULL);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_depozit, cod_magazin, cod_lutier, cod_livrare) VALUES(37, 'inregistrare/audio', 'interfata RedPumpKin', 2800, 1, 'otel', 'Mathers', 2, NULL, NULL, NULL);

--SELECT * FROM lutier;
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(21, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 3);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(22, 'instrument', 'chitara el. fretless', 4400, NULL, 'artar', 'eBanmaltez', 2);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(23, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 1);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(24, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 5);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(25, 'instrument', 'chitara el. DMustang', 1600, NULL, 'stejar', 'DnM Raider', 1);    
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(26, 'instrument', 'chitara el. :Haver', 1600, 3.6, 'stejar', 'Hoover', 6);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(27, 'instrument', 'drum set full iron', 1600, 34, 'otel', 'Jerryhmann', 2);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(28, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 3);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(29, 'instrument', 'drum set full iron', 1600, 34, 'otel', 'Jerryhmann', 2);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(38, 'instrument', 'chitara el. DMustang', 1600, NULL, 'stejar', 'DnM Raider', 1);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(39, 'instrument', 'chitara el. JumpingW', 2590, 3, 'frasin', 'Harvey Benington', 2);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(40, 'instrument', 'chitara el. JumpingW', 2590, 3, 'frasin', 'Harvey Benington', 3);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(41, 'audio', 'combo chitara 400V', 5000, 22, 'fier', 'GraySun', 6);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_lutier) VALUES(42, 'inregistrare/audio', 'interfata RedPumpKin', 2800, 1, 'otel', 'Mathers', 6);

INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(43, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 3);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(44, 'instrument', 'chitara el. fretless', 4400, NULL, 'artar', 'eBanmaltez', 2);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(45, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 1);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(46, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 5);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(47, 'instrument', 'chitara el. DMustang', 1600, NULL, 'stejar', 'DnM Raider', 1);    
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(48, 'instrument', 'chitara el. :Haver', 1600, 3.6, 'stejar', 'Hoover', 6);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(49, 'instrument', 'drum set full iron', 1600, 34, 'otel', 'Jerryhmann', 2);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(50, 'instrument', 'bas acustic', 1600, NULL, 'stejar', 'eBanmaltez', 3);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(51, 'instrument', 'drum set full iron', 1600, 34, 'otel', 'Jerryhmann', 2);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(52, 'instrument', 'chitara el. DMustang', 1600, NULL, 'stejar', 'DnM Raider', 1);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(53, 'instrument', 'chitara el. JumpingW', 2590, 3, 'frasin', 'Harvey Benington', 2);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(54, 'instrument', 'chitara el. JumpingW', 2590, 3, 'frasin', 'Harvey Benington', 3);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(55, 'audio', 'combo chitara 400V', 5000, 22, 'fier', 'GraySun', 6);
INSERT INTO produs(cod_produs, tip, nume, pret, greutate, material_predominant, producator, cod_livrare) VALUES(56, 'inregistrare/audio', 'interfata RedPumpKin', 2800, 1, 'otel', 'Mathers', 6);

--SELECT COUNT(cod_produs)
--FROM produs;
--
--SELECT *
--FROM produs
--ORDER BY cod_produs;


COMMIT;


