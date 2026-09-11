//const { stringify } = require("uuid");

let body = document.getElementsByTagName("body")[0];

window.onload = function pageLoad() {

    let utilizatorul_conectat = {};
    let galerie = document.getElementById("galerie");

    fetch('http://localhost:3000/images', {
        method: 'get'
    }).then((response) => {
        response.json().then((ImageList) => {
            console.log("Downloaded images");
            ImageList.forEach(dbImg => {
                let newGalerieItem = document.createElement("div");
                newGalerieItem.classList.add("galerie-item");
                let newGalerieSection = document.createElement("section");
                newGalerieSection.setAttribute("class", "galerie-s");
                newGalerieSection.attributes.display = "flex";
                newGalerieSection.attributes.justifyContent = "center";
                let photo = document.createElement("img");
                photo.src = dbImg.img;

                newGalerieSection.appendChild(photo);
                newGalerieItem.appendChild(newGalerieSection);
                galerie.appendChild(newGalerieItem);
            });

        });
    });

    fetch('http://localhost:3000/current_user', {
        method: 'get'
    }).then((response) => {
        response.json().then((crt_user) => {
            console.log("get: ", crt_user.name);
            utilizatorul_conectat = crt_user;
            if (!(crt_user.name == "offline")) {
                meniuModificare();
                meniuAdaugare();
            }

        })
    })

    function meniuModificare() {
        setTimeout(function() {
            let modificare = document.createElement("div");
            modificare.id = "Modificare";

            let descriere = document.createElement("div");
            descriere.classList.add("input-field");
            descriere.style.marginTop = "6%";
            descriere.setAttribute("opacity", "1");
            descriere.innerHTML = "Modificare imagine";
            modificare.appendChild(descriere);

            let cautare = document.createElement("input");
            cautare.classList.add("input-field");
            // cautare.placeholder = "id/nume/tag/autor";
            cautare.placeholder = "ID-ul pozei cautate";
            cautare.addEventListener('keypress', function(k) {
                if (k.key === 'Enter')
                    modificareConfirmata();
            });
            modificare.appendChild(cautare);

            let numeNou = document.createElement("input");
            numeNou.classList.add("input-field");
            numeNou.placeholder = "nume nou?";
            numeNou.addEventListener('keypress', function(k) {
                if (k.key === 'Enter')
                    modificareConfirmata();
            });
            modificare.appendChild(numeNou);

            let tagNou = document.createElement("input");
            tagNou.classList.add("input-field");
            tagNou.placeholder = "adaugi tag?";
            tagNou.addEventListener('keypress', function(k) {
                if (k.key === 'Enter')
                    modificareConfirmata();
            });
            modificare.appendChild(tagNou);

            function modificareConfirmata() {

                let matchedImage = {
                    "name": "",
                    "tags": []
                };
                let ok = false;
                fetch('http://localhost:3000/images', {
                    method: 'get'
                }).then((response) => {
                    response.json().then((ImageList) => {
                        console.log("Downloaded images");
                        //let matchedImages = [];
                        ImageList.forEach(dbImg => {
                            // if (dbImg.id === cautare.value || dbImg.name === cautare.value || dbImg.author === cautare.value)
                            //     matchedImages.push(dbImg);
                            // else {
                            //     for (let i = 0; i < dbImg.tags.length(); i++)
                            //         if (dbImg.tags[i] === cautare.value) {
                            //             matchedImages.push(dbImg);
                            //             break;
                            //         }
                            // }

                            if (dbImg.id === cautare.value) {
                                matchedImage = dbImg;
                                ok = true;
                            }
                        });
                        if (ok == false)
                            console.log("Poza cautata nu exista :O")
                        else if (numeNou.value || tagNou.value) {
                            console.log("Valid content");
                            if (numeNou.value)
                                matchedImage.name = numeNou.value;
                            if (tagNou.value) {
                                let newTags = tagNou.value.split(" ");
                                for (let i = 0; i < newTags.length; i++)
                                    matchedImage.tags.push(newTags[i]);
                            }

                            console.log("item ready to be modified");

                            fetch('http://localhost:3000/images/' + matchedImage.id, {
                                method: 'put',
                                headers: {
                                    'Content-Type': 'application/json'
                                },
                                body: JSON.stringify(matchedImage)
                            }).then(function(response) {
                                window.alert("Imaginea a fost modificata cu succes!");
                            });
                        }
                    });
                });
            }
            body.prepend(modificare);
        }, 1);
    }

    function meniuAdaugare() {
        setTimeout(function() {
            let adaugare = document.createElement("div");
            adaugare.id = "Adaugare";

            let descriere = document.createElement("div");
            descriere.classList.add("input-field");
            descriere.style.marginTop = "6%";
            descriere.setAttribute("opacity", "1");
            descriere.innerHTML = "Adaugare imagine";
            adaugare.appendChild(descriere);

            let cautare = document.createElement("input");
            cautare.setAttribute("type", "url");
            cautare.classList.add("input-field");
            cautare.placeholder = "URL";
            cautare.addEventListener('keypress', function(k) {
                if (k.key === 'Enter')
                    adaugareConfirmata();
            });
            adaugare.appendChild(cautare);

            let numele = document.createElement("input");
            numele.classList.add("input-field");
            numele.placeholder = "nume";
            numele.addEventListener('keypress', function(k) {
                if (k.key === 'Enter')
                    adaugareConfirmata();
            });
            adaugare.appendChild(numele);
            // numele.style.marginRight = "13%";

            let tags = document.createElement("input");
            tags.classList.add("input-field");
            tags.placeholder = "tags";
            tags.addEventListener('keypress', function(k) {
                if (k.key === 'Enter')
                    adaugareConfirmata();
            });
            adaugare.appendChild(tags);


            function adaugareConfirmata() {
                let ok = true;

                fetch('http://localhost:3000/images', {
                    method: 'get'
                }).then((response) => {
                    response.json().then((ImageList) => {
                        ImageList.forEach((dbImg) => {
                            if (dbImg.img == cautare.value)
                                ok = false;
                        });
                    });
                });

                delayedAdd();

                function delayedAdd() {
                    setTimeout(() => {
                        if (ok == false)
                            alert("Imaginea exista deja");
                        else if (cautare.value) {
                            let newImg = {
                                "tags": tags.value.split(" "),
                                "name": numele.value,
                                "img": cautare.value,
                                "author": utilizatorul_conectat.name
                            };
                            newImg.tags.push(newImg.author);
                            if (newImg.tags[0] == "")
                                newImg.tags.splice(0, 1);

                            fetch('http://localhost:3000/images', {
                                method: 'post',
                                headers: {
                                    'Content-Type': 'application/json'
                                },
                                body: JSON.stringify(newImg)
                            }).then(function(response) {
                                window.alert("Imagine adaugata cu succes!");
                                window.location.reload();
                            })
                        }
                    }, 5);
                }
            }
            body.prepend(adaugare);
        }, 1);
    }
}