-- ex 7: Creati o procedurã care primeste ca parametru de intrare codul unei livrãri si îi seteazã status_livrare ca 'sosita', 
-- iar pentru fiecare produs care are cod_livrare egal cu parametrul dat, seteazã cod_livrare cu NULL si cod_magazin 
-- cu codul magazinului ce corespunde livrarii in cauza. Se va folosi un cursor pentru a retine produsele ce trebuiesc actualizate.
CREATE OR REPLACE PROCEDURE sosireLivrare(codLivrare IN NUMBER) AS
    TYPE tabelCoduriProduse IS TABLE OF NUMBER;
    
    coduriProduse   tabelCoduriProduse :=tabelCoduriProduse();
    codMagazin    NUMBER;
    codProdusCrt   NUMBER;
    
    CURSOR c IS 
        SELECT cod_produs
        BULK COLLECT INTO coduriProduse
        FROM produs
        WHERE cod_livrare = codLivrare;
BEGIN
    IF (codLivrare IS NULL)
        THEN
            DBMS_OUTPUT.PUT_LINE('sosireLivrare: Parametrul dat nu poate fi NULL!');
            RETURN;
    END IF;

    SELECT cod_magazin
    INTO codMagazin
    FROM livrare
    WHERE cod_livrare = codLivrare;

    UPDATE livrare
    SET status_livrare = 'sosita'
    WHERE cod_livrare = codLivrare;

    OPEN c;
    LOOP
        FETCH c INTO codProdusCrt;
        EXIT WHEN c%NOTFOUND;
        
        UPDATE produs
        SET cod_livrare = NULL, cod_magazin = codMagazin
        WHERE cod_produs = codProdusCrt;
        DBMS_OUTPUT.PUT_LINE('Produsul cu codul ' || codProdusCrt || ' a fost livrat la magazinul cu codul ' || codMagazin );
    END LOOP;
    CLOSE c;

    DBMS_OUTPUT.PUT_LINE('Livrarea cu codul ' || codLivrare || ' s-a incheiat');
    
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.PUT_LINE('sosireLivrare: Nu exista o livrare cu codul '|| codLivrare || '!');   
END;
/

SELECT cod_produs produs, NVL(cod_magazin, 0) magazin, NVL(cod_livrare, 0) livrare, nume FROM produs;
SELECT * FROM livrare;

EXECUTE sosireLivrare(1);
/

SELECT cod_produs produs, NVL(cod_magazin, 0) magazin, NVL(cod_livrare, 0) livrare, nume FROM produs;
SELECT * FROM LIVRARE;

ROLLBACK;
/

-- ex 9: Creati o procedura care primeste un pararametru de intrare pentru codul unui produs 
-- si are un parametru de iesire pentru numele depozitului in care se afla acesta
-- si doi parametri de iesire pentru numarul de inmatriculare al unui vehicul, respectiv codul unui sofer,
-- ale caror depozite coincid cu cel al depozitului in care se afla produsul.
-- Vehiculul si soferul indicati nu vor avea un transport care sa aiba livrari neincheiate.
-- Daca produsul dat nu exista sau nu are un cod_depozit nenul sau daca nu se gaseste o pereche disponibila de vehicul si sofer,
-- parametrii de iesire vor fi setati toti cu -1.
CREATE OR REPLACE PROCEDURE soferiVehiculeDisponibile (codProdus IN produs.cod_produs%TYPE, numeDepozit OUT depozit.nume%TYPE, codVehiculDisponibil OUT vehicul.nr_inmat%TYPE, codSoferDisponibil OUT sofer.cod_sofer%TYPE) AS
    TYPE perecheVehiculSofer IS RECORD
    (nrInmat   vehicul.nr_inmat%TYPE,
    codSofer   sofer.cod_sofer%TYPE
    );
    TYPE tabelPerecheVehiculSofer IS TABLE OF perecheVehiculSofer;
    
    perechiDisponibile   tabelPerecheVehiculSofer := tabelPerecheVehiculSofer();
    
