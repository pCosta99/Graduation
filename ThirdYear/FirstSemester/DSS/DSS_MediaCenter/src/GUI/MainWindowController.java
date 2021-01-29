package GUI;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ButtonType;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import logic.MediaCenter;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.Optional;

public class MainWindowController {
    /* JavaFX components */
    @FXML private AnchorPane mainWindow;
    @FXML private AnchorPane dynamicWindow;
    @FXML private AnchorPane playingBar;
    @FXML private ImageView biblioteca;
    @FXML private ImageView upload;
    @FXML private ImageView search;
    @FXML private ImageView settings;
    @FXML private ImageView userPic;
    @FXML private ImageView logout;

    /* Controller for sound bar */
    @FXML private SoundBarController soundBarController;

    /* Model components */
    private MediaCenter mediaCenter;
    private String currentScreen;
    private boolean isGuestLoggedIn;

    public void initData(MediaCenter mediaCenter) throws IOException {
        this.mediaCenter = mediaCenter;
        this.currentScreen = "library";
        this.isGuestLoggedIn = this.mediaCenter.getUserLoggedInKey().equals("geral");
        if(!isGuestLoggedIn) this.userPic.setImage(new Image(this.mediaCenter.getUserLoggedIn().getProfilePicture()));

        /* Load sound bar */
        FXMLLoader loaderSb = new FXMLLoader(getClass().getResource("SoundBar.fxml"));
        this.playingBar.getChildren().add(loaderSb.load());
        this.soundBarController = loaderSb.getController();
        this.soundBarController.initData(this.mediaCenter, this.isGuestLoggedIn);

        /* Load first dynamic window */
        FXMLLoader loaderLibrary = new FXMLLoader(getClass().getResource("Library.fxml"));
        LibraryController libraryController = new LibraryController();
        loaderLibrary.setController(libraryController);
        this.dynamicWindow.getChildren().add(0,loaderLibrary.load());
        libraryController.initData(this.mediaCenter, this.soundBarController, this.isGuestLoggedIn);

        this.mainWindow.getScene().getWindow().setOnCloseRequest(e -> {this.soundBarController.release();System.exit(0);});

        if(!this.isGuestLoggedIn){
            this.upload.setOnMouseClicked(e -> {
                FileChooser fc = new FileChooser();
                fc.setInitialDirectory(new File("E:\\Downloads\\KANYE WEST - DISCOGRAPHY (2003-16) [CHANNEL NEO]"));
                List<File> list = fc.showOpenMultipleDialog(this.mainWindow.getScene().getWindow());
                if (list != null) {
                    for (File file : list) {
                        if(file.getAbsolutePath().endsWith("mp3") || file.getAbsolutePath().endsWith("mp4")){
                            this.mediaCenter.putContent(file.getAbsolutePath());
                        }
                    }
                }
                if(this.currentScreen.equals("library")) {
                    try {
                        library(true);
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                }
            });
        }

        this.biblioteca.setOnMouseClicked(e -> {
            try {
                library(false);
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        });
    }

    public void settings() throws IOException {
        if(!this.currentScreen.equals("settings") && !isGuestLoggedIn){
            this.currentScreen = "settings";
            FXMLLoader loaderNC = new FXMLLoader(getClass().getResource("settings.fxml"));
            SettingsController settingsController = new SettingsController();
            loaderNC.setController(settingsController);
            this.dynamicWindow.getChildren().set(0,loaderNC.load());
            settingsController.initData(this.mediaCenter);
        }
    }

    public void library(boolean force) throws IOException{
        if(force || !this.currentScreen.equals("library")){
            this.currentScreen = "library";
            FXMLLoader loaderLibrary = new FXMLLoader(getClass().getResource("Library.fxml"));
            LibraryController libraryController = new LibraryController();
            loaderLibrary.setController(libraryController);
            this.dynamicWindow.getChildren().set(0,loaderLibrary.load());
            libraryController.initData(this.mediaCenter, this.soundBarController, isGuestLoggedIn);
        }
    }

    public void logout(MouseEvent mouseEvent) throws IOException {
        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
        alert.setTitle("Confirmation Dialog");
        alert.setHeaderText(null);
        alert.setContentText("Are you ok with this?");

        Optional<ButtonType> result = alert.showAndWait();
        if (result.get() == ButtonType.OK){
            this.soundBarController.release();
            this.mediaCenter.logUser(null);
            FXMLLoader loader = new FXMLLoader(getClass().getResource("InitialScreen.fxml"));
            Stage window = (Stage) ((Node) mouseEvent.getSource()).getScene().getWindow();
            window.setScene(new Scene(loader.load()));
            InitialScreenController initialScreenController = loader.getController();
            initialScreenController.initData(this.mediaCenter);
            window.show();
        }
    }

    public void search() throws IOException{
        if(!this.currentScreen.equals("search")){
            this.currentScreen = "search";
            FXMLLoader loaderSearch = new FXMLLoader(getClass().getResource("search.fxml"));
            SearchController searchController = new SearchController();
            loaderSearch.setController(searchController);
            this.dynamicWindow.getChildren().set(0,loaderSearch.load());
            searchController.initData(this.mediaCenter, this.soundBarController, this.isGuestLoggedIn);
        }
    }
}
