package GUI;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.PasswordField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.HBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import logic.MediaCenter;
import model.Utilizador;

import java.io.IOException;

public class LoginUserController{

    @FXML private HBox hBoxProfilePicture;

    @FXML private Text text;

    @FXML private ImageView profilePicture;

    @FXML private PasswordField passField;

    @FXML private Text usernameField;

    @FXML private Button entrarButton;

    @FXML private ImageView goBackButton;

    private MediaCenter mediaCenter;

    public void initData(MediaCenter mediaCenter, Utilizador userSelected){
        this.mediaCenter = mediaCenter;
        usernameField.setText(userSelected.getEmail());
        profilePicture.setImage(new Image(userSelected.getProfilePicture()));
        profilePicture.fitWidthProperty().bind(hBoxProfilePicture.widthProperty());
        profilePicture.fitHeightProperty().bind(hBoxProfilePicture.heightProperty());
    }

    public void goBack(MouseEvent mouseEvent) throws IOException {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("InitialScreen.fxml"));
        Stage window = (Stage)((Node)mouseEvent.getSource()).getScene().getWindow();
        window.setScene(new Scene(loader.load()));
        InitialScreenController controller = loader.getController();
        controller.initData(this.mediaCenter);
        window.show();
    }

    public void loginButtonClicked(MouseEvent mouseEvent) throws IOException {
        if(passField.getText().equals(this.mediaCenter.getUser(usernameField.getText()).getPassword())){
            this.mediaCenter.logUser(usernameField.getText());
            this.mediaCenter.updateMediaCounter();

            FXMLLoader loaderMw = new FXMLLoader(getClass().getResource("MainWindow.fxml"));
            Stage window = (Stage) ((Node) mouseEvent.getSource()).getScene().getWindow();
            window.setScene(new Scene(loaderMw.load()));
            MainWindowController mwc = loaderMw.getController();
            mwc.initData(this.mediaCenter);
            window.show();
        } else {
            passField.clear();
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Information Dialog");
            alert.setHeaderText(null);
            alert.setContentText("Password mismatch.");
            alert.showAndWait();
        }
    }
}
