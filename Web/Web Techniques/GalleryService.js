const uuid = require("uuid");

let GalleryRepository = require("./GalleryRepository");

module.exports.getAllImages = () => {
    const ImagesList = GalleryRepository.readJSONFileImages();
    return ImagesList;
}

module.exports.getAllUsers = () => {
    const UsersList = GalleryRepository.readJSONFileUsers();
    return UsersList;
}

module.exports.addImage = (newImage) => {
    //aduc un array cu toate imaginile deja existente in db.json
    const ImageList = GalleryRepository.readJSONFileImages();
    //creez o imagine noua folosind datele venite din frontend pe body
    newImage.id = uuid.v4.apply();

    //introducem imaginea noua in lista tuturor imaginilor
    ImageList.push(newImage);
    //suprascriu db.json cu noua lista de imagini
    GalleryRepository.writeJSONFileImages(ImageList);

    return newImage;
}

module.exports.addUser = (newUser) => {
    //aduc un array cu toti utilizatorii deja existenti in db.json
    const UserList = GalleryRepository.readJSONFileUsers();
    //creez o imagine noua folosind datele venite din frontend pe body
    newUser.id = uuid.v4.apply();

    //introducem imaginea noua in lista tuturor utilizatorilor
    UserList.push(newUser)
        //suprascriu db.json cu noua lista de utilizatori
    GalleryRepository.writeJSONFileUsers(UserList);

    return newUser;
}

module.exports.addCrtUser = (crt_user) => {
    //console.log("sunt in galleryservice, la modulul addCrtUser!");
    GalleryRepository.writeJSONFileCrtUser(crt_user);
    return crt_user;
}

module.exports.getCurrent_user = () => {
    const crt_user = GalleryRepository.readJSONFileCrtUser();
    if (crt_user)
        return crt_user;
    return null;
}

module.exports.getImageById = (id) => {
    const ImageList = GalleryRepository.readJSONFileImages();
    let foundImage = null;
    ImageList.forEach(image => {
        if (image.id === id) {
            foundImage = image;
        }
    })
    return foundImage;
}

module.exports.getUserById = (id) => {
    const UserList = GalleryRepository.readJSONFileUsers();
    let foundUser = null;
    UserList.forEach(user => {
        if (user.id === id) {
            foundUser = user;
        }
    })
    return foundUser;
}

module.exports.getImageByName = (name) => {
    const ImageList = GalleryRepository.readJSONFileImages();
    let foundImage = null;
    ImageList.forEach(image => {
        if (image.name === name) {
            foundImage = image;
        }
    })
    return foundImage;
}

module.exports.getUserByName = (name) => {
    const UserList = GalleryRepository.readJSONFileUsers();
    let foundUser = null;
    UserList.forEach(user => {
        if (user.name === name) {
            foundUser = user;
        }
    })
    return foundUser;
}

module.exports.getImagesByTag = (tag) => {
    const ImageList = GalleryRepository.readJSONFileImages();
    if (tag == "" || tag == null || tag == undefined) {
        return ImageList;
    }
    //console.log(tag);
    let ImagesToReturn = [];
    for (let i = 0; i < ImageList.length; i++) {
        if (ImageList[i].tags.includes(tag)) {
            ImagesToReturn.push(ImageList[i]);
        }
    }
    return ImagesToReturn;
}

module.exports.updateImage = (id, image) => {
    const ImageList = GalleryRepository.readJSONFileImages();
    let updateImage = null;
    for (let i = 0; i < ImageList.length; i++) {
        if (ImageList[i].id === id) {
            //in cazul in care imaginea este gasita, ii actualizam datele
            if (image.name) {
                ImageList[i].name = image.name;
            }

            if (image.img) {
                ImageList[i].img = image.img;
            }

            ImageList[i].tags.splice(0, ImageList[i].tags.length); // pe img au fost deja incarcate tagurile vechi, 
            for (let j = 0; j < image.tags.length; j++) // le stergem din ImgList[i].tags sa nu fie dublate
                ImageList[i].tags.push(image.tags[j]);

            if (image.author) {
                ImageList[i].author = image.author;
            }

            updateImage = ImageList[i];
            break;
        }
    }
    //rescriem db.json cu datele imaginii actualizate;
    GalleryRepository.writeJSONFileImages(ImageList);
    return updateImage;
}

module.exports.updateUser = (id, user) => {
    const UserList = GalleryRepository.readJSONFileUsers();
    let updateUser = null;
    for (let i = 0; i < UserList.length; i++) {
        if (UserList[i].id === id) {
            //in cazul in care utilizatorul este gasit, ii actualizam datele
            if (user.password) {
                UserList[i].password = user.password;
            }

            if (user.email) {
                UserList[i].email = user.email;
            }

            if (user.phone) {
                UserList[i].phone = user.phone;
            }

            if (user.img) {
                UserList[i].img = user.img;
            }
            updateUser = UserList[i];
            break;
        }
    }
    //rescriem db.json cu datele utilizatorului actualizat;
    GalleryRepository.writeJSONFileUsers(UserList);
    return updateUser;
}

module.exports.deleteImage = (id) => {
    const ImageList = GalleryRepository.readJSONFileImages();
    let checkIfImageExists = false;
    for (let i = 0; i < ImageList.length; i++) {
        if (ImageList[i].id === id) {
            checkIfImageExists = true;
            //sterg imaginea de pe pozitia i
            // splice sterge de la indexul i atatea elemente cate indica al doilea argument
            ImageList.splice(i, 1);
            break;
        }
    }
    GalleryRepository.writeJSONFileImages(ImageList);
    return checkIfImageExists;
}

module.exports.deleteUser = (id) => {
    const UserList = GalleryRepository.readJSONFileUsers();
    let checkIfUserExists = false;
    for (let i = 0; i < UserList.length; i++) {
        if (UserList[i].id === id) {
            checkIfUserExists = true;
            //sterg utilizatorul de pe pozitia i
            // splice sterge de la indexul i atatea elemente cate indica al doilea argument
            UserList.splice(i, 1);
            break;
        }
    }
    GalleryRepository.writeJSONFileUsers(UserList);
    return checkIfUserExists;
}

module.exports.deleteCrtUser = () => {
    var crt_user = {
        name: "offline"
    }
    GalleryRepository.writeJSONFileCrtUser(crt_user);
    return crt_user;
}

function checkLoginInput() {
    //logat = 1;

    const UserList = GalleryRepository.readJSONFileUsers();

    let utilizator_input = getElemenById("Utilizator").value;
    let parola_input = getElementById("Parola").value;

    if (getUserById(utilizator_input) == null) {
        window.alert("Utilizatorul acesta nu exista. Creati cont?");
        return null;

    } else if (parola_input != getUserById(utilizator_input).password) {
        window.alert("Parola nu e tocmai corecta, mai incearca!");
        return null;
    } else
        return getUserById(utilizaor_input);
}

module.exports.logIn = () => {
    let crt_user = checkLoginInput();
    GalleryRepository.writeJSONFileCrtUser(crt_user);
    //console.log("Modulul logIn() apelat");
}

module.exports.logOut = () => {
    GalleryRepository.writeJSONFileCrtUser(null);
}