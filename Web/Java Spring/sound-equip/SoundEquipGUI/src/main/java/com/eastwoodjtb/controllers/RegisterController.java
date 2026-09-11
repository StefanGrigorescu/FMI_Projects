package com.eastwoodjtb.controllers;


import com.eastwoodjtb.fetches.FetchRegisterTask;
import com.eastwoodjtb.utils.DbUtil;
import com.eastwoodjtb.utils.StageTitleEnum;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;

import java.net.URL;
import java.util.Objects;
import java.util.ResourceBundle;

public class RegisterController implements Initializable {
    @FXML
    private TextField tf_username_register;
    @FXML
    private PasswordField pf_password_register;
    @FXML
    private PasswordField pf_confirm_password_register;
    @FXML
    private TextField tf_ftp_username_register;
    @FXML
    private PasswordField pf_ftp_password_register;
    @FXML
    private Button btn_register_register;
    @FXML
    private Hyperlink lnk_login_register;



    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        btn_register_register.setOnAction(new EventHandler<ActionEvent>()
        {
            @Override
            public void handle(ActionEvent actionEvent)
            {
                if(tf_username_register.getText() == null || pf_password_register.getText() == null || pf_confirm_password_register.getText() == null)
                {
                    System.out.println("You have to provide a username, a password and a confirmation password in order to register.");
                    Alert alert = new Alert(Alert.AlertType.ERROR);
                    alert.setContentText("You have to provide a username, a password and a confirmation password in order to register.");
                    alert.show();
                    return;
                }
                if(!Objects.equals(pf_password_register.getText(), pf_confirm_password_register.getText()))
                {
                    System.out.println("Passwords do not match.");
                    Alert alert = new Alert(Alert.AlertType.ERROR);
                    alert.setContentText("Passwords do not match.");
                    alert.show();
                    return;
                }

                FetchRegisterTask t = new FetchRegisterTask(tf_username_register.getText(), pf_password_register.getText(), tf_ftp_username_register.getText(), pf_ftp_password_register.getText());
                t.setOnSucceeded(e-> {
                    Boolean responseStatus = t.getValue();
                    if(!responseStatus)
                    {
                        System.out.println("Internal Error received from the server.");
                        Alert alert = new Alert(Alert.AlertType.ERROR);
                        alert.setContentText("Internal Error received from the server.");
                        alert.show();
                        return;
                    }
                    DbUtil.changeScene(actionEvent, "login.fxml", StageTitleEnum.login, null, null, null);
                    System.out.println("You have successfully registered you account!");
                    Alert alert = new Alert(Alert.AlertType.INFORMATION);
                    alert.setContentText("You have successfully registered you account!");
                    alert.show();
                });
                new Thread(t).start();
            }
        });

        lnk_login_register.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent actionEvent) {
                DbUtil.changeScene(actionEvent, "login.fxml", StageTitleEnum.login, null, null, null);
            }
        });
    }

}
