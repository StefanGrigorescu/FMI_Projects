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

    UPDATE livrare
    SET status_livrare = 'sosita'
    WHERE cod_livrare = codLivrare;
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