BEGIN
    WITH transporturiNeincheiate AS 
        (SELECT t.cod_transport codT
        FROM transport t
        WHERE 
            (SELECT COUNT(l.cod_livrare)
            FROM livrare l
            WHERE l.cod_transport = t.cod_transport            -- join intre livrare si transport
            AND LOWER(l.status_livrare) NOT LIKE 'sosita'  -- livrare neincheiata 
            ) > 0
        )
    SELECT DISTINCT d.nume, v.nr_inmat, s.cod_sofer
    INTO numeDepozit, codVehiculDisponibil, codSoferDisponibil
    FROM produs p, depozit d, vehicul v, sofer s
    WHERE p.cod_produs = codProdus    -- produsul cautat 
    AND d.cod_depozit = p.cod_depozit  -- join intre depozit si produs
    AND v.cod_depozit = d.cod_depozit  -- join intre vehicul si depozit 
    AND s.cod_depozit = d.cod_depozit  -- join intre sofer si depozit 
    AND v.nr_inmat NOT IN
        (SELECT v1.nr_inmat                       -- vehiculele cu transporturi neincheiate
        FROM vehicul v1, transport t
        WHERE t.nr_inmat = v.nr_inmat                           -- join intre transport si vehicul
        AND t.cod_transport IN 
            (SELECT codT                              -- multimea transporturilior cu livrari neincheiate
            FROM transporturiNeincheiate 
            )
        )
    AND s.cod_sofer NOT IN
        (SELECT s1.cod_sofer                      -- soferii cu transporturi neincheiate
        FROM sofer s1, transport t
        WHERE t.cod_sofer = s1.cod_sofer                      -- join intre transport si sofer
        AND t.cod_transport IN 
            (SELECT codT                              -- multimea transporturilior cu livrari neincheiate
            FROM transporturiNeincheiate 
            )
        );
    
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.PUT_LINE('soferiVehiculeDisponibile: Nu am gasit perechi disponibile de vehicule si soferi pentru produsul cu codul ' || codProdus || ' sau cod_depozit este NULL sau produsul dat nu exista.' );
            numeDepozit := -1;
            codVehiculDisponibil := -1;
            codSoferDisponibil:= -1;
            
        WHEN TOO_MANY_ROWS THEN
            DBMS_OUTPUT.PUT_LINE('soferiVehiculeDisponibile: S-au gasit mai multe perechi disponibile de vehicule si soferi pentru  produsul cu codul ' || codProdus);
            WITH transporturiNeincheiate AS 
            (SELECT t.cod_transport codT
            FROM transport t
            WHERE 
                (SELECT COUNT(l.cod_livrare)
                FROM livrare l
                WHERE l.cod_transport = t.cod_transport            -- join intre livrare si transport
                AND LOWER(l.status_livrare) NOT LIKE 'sosita'  -- livrare neincheiata 
                ) > 0
            )
            SELECT DISTINCT v.nr_inmat, s.cod_sofer
            BULK COLLECT INTO perechiDisponibile
            FROM produs p, depozit d, vehicul v, sofer s
            WHERE p.cod_produs = codProdus    -- produsul cautat 
            AND d.cod_depozit = p.cod_depozit  -- join intre depozit si produs
            AND v.cod_depozit = d.cod_depozit  -- join intre vehicul si depozit 
            AND s.cod_depozit = d.cod_depozit  -- join intre sofer si depozit 
            AND v.nr_inmat NOT IN
                (SELECT v1.nr_inmat                       -- vehiculele cu transporturi neincheiate
                FROM vehicul v1, transport t
                WHERE t.nr_inmat = v.nr_inmat                           -- join intre transport si vehicul
                AND t.cod_transport IN 
                    (SELECT codT                              -- multimea transporturilior cu livrari neincheiate
                    FROM transporturiNeincheiate 
                    )
                )
            AND s.cod_sofer NOT IN
                (SELECT s1.cod_sofer                      -- soferii cu transporturi neincheiate
                FROM sofer s1, transport t
                WHERE t.cod_sofer = s1.cod_sofer                      -- join intre transport si sofer
                AND t.cod_transport IN 
                    (SELECT codT                              -- multimea transporturilior cu livrari neincheiate
                    FROM transporturiNeincheiate 
                    )
                );
            codVehiculDisponibil := perechiDisponibile(1).nrInmat;
            codSoferDisponibil:= perechiDisponibile(1).codSofer;
            
            SELECT d.nume
            INTO numeDepozit
            FROM depozit d, sofer s
            WHERE s.cod_sofer = codSoferDisponibil -- soferul gasit
            AND d.cod_depozit = s.cod_depozit;         -- join intre depozit si sofer
            
        WHEN OTHERS THEN
            DBMS_OUTPUT.PUT_LINE('soferiVehiculeDisponibile: Alta exceptie pentru  produsul cu codul ' || codProdus);
END;
/

SELECT cod_produs produs, cod_depozit depozit FROM produs;

DECLARE 
    TYPE coduriProduse IS VARRAY(5) OF produs.cod_produs%TYPE;

    varrayCoduriProduse   coduriProduse:=coduriProduse();
    codV   vehicul.nr_inmat%TYPE;
    codS   sofer.cod_sofer%TYPE;
    numeD   depozit.nume%TYPE;
BEGIN
    FOR i IN 1..5 LOOP
        varrayCoduriProduse.extend();
    END LOOP;
    varrayCoduriProduse(1) := 14;
    varrayCoduriProduse(2) := 15;
    varrayCoduriProduse(3) := 35;
    varrayCoduriProduse(4) := 36;
    varrayCoduriProduse(5) := 37;

    FOR i IN varrayCoduriProduse.FIRST..varrayCoduriProduse.LAST LOOP
        soferiVehiculeDisponibile(varrayCoduriProduse(i), numeD, codV, codS);
    
        IF codS >= 0 
            THEN
                DBMS_OUTPUT.PUT_LINE('Pentru produsul cu codul ' || varrayCoduriProduse(i) || ': ');
                DBMS_OUTPUT.PUT_LINE('   nume depozit: ' || numeD);
                DBMS_OUTPUT.PUT_LINE('   nr inmat vehicul disponibil: ' || codV);
                DBMS_OUTPUT.PUT_LINE('   cod sofer disponibil: ' || codS);
                DBMS_OUTPUT.NEW_LINE();
            ELSE
                DBMS_OUTPUT.NEW_LINE();
        END IF;
    END LOOP;
END;
/

-- ex 6: Creati o procedura care primeste ca parametru de intrare un tabel indexat cu livrari si are parametrii de iesire doua tabele imbricate 
-- in care vor fi introduse codurile vehiculelor, respectiv soferilor care se ocupa de transporturile incheiate (ale caror ultime livrari au sosit acum). 
-- Modificati si livrarile de actualizat folosind procedura sosireLivrare (de la ex 7).
CREATE OR REPLACE TYPE varrayCoduriLivrari IS VARRAY(100) OF NUMBER(10);
/
CREATE OR REPLACE TYPE tblCoduriVehicule IS TABLE OF VARCHAR2(20);
/
CREATE OR REPLACE TYPE tblCoduriSoferi IS TABLE OF VARCHAR2(10);
/
CREATE OR REPLACE PROCEDURE elibereazaVehiculeSoferi(nrLivrari IN NUMBER, coduriLivrari IN varrayCoduriLivrari, coduriVehicule OUT tblCoduriVehicule, coduriSoferi OUT tblCoduriSoferi) AS    
    codTransport   NUMBER;
