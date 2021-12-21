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

SELECT cod_produs, NVL(cod_magazin, 0) cod_magazin, NVL(cod_livrare, 0) cod_livrare, nume 
FROM produs;
/
EXECUTE sosireLivrare(1);
/
SELECT cod_produs, NVL(cod_magazin, 0) cod_magazin, NVL(cod_livrare, 0) cod_livrare, nume 
FROM produs;

ROLLBACK;
/

-- ex 9: Creati o procedura care primeste un pararametru de intrare codul unui depozit si are doi parametrii de iesire 
-- pentru numarul de inmatriculare al unui vehicul, respectiv codul unui sofer, ale caror depozite coincid cu cel dat de codul din primul parametru. 
-- Soferul si vehicului indicati nu vor avea un transport care sa aiba livrari neincheiate
CREATE OR REPLACE PROCEDURE soferiVehiculeDisponibile (codDepozit IN NUMBER, codVehiculDisponibil OUT NUMBER, codSoferDisponibil OUT NUMBER) AS
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
    SELECT DISTINCT v.nr_inmat, s.cod_sofer
    INTO codVehiculDisponibil, codSoferDisponibil
    FROM depozit d, vehicul v, sofer s
    WHERE d.cod_depozit = codDepozit  -- depozitul cautat
    AND v.cod_depozit = d.cod_depozit  -- join intre vehicul si depozit 
    AND s.cod_depozit = d.cod_depozit  -- join intre sofer si depozit 
    AND v.nr_inmat NOT IN
        (SELECT v1.nr_inmat                       -- vehiculele cu transporturi neincheiate
        FROM vehicul v1, transport t
        WHERE t.nr_inmat = v.nr_inmat                           -- join intre transport si vehicul
        AND t.cod_transport IN transporturiNeincheiate -- mulimea transporturilior cu livrari neincheiate
        )
    AND s.cod_sofer NOT IN
        (SELECT s1.cod_sofer                      -- soferii cu transporturi neincheiate
        FROM sofer s1, transport t
        WHERE t.cod_sofer = s1.cod_sofer                      -- join intre transport si sofer
        AND t.cod_transport IN transporturiNeincheiate -- mulimea transporturilior cu livrari neincheiate
        );
    
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.PUT_LINE('soferiVehiculeDisponibile: Nu am gasit perechi disponibile de soferi si vehicule pentru depozitul cu codul ' || codDepozit);
            codVehiculDisponibil := -1;
            codSoferDisponibil:= -1;
            
        WHEN TOO_MANY_ROWS THEN
            DBMS_OUTPUT.PUT_LINE('soferiVehiculeDisponibile: S-au gasit mai multe perechi disponibile de soferi si vehicule pentru depozitull cu codul '|| codDepozit);
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
            INTO codVehiculDisponibil, codSoferDisponibil
            FROM depozit d, vehicul v, sofer s
            WHERE d.cod_depozit = codDepozit  -- depozitul cautat
            AND v.cod_depozit = d.cod_depozit  -- join intre vehicul si depozit 
            AND s.cod_depozit = d.cod_depozit  -- join intre sofer si depozit 
            AND v.nr_inmat NOT IN
                (SELECT v1.nr_inmat                       -- vehiculele cu transporturi neincheiate
                FROM vehicul v1, transport t
                WHERE t.nr_inmat = v.nr_inmat                           -- join intre transport si vehicul
                AND t.cod_transport IN transporturiNeincheiate -- mulimea transporturilior cu livrari neincheiate
                )
            AND s.cod_sofer NOT IN
                (SELECT s1.cod_sofer                      -- soferii cu transporturi neincheiate
                FROM sofer s1, transport t
                WHERE t.cod_sofer = s1.cod_sofer                      -- join intre transport si sofer
                AND t.cod_transport IN transporturiNeincheiate -- mulimea transporturilior cu livrari neincheiate
                );
            codVehiculDisponibil := perechiDisponibile(1).nrInmat;
            codSoferDisponibil:= perechiDisponibile(1).codSofer;
            
        WHEN OTHERS THEN
            DBMS_OUTPUT.PUT_LINE('soferiVehiculeDisponibile: Alta exceptie pentru depozitul cu codul ' || codDepozit);
END;
/

DECLARE 
    codV   vehicul.nr_inmat%TYPE;
    codS   sofer.cod_sofer%TYPE;
BEGIN
    soferiVehiculeDisponibile(1, codV, codS);

    DBMS_OUTPUT.PUT_LINE('cod vehicul disponibil: ' || codV);
    DBMS_OUTPUT.PUT_LINE('cod sofer disponibil: ' || codS);
END;
/

-- ex 6: Creati o procedura care primeste ca parametru de intrare un tabel indexat cu livrari si are parametrii de iesire tabele imbricate 
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
ROLLBACK;






