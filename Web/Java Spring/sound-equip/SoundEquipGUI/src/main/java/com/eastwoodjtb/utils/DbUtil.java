package com.eastwoodjtb.utils;

import com.eastwoodjtb.controllers.HomeController;
import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.Objects;


public class DbUtil {
/*    private static final String h2Url = "jdbc:h2:file:~/soundequip/SoundEquipH2Db;DATABASE_TO_UPPER=false";
    private static final String h2User = "sa";
    private static final String h2Password = "kepo&friends";*/


    public static void changeScene(ActionEvent event, String fxmlFile, StageTitleEnum stageTitle, String accessToken, String refreshToken, String username)
    {
        Parent root;
        if(stageTitle == StageTitleEnum.home)
            try {
                FXMLLoader loader = new FXMLLoader(DbUtil.class.getResource(fxmlFile));
                root = loader.load();
                HomeController homeController = loader.getController();
                homeController.setUserInformation(username);
                homeController.storeTokens(accessToken, refreshToken);
            }
            catch (IOException e)
            {
                e.printStackTrace();
                return;
            }
        else
            try
            {
                root = FXMLLoader.load(Objects.requireNonNull(DbUtil.class.getResource(fxmlFile)));
            }
            catch (IOException e)
            {
                e.printStackTrace();
                return;
            }
        Stage stage = (Stage) ((Node) event.getSource()).getScene().getWindow();
        stage.setTitle(stageTitle.getTitle());
        stage.setScene(new Scene(root, 600, 400));
        stage.show();
    }
}
// --module-path "path\to\javafx-sdk-17\lib" --add-modules
// javafx.controls,javafx.fxml