BEGIN
    FOR i IN coduriLivrari.FIRST..coduriLivrari.LAST LOOP
        sosireLivrare(coduriLivrari(i));          -- se incheie livrarea crt
        
        SELECT cod_transport
        INTO codTransport
        FROM livrare
        WHERE cod_livrare = coduriLivrari(i);
        
        SELECT DISTINCT v.nr_inmat
        BULK COLLECT INTO coduriVehicule
        FROM vehicul v, transport t
        WHERE t.cod_transport = codTransport -- transportul ce corespunde livrarii crt
        AND t.nr_inmat = v.nr_inmat                  -- join intre transport si vehicul
        AND t.cod_transport NOT IN                  -- transport care sa nu fie in mulimea transporturilior cu livrari neincheiate
            (SELECT t1.cod_transport 
            FROM transport t1
            WHERE 
                (SELECT COUNT(l.cod_livrare)
                FROM livrare l
                WHERE l.cod_transport = t.cod_transport            -- join intre livrare si transport
                AND LOWER(l.status_livrare) NOT LIKE 'sosita'  -- livrare neincheiata 
                ) > 0
            );
            
        SELECT DISTINCT s.cod_sofer
        BULK COLLECT INTO coduriSoferi
        FROM sofer s, transport t
        WHERE t.cod_transport = codTransport -- transportul ce corespunde livrarii crt
        AND t.cod_sofer = s.cod_sofer               -- join intre transport si sofer
        AND t.cod_transport NOT IN                  -- transport care sa nu fie in mulimea transporturilior cu livrari neincheiate
            (SELECT t1.cod_transport 
            FROM transport t1
            WHERE 
                (SELECT COUNT(l.cod_livrare)
                FROM livrare l
                WHERE l.cod_transport = t.cod_transport            -- join intre livrare si transport
                AND LOWER(l.status_livrare) NOT LIKE 'sosita'  -- livrare neincheiata 
                ) > 0
            );
    END LOOP;    
END;
/

SELECT * FROM livrare;
SELECT * FROM transport;
SELECT cod_produs produs, cod_livrare livrare, cod_magazin magazin FROM produs;

DECLARE
    coduriLivrari  varrayCoduriLivrari := varrayCoduriLivrari();
    coduriVehicule   tblCoduriVehicule := tblCoduriVehicule();
    coduriSoferi   tblCoduriSoferi := tblCoduriSoferi();

BEGIN
    SELECT cod_livrare
    BULK COLLECT INTO coduriLivrari
    FROM livrare
    WHERE cod_livrare IN (1,2,3, 5,6, 7);
    
    elibereazaVehiculeSoferi(4, coduriLivrari, coduriVehicule, coduriSoferi);
    
    DBMS_OUTPUT.PUT('Nr inmatriculare vehicule eliberate: ');
    FOR i IN coduriVehicule.FIRST..coduriVehicule.LAST LOOP
        DBMS_OUTPUT.PUT(coduriVehicule(i) || '   ');
    END LOOP;
    DBMS_OUTPUT.NEW_LINE;
    
    DBMS_OUTPUT.PUT('Coduri soferi eliberati: ');
    FOR i IN coduriSoferi.FIRST..coduriSoferi.LAST LOOP
        DBMS_OUTPUT.PUT(coduriSoferi(i) || '   ');
    END LOOP;
    DBMS_OUTPUT.NEW_LINE;
END;
/

SELECT * FROM livrare;
SELECT cod_produs produs, cod_livrare livrare, cod_magazin magazin FROM produs;

ROLLBACK;

-- ex 8: Creati o functie ce primeste ca parametru de intrare codul unui produs si returneaza codul unui lutier 
-- aflat in acelasi judet in care se gaseste magazinul in care sta produsul dat. Daca produsul nu se afla intr-un magazin 
-- sau daca in judetul respectiv nu este niciun lutier, functia va returna -1.
CREATE OR REPLACE FUNCTION lutierLocal(codProdus IN NUMBER)
RETURN lutier.cod_lutier%TYPE AS 
    TYPE coduriLutieri   IS TABLE OF lutier.cod_lutier%TYPE;

    judetLocatie    locatie.judet%TYPE;
    codLutier         lutier.cod_lutier%TYPE;
    tblCoduriLutieri   coduriLutieri := coduriLutieri();
    
BEGIN
    SELECT l.judet
    INTO judetLocatie
    FROM produs p, magazin m, locatie l 
    WHERE p.cod_produs = codProdus          -- alegem produsul cu codul dat
    AND m.cod_magazin = p.cod_magazin   -- join intre magazin si produs 
    AND l.cod_locatie = m.cod_locatie;        -- join intre locatie si magazin 
    
    SELECT lut.cod_lutier
    INTO codLutier
    FROM lutier lut, locatie loc
    WHERE loc.judet = judetLocatie             -- judetul gasit
    AND lut.cod_locatie = loc.cod_locatie;  -- join intre lutier si locatie 
    
    RETURN codLutier;
    
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.PUT_LINE('lutierLocal: Atentie! In produs, cod_magazin nu trebuie sa fie null. Posibil sa nu fie un lutier in judetul magazinului.');
            RETURN -1;
        WHEN TOO_MANY_ROWS THEN
            DBMS_OUTPUT.PUT_LINE('lutierLocal: Mai multi lutieri sunt la dispozitie. A fost returnat primul gasit.');
            SELECT lut.cod_lutier
            BULK COLLECT INTO tblCoduriLutieri
            FROM lutier lut, locatie loc
            WHERE loc.judet = judetLocatie             -- judetul gasit
            AND lut.cod_locatie = loc.cod_locatie;  -- join intre lutier si locatie 
            
            codLutier := tblCoduriLutieri(1);
            RETURN codLutier;
