package GUI;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import logic.MediaCenter;

public class Main extends Application {

    private MediaCenter mediaCenter;

    @Override
    public void start(Stage primaryStage) throws Exception{
        this.mediaCenter = new MediaCenter(false);

        //this.mediaCenter.newUser("admin@", "ADMIN", "admin", false, true);
        FXMLLoader loader = new FXMLLoader(getClass().getResource("InitialScreen.fxml"));
        primaryStage.setTitle("Media Center");
        primaryStage.setScene(new Scene(loader.load()));
        InitialScreenController controller = loader.getController();
        controller.initData(this.mediaCenter);
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
