module com.eastwoodjtb {
    requires javafx.controls;
    requires javafx.fxml;
    requires com.fasterxml.jackson.core;
    requires com.fasterxml.jackson.databind;
    requires java.net.http;
    requires com.google.gson;
    requires lombok;

    opens com.eastwoodjtb to javafx.fxml;
    exports com.eastwoodjtb;
}