END lutierLocal;
/

SELECT p.cod_produs produs, m.cod_magazin magazin, l.judet judet
FROM produs p, magazin m, locatie l
WHERE p.cod_magazin IS NOT NULL
AND m.cod_magazin = p.cod_magazin
AND l.cod_locatie = m.cod_locatie;

SELECT lut.cod_lutier lutier, loc.judet judet
FROM lutier lut, locatie loc
WHERE loc.cod_locatie = lut.cod_locatie;

DECLARE
    TYPE coduriLutieri IS VARRAY(4) OF  lutier.cod_lutier%TYPE;
    varrayCoduriLutieri   coduriLutieri:=coduriLutieri();

    codLutierLocal   lutier.cod_lutier%TYPE;
BEGIN
    varrayCoduriLutieri.extend();
    varrayCoduriLutieri.extend();
    varrayCoduriLutieri.extend();
    varrayCoduriLutieri.extend();
    varrayCoduriLutieri(1):= 10;   -- un lutier in judet
    varrayCoduriLutieri(2):= 11;   -- mai multi lutieri in judet (too many rows)
    varrayCoduriLutieri(3):= 13;   -- niciun lutier in judet (no data found) 
    varrayCoduriLutieri(4):= 15;   -- cod magazin null (no data found)
    
    FOR i IN varrayCoduriLutieri.FIRST..varrayCoduriLutieri.LAST LOOP
        codLutierLocal := lutierLocal(varrayCoduriLutieri(i));  -- vezi pt codurile de produse: 10, 11, 13
        
        IF codLutierLocal > -1
            THEN
                DBMS_OUTPUT.PUT_LINE('Pentru produsul cu codul ' || varrayCoduriLutieri(i) || ', codul unui lutier din judet: ' || codLutierLocal);
            ELSE 
                DBMS_OUTPUT.PUT_LINE('Pentru produsul cu codul ' || varrayCoduriLutieri(i) || ', eroare intampinata! Vedeti detalii mai sus');
        END IF;
        DBMS_OUTPUT.NEW_LINE();
    END LOOP;
END;
/


-- ex 10: Creati un trigger de tip LMD la nivel de comanda cu executare dupa, pentru update-uri pe coloana status_livrare a tabelului livrare.
-- Triggerul va calcula pretul maxim al unui produs aflat intr-un magazin si va actualiza
-- preturile tuturor produselor aflate in magazine si avand acel pret maxim, prin reducere cu 5%.
CREATE OR REPLACE TRIGGER ofertaProdus
    AFTER UPDATE OF status_livrare ON livrare
DECLARE 
    TYPE coduriProduse IS TABLE OF produs.cod_produs%TYPE;
    
    tblCoduriProduse   coduriProduse := coduriProduse();
    pretMaxim   produs.pret%TYPE;
    pretCrt   produs.pret%TYPE;
    codMagazinCrt   produs.cod_magazin%TYPE;
BEGIN
    SELECT MAX(pret)
    INTO pretMaxim
    FROM produs
    WHERE cod_magazin IS NOT NULL;
    
    SELECT cod_produs
    BULK COLLECT INTO tblCoduriProduse
    FROM produs
    WHERE pret = pretMaxim
    AND cod_magazin IS NOT NULL;
    
    FOR i IN tblCoduriProduse.FIRST..tblCoduriProduse.LAST LOOP
        SELECT pret, cod_magazin
        INTO pretCrt, codMagazinCrt
        FROM produs
        WHERE cod_produs = tblCoduriProduse(i);
        
        DBMS_OUTPUT.PUT_LINE('Produsul cu codul ' || tblCoduriProduse(i) || ', aflat in magazinul cu codul ' || codMagazinCrt || ', este redus prin oferta de la ' || pretCrt || ' lei la ' || 95/100*pretCrt || ' lei!');
    END LOOP;
    
    FORALL i IN tblCoduriProduse.FIRST..tblCoduriProduse.LAST
        UPDATE produs
            SET pret = 95/100 * pret
            WHERE cod_produs = tblCoduriProduse(i);
    
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.PUT_LINE('ofertaProdus: Eroare no_data_found; este posibil sa nu existe niciun produs in magazine.');
END;
/

SELECT cod_produs produs, NVL(cod_magazin, 0) magazin, pret, NVL(cod_livrare, 0) livrare, nume FROM produs;
SELECT * FROM livrare;

EXECUTE sosireLivrare(2);
/

SELECT cod_produs produs, NVL(cod_magazin, 0) magazin, pret, NVL(cod_livrare, 0) livrare, nume FROM produs;
SELECT * FROM LIVRARE;

ROLLBACK;

DROP TRIGGER ofertaProdus;


-- ex 11: Creati un trigger de tip LMD la nivel de linie cu executare inainte, pentru update-uri pe coloana status_livrare a tabelului livrare.
-- Triggerul va adauga intr-un depozit anume (adica va insera in tabelul produs) cate un produs nou pentru fiecare produs din livrarea ce se va actualiza.
-- Produsele noi vor prelua de la cele livrate: tip, nume, pret, greutate, material_predominant, producator
-- Depozitul se va alege in felul urmator: prioritatea cea mai mare o au depozitele din acelasi judet cu magazinul la care ajunge livrarea,
-- apoi mai au prioritate depozitele cu un numar cat mai mic de produse (calculat inainte de actualizarile triggerului).
SELECT m.cod_magazin, loc.judet
FROM magazin m, locatie loc
WHERE m.cod_locatie = loc.cod_locatie;

