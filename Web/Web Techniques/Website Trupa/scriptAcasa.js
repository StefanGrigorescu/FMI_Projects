//const { get } = require("./controller/GalleryController");

//const { response } = require("express");

//const { response } = require("express");

var body = document.getElementsByTagName("body")[0];

window.onload = function pageLoad() {

    fetch('http://localhost:3000/current_user', {
        method: 'get'
    }).then((response) => {
        response.json().then((crt_user) => {
            if (crt_user.name === "offline")
                creeazaButoaneLogare();
            else
                creeazaButonDelogare();
        })
    })

}

function creeazaButoaneLogare() {
    console.log("creeazaButoaneLogare");

    let logo_div = document.getElementById("login-id");

    var user = document.createElement("input");
    user.classList.add("caseta-log");
    user.id = "Utilizator";
    user.placeholder = "Utilizator";
    user.addEventListener('keypress', function(e) {
        if (e.key === 'Enter') {
            incercareLogare();
        }
    });
    logo_div.appendChild(user);


    var password = document.createElement("input");
    password.setAttribute("class", "caseta-log");
    password.setAttribute("type", "password");
    password.id = "Parola";
    password.placeholder = "Parola";
    password.addEventListener('keypress', function(e) {
        if (e.key === 'Enter') {
            incercareLogare();
        }
    });
    logo_div.appendChild(password);


    var buton_logare = document.createElement("button");
    buton_logare.classList.add("buton", "caseta-log");
    buton_logare.id = "Logare";
    buton_logare.innerText = "Logare";
    buton_logare.onclick = function() { incercareLogare() };
    logo_div.appendChild(buton_logare);


    var buton_inregistrare = document.createElement("button");
    buton_inregistrare.classList.add("buton", "caseta-log");
    buton_inregistrare.id = "Inregistrare";
    buton_inregistrare.innerText = "Inregistrare";
    buton_inregistrare.onclick = function() { Inregistrare() };
    logo_div.appendChild(buton_inregistrare);
}

function incercareLogare() {
    console.log("am apelat fct de incercareLogare");
    let id = document.getElementById("Utilizator").value;
    let parola = document.getElementById("Parola").value;


    fetch('http://localhost:3000/users', {
        method: 'get'
    }).then((response) => {
        response.json().then((users) => {
            console.log("Am intrat in getul din incercareLogare");
            let gasit = 0;

            users.forEach(checkIfExists);

            function checkIfExists(oneUser) {
                if (oneUser.name == id && gasit == 0) {
                    gasit = 1;

                    if (oneUser.password != parola) {
                        console.log(parola);
                        window.alert("Parola nu e tocmai corecta, mai incearca!");
                    } else {
                        Logare([oneUser.name, oneUser.password]);
                    }
                }
            }

            if (gasit == 0)
                window.alert("Utilizatorul acesta nu exista. Creati cont?");
        })
    })

}

function Logare(user) {
    console.log("am intrat in fct de Logare");

    var crt_user = {
        "name": user[0],
        "password": user[1]
    }

    fetch('http://localhost:3000/current_user', {
        method: 'post',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(crt_user)
    }).then(function(response) {
        window.alert("Buna siua, " + crt_user.name);
    })

    stergeButoaneLogare();

    creeazaButonDelogare();
}

function stergeButoaneLogare() {
    let logo_div = document.getElementById("login-id");

    var user_box = document.getElementById("Utilizator");
    logo_div.removeChild(user_box);

    var password_box = document.getElementById("Parola");
    logo_div.removeChild(password_box);

    var buton_logare = document.getElementById("Logare");
    logo_div.removeChild(buton_logare);

    var buton_inregistrare = document.getElementById("Inregistrare");
    logo_div.removeChild(buton_inregistrare);
}

function creeazaButonDelogare() {
    let logo_div = document.getElementById("login-id");

    var buton_delogare = document.createElement("button");
    buton_delogare.classList.add("buton", "caseta-log");
    buton_delogare.id = "Delogam";
    buton_delogare.innerHTML = "Delogare";
    buton_delogare.setAttribute('onclick', 'Delogare()');
    logo_div.appendChild(buton_delogare);
}


