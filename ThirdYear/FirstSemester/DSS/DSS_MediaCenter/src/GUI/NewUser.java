package GUI;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.*;
import javafx.scene.layout.AnchorPane;
import logic.MediaCenter;
import model.Utilizador;

import java.io.IOException;

public class NewUser {
    @FXML private AnchorPane screen;
    @FXML private TextField nomeUser;
    @FXML private PasswordField pass1;
    @FXML private PasswordField pass2;
    @FXML private TextField email;
    @FXML private CheckBox adminCheckBox;
    @FXML private Label adminLabel;
    @FXML private Button submeter;
    @FXML private Button voltar;

    private MediaCenter mediaCenter;

    public void initData(MediaCenter mediaCenter){
        this.mediaCenter = mediaCenter;

        if(this.mediaCenter.getUserLoggedIn().isDarkMode()) this.screen.getStylesheets().add("style/dark_theme.css");

        this.submeter.setOnMouseClicked(e -> {
            if(this.pass1.getText().equals(this.pass2.getText()) && this.nomeUser.getText().length() > 3 && this.email.getText().length() > 3 && this.pass1.getText().length() > 3){
                this.mediaCenter.newUser(new Utilizador(this.email.getText(), this.nomeUser.getText(), this.pass1.getText(), false, this.adminCheckBox.isSelected()));
                Alert alert = new Alert(Alert.AlertType.INFORMATION);
                alert.setTitle("Information Dialog");
                alert.setHeaderText(null);
                alert.setContentText("User created sucessfully!");
                alert.showAndWait();
                goBack();
            }
            else {
                Alert alert = new Alert(Alert.AlertType.INFORMATION);
                alert.setTitle("Information Dialog");
                alert.setHeaderText(null);
                alert.setContentText("Credentials not entered correctly.");
                alert.showAndWait();
            }
        });

        this.voltar.setOnMouseClicked(e -> goBack());
    }

    public void goBack(){
        FXMLLoader loader = new FXMLLoader(getClass().getResource("settings.fxml"));
        SettingsController settingsController = new SettingsController();
        loader.setController(settingsController);
        this.screen.getChildren().clear();
        try {
            this.screen.getChildren().add(0,loader.load());
        } catch (IOException e1) {
            e1.printStackTrace();
        }
        settingsController.initData(this.mediaCenter);
    }
}