SELECT d.cod_depozit, loc.judet
FROM depozit d, locatie loc
WHERE d.cod_locatie = loc.cod_locatie;

CREATE SEQUENCE seqCoduriProduse
    START WITH 57
    INCREMENT BY 1
    MINVALUE 1
    MAXVALUE 999999999
    CYCLE;

CREATE OR REPLACE TRIGGER completareStocDepozit 
    BEFORE UPDATE OF status_livrare ON livrare
    FOR EACH ROW
DECLARE
    TYPE produseNoi IS TABLE OF produs%ROWTYPE;
    TYPE coduriDepozite IS TABLE OF depozit.cod_depozit%TYPE;
    
    tblProduseNoi   produseNoi := produseNoi();
    tblCoduriDepozite   coduriDepozite := coduriDepozite();
    codLivrare     livrare.cod_livrare%TYPE;
    codDepozit   depozit.cod_depozit%TYPE;
    codMagazin  livrare.cod_magazin%TYPE;
BEGIN
    codLivrare := :OLD.cod_livrare;
    codMagazin := :OLD.cod_magazin;
    
    -- retinem depozitele din acelasi judet cu magazinul la care ajunge livrarea curenta:
    SELECT d.cod_depozit
    BULK COLLECT INTO tblCoduriDepozite
    FROM locatie loc, depozit d
    WHERE loc.judet =                               -- locatiile aflate in judetul curent
        (SELECT loc.judet judetulCrt                         
        FROM magazin m, locatie loc
        WHERE m.cod_magazin = codMagazin   -- magazinul la care ajunge livrarea curenta
        AND loc.cod_locatie = m.cod_locatie)    -- join intre locatie si magazin (adica locatia magazinului)
    AND d.cod_locatie = loc.cod_locatie   -- join intre depozit si locatie (adica depozitul de la locatie)
    ORDER BY                                             -- depozitele cu mai putine produse au prioritate
        (SELECT COUNT(p.cod_produs)
        FROM produs p
        WHERE p.cod_depozit = d.cod_depozit  -- join intre produs si depozit
        ); 

    -- daca nu am gasit un depozit in acelasi judet, consideram toate depozitele
    IF tblCoduriDepozite.COUNT = 0
        THEN
            SELECT d.cod_depozit
            BULK COLLECT INTO tblCoduriDepozite
            FROM depozit d
            ORDER BY                                      -- depozitele cu mai putine produse au prioritate
            (SELECT COUNT(p.cod_produs)
            FROM produs p
            WHERE p.cod_depozit = d.cod_depozit  -- join intre produs si depozit
            );
    END IF;

    codDepozit := tblCoduriDepozite(1);  -- retinem doar primul depozit (cu prioritatea cea mai mare) din cate avem la dispozitie
    
    SELECT seqCoduriProduse.NEXTVAL, codDepozit, NULL, NULL, NULL, p.tip, p.nume, p.pret, p.greutate, p.material_predominant, p.producator
    BULK COLLECT INTO tblProduseNoi
    FROM produs p
    WHERE p.cod_livrare = codLivrare;
    
    FOR i IN tblProduseNoi.FIRST..tblProduseNoi.LAST LOOP
        -- inseram in produs o linie cu toate datele din tblProduseNoi(i):
        INSERT INTO produs VALUES(tblProduseNoi(i).cod_produs, tblProduseNoi(i).cod_depozit, tblProduseNoi(i).cod_magazin, tblProduseNoi(i).cod_lutier, tblProduseNoi(i).cod_livrare, 
        tblProduseNoi(i).tip, tblProduseNoi(i).nume, tblProduseNoi(i).pret, tblProduseNoi(i).greutate, tblProduseNoi(i).material_predominant, tblProduseNoi(i).producator);

        DBMS_OUTPUT.PUT_LINE('In depozitul cu codul ' || tblProduseNoi(i).cod_depozit || ' a fost adaugat produsul cu codul ' || tblProduseNoi(i).cod_produs);
    END LOOP;
    
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.PUT_LINE('completareStocDepozit: Eroare no_data_found; este posibil sa nu fie produse in livrarea respectiva');
END;
/

SELECT *
FROM produs;

EXECUTE sosireLivrare(1);
/

SELECT *
FROM produs;

ROLLBACK;

DROP TRIGGER completareStocDepozit;
DROP SEQUENCE seqCoduriProduse;


-- ex 12: Creati un trigger la nivel de schema pentru drop, cu executare inainte. 
-- Triggerul verifica daca obiectul de sters este tabelul produs si apoi, in functie de valoarea unui boolean 
-- definit intr-un pachet separat pentru operarea permisiunilor, triggerul fie va permite stergerea tabelului,
-- fie va ridica o eroare difinita de utilizator.
CREATE OR REPLACE PACKAGE pkgPermisiuni AS
    permisiuneDropProdus   BOOLEAN := TRUE;
    PROCEDURE setPermisiuneDropProdus (val IN BOOLEAN);
END pkgPermisiuni;
/

