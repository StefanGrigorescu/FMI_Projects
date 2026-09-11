var express = require('express');
var router = express.Router();

let GalleryService = require("./GalleryService");

// Read All Images
router.get("/images", (req, res) => {
    let ImagesList = GalleryService.getAllImages()
    if (ImagesList != undefined && ImagesList.length != 0) {
        res.status(200).send(ImagesList);
    } else {
        res.status(404).send("No images found!");
    }
});

// Read All Users
router.get("/users", (req, res) => {
    //console.log("am incercat si noi...");
    let UsersList = GalleryService.getAllUsers();
    if (UsersList != undefined && UsersList.length != 0) {
        res.status(200).send(UsersList);
    } else {
        res.status(404).send("No users found, bro...!");
    }
});

//Read crt_user
router.get("/current_user", (req, res) => {
    let crt_user = GalleryService.getCurrent_user();
    if (crt_user)
        res.status(200).send(crt_user);
    else
        res.status(200).send("offline user");
});

// Create Image
router.post("/images", (req, res) => {
    let newImage = GalleryService.addImage(req.body);
    //trimit raspuns catre frontend ca totul a fost ok
    res.status(200).send(newImage);
});

// Create User
router.post("/users", (req, res) => {
    let newUser = GalleryService.addUser(req.body);
    //trimit raspuns catre frontend ca totul a fost ok
    res.status(200).send(newUser);
});

// Create current_user
router.post("/current_user", (req, res) => {
    //console.log("sunt in gallerycontroller, la postul pt current_user!");
    let crt_user = GalleryService.addCrtUser(req.body);
    res.status(200).send(crt_user);
});

// Read One Image
router.get("/images/:id", (req, res) => {
    let id = req.params.id;
    let image = GalleryService.getImageById(id)
    if (image === null) {
        res.status(404).send("No image found!");
    } else {
        res.status(200).send(image);
    }
});

// Read One User
router.get("/users/:id", (req, res) => {
    let id = req.params.id;
    let user = GalleryService.getUserById(id);
    if (user === null) {
        res.status(404).send("No user found!");
    } else {
        res.status(200).send(user);
    }
});

router.get("/images/name/:name", (req, res) => {
    let name = req.params.name;
    let image = GalleryService.getImageByName(name);
    if (image === null) {
        res.status(404).send("No image found!");
    } else {
        res.status(200).send(image);
    }
});

router.get("/users/name/:name", (req, res) => {
    let name = req.params.name;
    let user = GalleryService.getUserByName(name);
    if (user === null) {
        res.status(404).send("No user found!");
    } else {
        res.status(200).send(user);
    }
});

router.get("/images/filter/property", (req, res) => {
    let tag = req.query.tag;
    let images = GalleryService.getImagessByTag(tag);
    if (images.length) {
        res.status(200).send(images);
    } else {
        res.status(404).send("Images not found!");
    }
});

// Update Image
router.put("/images/:id", (req, res) => {
    //iau id-ul imaginii pe care verau sa o actualizez
    let id = req.params.id;
    let image = GalleryService.updateImage(id, req.body);
    if (image !== null) {
        res.status(200).send(image);
    } else {
        //trimitem exception 404 not found
        res.status(404).send("No image found!");
    }
});

// Update User
router.put("/users/:id", (req, res) => {
    //iau id-ul utilizatorului pe care verau sa il actualizez
    let id = req.params.id;
    let user = GalleryService.updateUser(id, req.body);
    if (user !== null) {
        res.status(200).send(user);
    } else {
        //trimitem exception 404 not found
        res.status(404).send("No user found!");
    }
});

// Delete Image
router.delete("/images/:id", (req, res) => {
    let id = req.params.id;
    let deleteFlag = GalleryService.deleteImage(id);
    if (deleteFlag === true) {
        res.status(200).send("Image deleted!");
    } else {
        res.status(404).send("Image not found!");
    }
});

// Delete User
router.delete("/users/:id", (req, res) => {
    let id = req.params.id;
    let deleteFlag = GalleryService.deleteUser(id);
    if (deleteFlag === true) {
        res.status(200).send("User deleted!");
    } else {
        res.status(404).send("User not found!");
    }
});

// Delete CrtUser
router.delete("/current_user", (req, res) => {
    GalleryService.deleteCrtUser();
    res.status(200).send("V-ati deconectat!");
});

module.exports = router;