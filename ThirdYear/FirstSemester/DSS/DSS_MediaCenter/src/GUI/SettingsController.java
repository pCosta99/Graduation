package GUI;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.*;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.GridPane;
import logic.MediaCenter;
import model.Utilizador;

import java.io.IOException;
import java.util.Optional;

public class SettingsController {
    @FXML private GridPane grid;
    @FXML private AnchorPane screen;
    @FXML private RadioButton notDarkMode;
    @FXML private RadioButton darkMode;
    @FXML private Button buttonSubmit;
    @FXML private TextField currentUser;
    @FXML private TextField newUser;
    @FXML private PasswordField newPass;
    @FXML private PasswordField checkPass;
    @FXML private Button buttonNewUser;
    private ToggleGroup toggleGroup;

    private MediaCenter mediaCenter;

    public void initData(MediaCenter mediaCenter){
        this.mediaCenter = mediaCenter;

        if(this.mediaCenter.getUserLoggedIn().isDarkMode()) {
            this.grid.getStylesheets().add("style/dark_theme.css");
            this.screen.getStylesheets().add("style/dark_theme.css");
        }

        this.toggleGroup = new ToggleGroup();
        this.darkMode.setToggleGroup(this.toggleGroup);
        this.notDarkMode.setToggleGroup(this.toggleGroup);
        if(this.mediaCenter.getUserLoggedIn().isDarkMode()) this.toggleGroup.selectToggle(this.darkMode);
        else this.toggleGroup.selectToggle(this.notDarkMode);

        this.currentUser.setText(this.mediaCenter.getUserLoggedIn().getUsername());
        this.currentUser.setEditable(false);

        if(!this.mediaCenter.getUserLoggedIn().isAdmin()) this.buttonNewUser.setVisible(false);

        this.buttonSubmit.setOnMouseClicked(e -> {
            Utilizador newUser = this.mediaCenter.getUserLoggedIn();
            if(this.newUser.getText().length() > 3) newUser.setUsername(this.newUser.getText());
            if(this.newPass.getText().length() > 3 && this.newPass.getText().equals(this.checkPass.getText())) newUser.setPassword(this.newPass.getText());
            newUser.setDarkMode(this.darkMode.isSelected());
            Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
            alert.setTitle("Confirmation Dialog");
            alert.setHeaderText(null);
            alert.setContentText("Do you wanna proceed?");

            Optional<ButtonType> result = alert.showAndWait();
            if (result.get() == ButtonType.OK){
                this.mediaCenter.editUser(newUser);
                this.currentUser.setText(newUser.getUsername());
            }

            this.newPass.clear();
            this.newUser.clear();
            this.checkPass.clear();
        });

        this.buttonNewUser.setOnMouseClicked(e -> {
            try {
                FXMLLoader loaderNC = new FXMLLoader(getClass().getResource("novoUser.fxml"));
                NewUser newUser = new NewUser();
                loaderNC.setController(newUser);
                this.screen.getChildren().clear();
                this.screen.getChildren().add(0,loaderNC.load());
                newUser.initData(this.mediaCenter);
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        });
    }
}
