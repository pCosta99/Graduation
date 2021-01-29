package GUI;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.HBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import logic.MediaCenter;
import model.Utilizador;

import java.io.IOException;

public class InitialScreenController {
    @FXML private AnchorPane anchor;
    @FXML private ListView<Utilizador> listView;
    @FXML private Text text;
    @FXML private Button guestLogin;
    @FXML private ImageView close;

    private MediaCenter mediaCenter;

    public void initData(MediaCenter mediaCenter){
        this.mediaCenter = mediaCenter;
        this.anchor.getScene().getWindow().setOnCloseRequest(e -> System.exit(0));
        ObservableList<Utilizador> observableUsers = FXCollections.observableArrayList();
        observableUsers.addAll(this.mediaCenter.getUsers());
        //These items are for configuring the ListArea
        listView.setCellFactory(lv -> {
            ListCell<Utilizador> cell = new ListCell<>() {
                @Override
                protected void updateItem(Utilizador user, boolean empty) {
                    super.updateItem(user, empty);

                    if (empty) {
                        setGraphic(null);
                    } else {
                        // Create a HBox to hold our displayed value
                        HBox hBox = new HBox(5);
                        hBox.setAlignment(Pos.CENTER_LEFT);

                        // Add the values from our piece to the HBox
                        ImageView profilePicture = new ImageView(user.getProfilePicture());
                        // Resize the image, if necessary
                        profilePicture.setFitHeight(80);
                        profilePicture.setFitWidth(60);
                        hBox.getChildren().addAll(profilePicture, new Label(user.getEmail()));

                        // Set the HBox as the display
                        setGraphic(hBox);
                    }
                }
            };
            cell.setOnMouseClicked(e -> {
                if (!cell.isEmpty()) {
                    try {
                        FXMLLoader loader = new FXMLLoader(getClass().getResource("LoginUser.fxml"));
                        Stage window = (Stage)((Node)e.getSource()).getScene().getWindow();
                        window.setScene(new Scene(loader.load()));
                        LoginUserController controller = loader.getController();
                        controller.initData(this.mediaCenter, cell.getItem());
                        window.show();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                }
            });
            return cell;
        });

        listView.setItems(observableUsers);
        listView.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
    }

    public void guestLoginButtonPushed(MouseEvent mouseEvent) throws IOException {
        this.mediaCenter.logUser("geral");
        FXMLLoader loaderMw = new FXMLLoader(getClass().getResource("MainWindow.fxml"));
        Stage window = (Stage) ((Node) mouseEvent.getSource()).getScene().getWindow();
        window.setScene(new Scene(loaderMw.load()));
        MainWindowController mwc = loaderMw.getController();
        mwc.initData(this.mediaCenter);
        window.show();
    }

    public void shutdownButtonPushed(MouseEvent mouseEvent) {
        Stage window = (Stage) ((Node)mouseEvent.getSource()).getScene().getWindow();
        window.close();
        System.exit(0);
    }
}
