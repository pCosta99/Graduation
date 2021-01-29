package GUI;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Pos;
import javafx.scene.control.*;
import javafx.scene.input.MouseButton;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.HBox;
import model.Media;
import logic.MediaCenter;

import java.io.IOException;
import java.util.List;
import java.util.stream.Collectors;

public class SearchController {

    @FXML private AnchorPane anchor;
    @FXML private TextField searchText;
    @FXML private Button searchButton;
    @FXML private ListView<Media> searchListView;

    private MediaCenter mediaCenter;
    private SoundBarController soundBarController;
    private boolean isGuestLoggedIn;

    public void initData(MediaCenter mediaCenter, SoundBarController soundBarController, boolean isGuestLoggedIn){
        this.mediaCenter = mediaCenter;
        this.soundBarController = soundBarController;
        this.isGuestLoggedIn = isGuestLoggedIn;

        if(!this.isGuestLoggedIn && this.mediaCenter.getUserLoggedIn().isDarkMode()) this.anchor.getStylesheets().add("style/dark_theme.css");

        this.searchListView.setCellFactory(lv -> {
            ListCell<Media> cell = new ListCell<>() {
                @Override
                protected void updateItem(Media media, boolean empty) {
                    super.updateItem(media, empty);

                    if (empty) {
                        setGraphic(null);
                    } else {
                        // Create a HBox to hold our displayed value
                        HBox hBox = new HBox(5);
                        hBox.setAlignment(Pos.CENTER_LEFT);

                        if(media.isMusic()) hBox.getChildren().add(new Label(media.getMetadata().getTitle() + " (M)"));
                        else hBox.getChildren().add(new Label(media.getMetadata().getTitle() + " (V)"));

                        // Set the HBox as the display
                        setGraphic(hBox);
                    }
                }
            };
            cell.setOnMouseClicked(e -> {
                if (!cell.isEmpty()) {
                    if(!isGuestLoggedIn && e.getButton().equals(MouseButton.SECONDARY)){
                        try {
                            FXMLLoader loader = new FXMLLoader(getClass().getResource("Metadados.fxml"));
                            MetadadosController metadadosController = new MetadadosController();
                            loader.setController(metadadosController);
                            this.anchor.getChildren().clear();
                            this.anchor.getChildren().add(0,loader.load());
                            metadadosController.initData(this.mediaCenter, cell.getItem(), this.soundBarController, "search");
                        } catch (IOException e1) {e1.printStackTrace();}
                    } else if(e.getButton().equals(MouseButton.PRIMARY)) {
                        Media choice = cell.getItem();
                        if(choice.isMusic()){
                            this.soundBarController.setMediaPlaying(choice);
                        } else if(!choice.isMusic()){
                            try {
                                if(this.soundBarController.isPlaying()) this.soundBarController.playClicked();
                                ProcessBuilder pb = new ProcessBuilder("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe", choice.getPath());
                                pb.start();
                            } catch (IOException e1) {
                                e1.printStackTrace();
                            }
                        }
                    }
                }
            });
            return cell;
        });

        this.searchButton.setOnMouseClicked(e->{
            String search = this.searchText.getText().toUpperCase();
            List<Media> mediaList = this.mediaCenter.getAllContentFromUser().stream().filter(m-> m.getMetadata().getTitle().toUpperCase().contains(search) || m.getMetadata().getArtist().toUpperCase().contains(search)).collect(Collectors.toList());
            ObservableList<Media> observableList = FXCollections.observableArrayList();
            observableList.addAll(mediaList);
            this.searchListView.setItems(observableList);
            this.searchListView.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
        });

        this.searchText.setOnMouseClicked(e -> this.searchText.clear());
    }
}