function Delogare() {
    console.log("Am intrat in Delogare");

    // StergeButonDelogare();
    // creeazaButonDelogare();

    fetch("http://localhost:3000/current_user", {
        method: "delete",
    }).then((response) => {
        window.location.reload();
        console.log("V-atz deconectat!");
    })
}

// function StergeButonDelogare() {
//     console.log("Am intrat in StergeButonDelogare");

//     let logo_div = document.getElementById("login-id");
//     var buton_delogare = document.getElementById("Delogam");

//     // console.log(buton_delogare.id);

//     logo_div.removeChild(buton_delogare);
// }

function Inregistrare() {
    console.log("In inregistrare");

    let inreg = document.createElement("div");
    inreg.setAttribute("class", "inregistrare");

    var inchide = document.createElement("button");
    inchide.classList.add("buton", "caseta-log");
    inchide.style.width = "5%";
    inchide.style.marginLeft = "93%";
    inchide.style.textAlign = "center";
    inchide.id = "Inchide";
    inchide.textContent = " X ";
    inchide.onclick = function() { body.removeChild(inreg); };
    inreg.appendChild(inchide);

    var user = document.createElement("input");
    user.classList.add("caseta-log");
    user.id = "Utilizator";
    user.placeholder = "Utilizator";
    inreg.appendChild(user);

    var password = document.createElement("input");
    password.setAttribute("class", "caseta-log");
    password.setAttribute("type", "password");
    password.id = "Parola";
    password.placeholder = "Parola";
    inreg.appendChild(password);

    var cpassword = document.createElement("input");
    cpassword.setAttribute("class", "caseta-log");
    cpassword.setAttribute("type", "password");
    cpassword.id = "cParola";
    cpassword.placeholder = "Confirma parola";
    inreg.appendChild(cpassword);

    var email = document.createElement("input");
    email.setAttribute("class", "caseta-log");
    email.id = "Email";
    email.placeholder = "Email";
    inreg.appendChild(email);

    var telefon = document.createElement("input");
    telefon.setAttribute("class", "caseta-log");
    telefon.id = "Telefon";
    telefon.placeholder = "Telefon";
    inreg.appendChild(telefon);

    var gata = document.createElement("button");
    gata.classList.add("buton", "caseta-log");
    gata.id = "Gata";
    gata.textContent = " Gata! ";
    gata.onclick = function() {
        let id = user.value;
        let parola = password.value;
        let cparola = cpassword.value;
        let mail = email.value;
        let tel = telefon.value;

        let ok = true;

        if (!(parola === cparola)) {
            window.alert("Parolele nu corespund!");
            ok = false;
        } else if (!id) {
            window.alert("Introduceti un nume de utilizator!");
            ok = false;
        } else if (!parola) {
            window.alert("Introduceti o parola!");
            ok = false;
        } else if (!cparola) {
            window.alert("Confirmati parola!");
            ok = false;
        } else {

            fetch('http://localhost:3000/users', {
                method: 'get'
            }).then((response) => {
                response.json().then((users) => {

                    let gasit = 0;

                    users.forEach(checkIfExists);

                    function checkIfExists(oneUser) {
                        if (oneUser.name == id)
                            gasit = 1;
                    }

                    if (gasit == 1) {
                        window.alert("Utilizatorul deja exista! Alege alt nume!");
                        ok = false;
                    }
                })
            })

        }

        if (ok == true) {
            let newUser = {
                "name": id,
                "password": parola,
                "email": mail,
                "phone": tel
            }

            fetch('http://localhost:3000/users', {
                method: 'post',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(newUser)
            }).then(function(response) {
                window.alert("Utilizator adaugat!");
                body.removeChild(inreg);
            })
        }
    };
    inreg.appendChild(gata);

    // "name": "marcel",
    // "password": "strongpa55",
    // "email": "marceldestroyer@email.com",
    // "phone": "0773420069",
    // "img": "placeholder"

    body.appendChild(inreg);


}