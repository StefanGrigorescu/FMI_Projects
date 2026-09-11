package com.eastwoodjtb.controllers;


import com.eastwoodjtb.fetches.FetchLoginTask;
import com.eastwoodjtb.utils.DbUtil;
import com.eastwoodjtb.utils.StageTitleEnum;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;

import java.net.URL;
import java.util.ResourceBundle;


public class LoginController implements Initializable {
    @FXML
    private TextField tf_username_login;

    @FXML
    private PasswordField pf_password_login;

    @FXML
    private Button btn_login_login;

    @FXML
    private Hyperlink lnk_register_login;

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        btn_login_login.setOnAction(new EventHandler<ActionEvent>()
        {
            @Override
            public void handle(ActionEvent actionEvent)
            {
                if(tf_username_login.getText() == null || pf_password_login.getText() == null)
                {
                    System.out.println("Both username and password must be provided in order to login.");
                    Alert alert = new Alert(Alert.AlertType.ERROR);
                    alert.setContentText("Both username and password must be provided in order to login.");
                    alert.show();
                    return;
                }

                FetchLoginTask t = new FetchLoginTask(tf_username_login.getText(), pf_password_login.getText());
                t.setOnSucceeded(e-> {
                    String accessToken = t.getValue().getAccess_token();
                    String refreshToken = t.getValue().getRefresh_token();
                    DbUtil.changeScene(actionEvent, "home.fxml", StageTitleEnum.home, accessToken, refreshToken, tf_username_login.getText());
                });
                new Thread(t).start();
            }
        });

        lnk_register_login.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent actionEvent) {
                DbUtil.changeScene(actionEvent, "register.fxml", StageTitleEnum.register, null, null, null);
            }
        });
    }

}