CREATE OR REPLACE PACKAGE BODY pkgPermisiuni AS
    PROCEDURE setPermisiuneDropProdus (val IN BOOLEAN) IS
    BEGIN
        permisiuneDropProdus := val;
        IF permisiuneDropProdus
            THEN
                DBMS_OUTPUT.PUT_LINE('permisiuneDropProdus a fost setata la TRUE');
            ELSE 
                DBMS_OUTPUT.PUT_LINE('permisiuneDropProdus a fost setata la FALSE');
        END IF;    
    END;
END pkgPermisiuni;
/

CREATE OR REPLACE TRIGGER verificareStergere 
    BEFORE DROP ON SCHEMA
DECLARE    
    permisiunea   BOOLEAN;
BEGIN
    IF LOWER(SYS.DICTIONARY_OBJ_NAME) LIKE 'produs'
        THEN
            permisiunea := pkgPermisiuni.permisiuneDropProdus;
            
            IF permisiunea = FALSE
                THEN 
                    DBMS_OUTPUT.PUT_LINE('verificareStergere: (' || SYSDATE || ') Nu a fost permisa stergerea tabelului produs.');
                    RAISE_APPLICATION_ERROR(-20998, 'verificareStergere: Nu a fost permisa stergerea tabelului produs! Setati permisiuneDropProdus la TRUE si incercati iar!');
                ELSE
                    DBMS_OUTPUT.PUT_LINE('verificareStergere: (' || SYSDATE || ') Tabelulul produs a fost sters cu succes.');
            END IF;
    END IF;
END;
/

EXECUTE pkgPermisiuni.setPermisiuneDropProdus(FALSE);
/

DROP TABLE produs;

EXECUTE pkgPermisiuni.setPermisiuneDropProdus(TRUE);
/

DROP TABLE produs;

ROLLBACK;

DROP TRIGGER verificareStergere;


-- ex 13: Creati un pachet musicShopExtensii ce va cuprinde toate tipurile, functiile si procedurile create pana acum pentru operarea bazei de date MusicShp.
CREATE OR REPLACE PACKAGE musicShopExtensii AS
    -- de la ex 7:
    PROCEDURE sosireLivrare(codLivrare IN NUMBER);
    -- de la ex 9:      
    PROCEDURE soferiVehiculeDisponibile (codProdus IN produs.cod_produs%TYPE, numeDepozit OUT depozit.nume%TYPE, codVehiculDisponibil OUT vehicul.nr_inmat%TYPE, codSoferDisponibil OUT sofer.cod_sofer%TYPE);
    -- de la ex 6:
    TYPE varrayCoduriLivrari IS VARRAY(100) OF NUMBER(10);
    TYPE tblCoduriVehicule IS TABLE OF VARCHAR2(20);
    TYPE tblCoduriSoferi IS TABLE OF VARCHAR2(10);
    PROCEDURE elibereazaVehiculeSoferi(nrLivrari IN NUMBER, coduriLivrari IN musicShopExtensii.varrayCoduriLivrari, coduriVehicule OUT musicShopExtensii.tblCoduriVehicule, 
        coduriSoferi OUT musicShopExtensii.tblCoduriSoferi);     
    -- de la ex 8:
    FUNCTION lutierLocal(codProdus IN NUMBER)
        RETURN lutier.cod_lutier%TYPE;
END musicShopExtensii;
/

