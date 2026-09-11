package com.eastwoodjtb.controllers;


import com.eastwoodjtb.utils.DbUtil;
import com.eastwoodjtb.utils.StageTitleEnum;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;

import java.net.URL;
import java.util.ResourceBundle;

public class HomeController implements Initializable {
    @FXML
    private Button btn_logout_home;

    @FXML
    private Label lbl_username_home;

    @FXML
    private Label lbl_ftp_username_home;


    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        btn_logout_home.setOnAction(new EventHandler<ActionEvent>()
        {
            @Override
            public void handle(ActionEvent event)
            {
                DbUtil.changeScene(event, "login.fxml", StageTitleEnum.login, null, null, null);
            }
        });

    }

    public void setUserInformation(String username)
    {
        lbl_username_home.setText("username: " + username);
        lbl_ftp_username_home.setText("");
    }

    public void storeTokens(String accessToken, String refreshToken)
    {

    }

}
