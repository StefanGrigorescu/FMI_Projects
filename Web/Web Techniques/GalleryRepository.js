const fs = require("fs");

// Functia de citire din fisierul db.json
module.exports.readJSONFileImages = () => {
    return JSON.parse(fs.readFileSync("db.json"))["images"]; //, "users", "current_user"];
}

function readJSONFileImages() {
    return JSON.parse(fs.readFileSync("db.json"))["images"]; //, "users", "current_user"];
}

module.exports.readJSONFileUsers = () => {
    return JSON.parse(fs.readFileSync("db.json"))["users"]; //, "users", "current_user"];
}

function readJSONFileUsers() {
    return JSON.parse(fs.readFileSync("db.json"))["users"]; //, "users", "current_user"];
}

module.exports.readJSONFileCrtUser = () => {
    return JSON.parse(fs.readFileSync("db.json"))["current_user"]; //, "users", "current_user"];
}

function readJSONFileCrtUser() {
    return JSON.parse(fs.readFileSync("db.json"))["current_user"]; //, "users", "current_user"];
}


// Functia de scriere in fisierul db.json pt imagini
module.exports.writeJSONFileImages = (content) => {
    let content_users = readJSONFileUsers();
    let content_crt_user = readJSONFileCrtUser();
    fs.writeFileSync(
        "db.json",
        JSON.stringify({ images: content, users: content_users, current_user: content_crt_user }, null, 4),
        "utf8",
        err => {
            if (err) {
                console.log(err);
            }
        }
    );
}

// Functia de scriere in fisierul db.json pt utilizatori
module.exports.writeJSONFileUsers = (content) => {
    let content_images = readJSONFileImages();
    let content_crt_user = readJSONFileCrtUser();
    fs.writeFileSync(
        "db.json",
        JSON.stringify({ images: content_images, users: content, current_user: content_crt_user }, null, 4),
        "utf8",
        err => {
            if (err) {
                console.log(err);
            }
        }
    );
}

// Functia de scriere in fisierul db.json pt utilizatorul_curent
module.exports.writeJSONFileCrtUser = (content) => {
    //console.log("sunt in galleryrepository, la modulul writeJSONFileCrtUser!");
    let content_images = readJSONFileImages();
    let content_users = readJSONFileUsers();
    fs.writeFileSync(
        "db.json",
        JSON.stringify({ images: content_images, users: content_users, current_user: content }, null, 4),
        "utf8",
        err => {
            if (err) {
                console.log(err);
            }
        }
    );
}