CREATE OR REPLACE PACKAGE BODY musicShopExtensii AS
    -- de la ex 7:
    PROCEDURE sosireLivrare(codLivrare IN NUMBER) IS
        TYPE tabelCoduriProduse IS TABLE OF NUMBER;
        
        coduriProduse   tabelCoduriProduse :=tabelCoduriProduse();
        codMagazin    NUMBER;
        codProdusCrt   NUMBER;
        
        CURSOR c IS 
            SELECT cod_produs
            BULK COLLECT INTO coduriProduse
            FROM produs
            WHERE cod_livrare = codLivrare;
    BEGIN
        IF (codLivrare IS NULL)
            THEN
                DBMS_OUTPUT.PUT_LINE('sosireLivrare: Parametrul dat nu poate fi NULL!');
                RETURN;
        END IF;
    
        SELECT cod_magazin
        INTO codMagazin
        FROM livrare
        WHERE cod_livrare = codLivrare;
    
        UPDATE livrare
        SET status_livrare = 'sosita'
        WHERE cod_livrare = codLivrare;
    
        OPEN c;
        LOOP
            FETCH c INTO codProdusCrt;
            EXIT WHEN c%NOTFOUND;
            
            UPDATE produs
            SET cod_livrare = NULL, cod_magazin = codMagazin
            WHERE cod_produs = codProdusCrt;
            DBMS_OUTPUT.PUT_LINE('Produsul cu codul ' || codProdusCrt || ' a fost livrat la magazinul cu codul ' || codMagazin );
        END LOOP;
        CLOSE c;
    
        DBMS_OUTPUT.PUT_LINE('Livrarea cu codul ' || codLivrare || ' s-a incheiat');
        
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                DBMS_OUTPUT.PUT_LINE('sosireLivrare: Nu exista o livrare cu codul '|| codLivrare || '!');   
    END;
    
    
    -- de la ex 9:
    PROCEDURE soferiVehiculeDisponibile (codProdus IN produs.cod_produs%TYPE, numeDepozit OUT depozit.nume%TYPE, codVehiculDisponibil OUT vehicul.nr_inmat%TYPE, codSoferDisponibil OUT sofer.cod_sofer%TYPE) AS
        TYPE perecheVehiculSofer IS RECORD
        (nrInmat   vehicul.nr_inmat%TYPE,
        codSofer   sofer.cod_sofer%TYPE
        );
        TYPE tabelPerecheVehiculSofer IS TABLE OF perecheVehiculSofer;
        
        perechiDisponibile   tabelPerecheVehiculSofer := tabelPerecheVehiculSofer();
        
    BEGIN
        WITH transporturiNeincheiate AS 
            (SELECT t.cod_transport codT
            FROM transport t
            WHERE 
                (SELECT COUNT(l.cod_livrare)
                FROM livrare l
                WHERE l.cod_transport = t.cod_transport            -- join intre livrare si transport
                AND LOWER(l.status_livrare) NOT LIKE 'sosita'  -- livrare neincheiata 
                ) > 0
            )
        SELECT DISTINCT d.nume, v.nr_inmat, s.cod_sofer
        INTO numeDepozit, codVehiculDisponibil, codSoferDisponibil
        FROM produs p, depozit d, vehicul v, sofer s
        WHERE p.cod_produs = codProdus    -- produsul cautat 
        AND d.cod_depozit = p.cod_depozit  -- join intre depozit si produs
        AND v.cod_depozit = d.cod_depozit  -- join intre vehicul si depozit 
        AND s.cod_depozit = d.cod_depozit  -- join intre sofer si depozit 
        AND v.nr_inmat NOT IN
            (SELECT v1.nr_inmat                       -- vehiculele cu transporturi neincheiate
            FROM vehicul v1, transport t
            WHERE t.nr_inmat = v.nr_inmat                           -- join intre transport si vehicul
            AND t.cod_transport IN 
                (SELECT codT                              -- multimea transporturilior cu livrari neincheiate
                FROM transporturiNeincheiate 
                )
            )
        AND s.cod_sofer NOT IN
            (SELECT s1.cod_sofer                      -- soferii cu transporturi neincheiate
            FROM sofer s1, transport t
            WHERE t.cod_sofer = s1.cod_sofer                      -- join intre transport si sofer
            AND t.cod_transport IN 
                (SELECT codT                              -- multimea transporturilior cu livrari neincheiate
                FROM transporturiNeincheiate 
                )
            );
        
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                DBMS_OUTPUT.PUT_LINE('soferiVehiculeDisponibile: Nu am gasit perechi disponibile de vehicule si soferi pentru produsul cu codul ' || codProdus || ' sau cod_depozit este NULL sau produsul dat nu exista.' );
                numeDepozit := -1;
                codVehiculDisponibil := -1;
                codSoferDisponibil:= -1;
                
            WHEN TOO_MANY_ROWS THEN
                DBMS_OUTPUT.PUT_LINE('soferiVehiculeDisponibile: S-au gasit mai multe perechi disponibile de vehicule si soferi pentru  produsul cu codul ' || codProdus);
                WITH transporturiNeincheiate AS 
                (SELECT t.cod_transport codT
                FROM transport t
                WHERE 
                    (SELECT COUNT(l.cod_livrare)
                    FROM livrare l
                    WHERE l.cod_transport = t.cod_transport            -- join intre livrare si transport
                    AND LOWER(l.status_livrare) NOT LIKE 'sosita'  -- livrare neincheiata 
                    ) > 0
                )
                SELECT DISTINCT v.nr_inmat, s.cod_sofer
                BULK COLLECT INTO perechiDisponibile
                FROM produs p, depozit d, vehicul v, sofer s
                WHERE p.cod_produs = codProdus    -- produsul cautat 
                AND d.cod_depozit = p.cod_depozit  -- join intre depozit si produs
                AND v.cod_depozit = d.cod_depozit  -- join intre vehicul si depozit 
                AND s.cod_depozit = d.cod_depozit  -- join intre sofer si depozit 
                AND v.nr_inmat NOT IN
                    (SELECT v1.nr_inmat                       -- vehiculele cu transporturi neincheiate
                    FROM vehicul v1, transport t
                    WHERE t.nr_inmat = v.nr_inmat                           -- join intre transport si vehicul
                    AND t.cod_transport IN 
                        (SELECT codT                              -- multimea transporturilior cu livrari neincheiate
                        FROM transporturiNeincheiate 
                        )
                    )
                AND s.cod_sofer NOT IN
                    (SELECT s1.cod_sofer                      -- soferii cu transporturi neincheiate
                    FROM sofer s1, transport t
                    WHERE t.cod_sofer = s1.cod_sofer                      -- join intre transport si sofer
                    AND t.cod_transport IN 
                        (SELECT codT                              -- multimea transporturilior cu livrari neincheiate
                        FROM transporturiNeincheiate 
                        )
                    );
                codVehiculDisponibil := perechiDisponibile(1).nrInmat;
                codSoferDisponibil:= perechiDisponibile(1).codSofer;
                
                SELECT d.nume
                INTO numeDepozit
                FROM depozit d, sofer s
                WHERE s.cod_sofer = codSoferDisponibil -- soferul gasit
                AND d.cod_depozit = s.cod_depozit;         -- join intre depozit si sofer
                
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('soferiVehiculeDisponibile: Alta exceptie pentru  produsul cu codul ' || codProdus);
    END;
    
    
    -- de la ex 6:     
    PROCEDURE elibereazaVehiculeSoferi(nrLivrari IN NUMBER, coduriLivrari IN musicShopExtensii.varrayCoduriLivrari, coduriVehicule OUT musicShopExtensii.tblCoduriVehicule, 
    coduriSoferi OUT musicShopExtensii.tblCoduriSoferi) IS    
        codTransport   NUMBER;
    BEGIN
        FOR i IN coduriLivrari.FIRST..coduriLivrari.LAST LOOP
            musicShopExtensii.sosireLivrare(coduriLivrari(i));          -- se incheie livrarea crt
            
            SELECT cod_transport
            INTO codTransport
            FROM livrare
            WHERE cod_livrare = coduriLivrari(i);
            
            SELECT DISTINCT v.nr_inmat
            BULK COLLECT INTO coduriVehicule
            FROM vehicul v, transport t
            WHERE t.cod_transport = codTransport -- transportul ce corespunde livrarii crt
            AND t.nr_inmat = v.nr_inmat                  -- join intre transport si vehicul
            AND t.cod_transport NOT IN                  -- transport care sa nu fie in mulimea transporturilior cu livrari neincheiate
                (SELECT t1.cod_transport 
                FROM transport t1
                WHERE 
                    (SELECT COUNT(l.cod_livrare)
                    FROM livrare l
                    WHERE l.cod_transport = t.cod_transport            -- join intre livrare si transport
                    AND LOWER(l.status_livrare) NOT LIKE 'sosita'  -- livrare neincheiata 
                    ) > 0
                );
                
            SELECT DISTINCT s.cod_sofer
            BULK COLLECT INTO coduriSoferi
            FROM sofer s, transport t
            WHERE t.cod_transport = codTransport -- transportul ce corespunde livrarii crt
            AND t.cod_sofer = s.cod_sofer               -- join intre transport si sofer
            AND t.cod_transport NOT IN                  -- transport care sa nu fie in mulimea transporturilior cu livrari neincheiate
                (SELECT t1.cod_transport 
                FROM transport t1
                WHERE 
                    (SELECT COUNT(l.cod_livrare)
                    FROM livrare l
                    WHERE l.cod_transport = t.cod_transport            -- join intre livrare si transport
                    AND LOWER(l.status_livrare) NOT LIKE 'sosita'  -- livrare neincheiata 
                    ) > 0
                );
        END LOOP;    
    END;
    
    
    -- de la ex 8: 
    FUNCTION lutierLocal(codProdus IN NUMBER)
    RETURN lutier.cod_lutier%TYPE IS lutierul lutier.cod_lutier%TYPE; 
        TYPE coduriLutieri   IS TABLE OF lutier.cod_lutier%TYPE;
    
        judetLocatie    locatie.judet%TYPE;
        codLutier         lutier.cod_lutier%TYPE;
        tblCoduriLutieri   coduriLutieri := coduriLutieri();
    BEGIN
        SELECT l.judet
        INTO judetLocatie
        FROM produs p, magazin m, locatie l 
        WHERE p.cod_produs = codProdus          -- alegem produsul cu codul dat
        AND m.cod_magazin = p.cod_magazin   -- join intre magazin si produs 
        AND l.cod_locatie = m.cod_locatie;        -- join intre locatie si magazin 
        
        SELECT lut.cod_lutier
        INTO codLutier
        FROM lutier lut, locatie loc
        WHERE loc.judet = judetLocatie             -- judetul gasit
        AND lut.cod_locatie = loc.cod_locatie;  -- join intre lutier si locatie 
        
        RETURN codLutier;
        
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                DBMS_OUTPUT.PUT_LINE('lutierLocal: Atentie! In produs, cod_magazin nu trebuie sa fie null. Posibil sa nu fie un lutier in judetul magazinului.');
                RETURN -1;
            WHEN TOO_MANY_ROWS THEN
                DBMS_OUTPUT.PUT_LINE('lutierLocal: Mai multi lutieri sunt la dispozitie. A fost returnat primul gasit.');
                SELECT lut.cod_lutier
                BULK COLLECT INTO tblCoduriLutieri
                FROM lutier lut, locatie loc
                WHERE loc.judet = judetLocatie             -- judetul gasit
                AND lut.cod_locatie = loc.cod_locatie;  -- join intre lutier si locatie 
                
                codLutier := tblCoduriLutieri(1);
                RETURN codLutier;
    END lutierLocal;  
END musicShopExtensii;
/


SELECT * FROM livrare;
SELECT * FROM transport;
SELECT cod_produs produs, cod_livrare livrare, cod_magazin magazin FROM produs;

DECLARE
    coduriLivrari       musicShopExtensii.varrayCoduriLivrari := musicShopExtensii.varrayCoduriLivrari();
    coduriVehicule   musicShopExtensii.tblCoduriVehicule   := musicShopExtensii.tblCoduriVehicule();
    coduriSoferi        musicShopExtensii.tblCoduriSoferi        := musicShopExtensii.tblCoduriSoferi();

BEGIN
    SELECT cod_livrare
    BULK COLLECT INTO coduriLivrari
    FROM livrare
    WHERE cod_livrare IN (1,2,3, 5,6, 7);
    
    musicShopExtensii.elibereazaVehiculeSoferi(4, coduriLivrari, coduriVehicule, coduriSoferi);
    
    DBMS_OUTPUT.PUT('Nr inmatriculare vehicule eliberate: ');
    FOR i IN coduriVehicule.FIRST..coduriVehicule.LAST LOOP
        DBMS_OUTPUT.PUT(coduriVehicule(i) || '   ');
    END LOOP;
    DBMS_OUTPUT.NEW_LINE;
    
    DBMS_OUTPUT.PUT('Coduri soferi eliberati: ');
    FOR i IN coduriSoferi.FIRST..coduriSoferi.LAST LOOP
        DBMS_OUTPUT.PUT(coduriSoferi(i) || '   ');
    END LOOP;
    DBMS_OUTPUT.NEW_LINE;
END;
/

SELECT * FROM livrare;
SELECT cod_produs produs, cod_livrare livrare, cod_magazin magazin FROM produs;

ROLLBACK;

DROP PACKAGE musicShopExtensii;


