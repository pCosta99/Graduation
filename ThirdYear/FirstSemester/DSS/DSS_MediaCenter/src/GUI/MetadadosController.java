package GUI;

import com.mpatric.mp3agic.ID3v2;
import com.mpatric.mp3agic.InvalidDataException;
import com.mpatric.mp3agic.Mp3File;
import com.mpatric.mp3agic.UnsupportedTagException;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonType;
import javafx.scene.control.TextField;
import javafx.scene.layout.*;
import javafx.stage.DirectoryChooser;
import model.Media;
import logic.MediaCenter;
import model.Metadata;
import org.apache.commons.io.FileUtils;

import java.io.File;
import java.io.IOException;
import java.util.Optional;


public class MetadadosController {
    @FXML private AnchorPane screen;
    @FXML private TextField nf1;
    @FXML private TextField a1;
    @FXML private TextField a2;
    @FXML private TextField t1;
    @FXML private TextField t2;
    @FXML private TextField al1;
    @FXML private TextField al2;
    @FXML private TextField ano1;
    @FXML private TextField ano2;
    @FXML private TextField gen1;
    @FXML private TextField gen2;
    @FXML private Button submeterButton;
    @FXML private Button downButton;
    @FXML private Button goBack;
    @FXML private Button remover;

    private MediaCenter mediaCenter;
    private Media media;
    private SoundBarController soundBarController;
    private String parent;

    public void initData(MediaCenter mediaCenter, Media m, SoundBarController soundBarController, String parent){
        this.mediaCenter = mediaCenter;
        this.media = m;
        this.soundBarController = soundBarController;
        this.parent = parent;

        if(this.mediaCenter.getUserLoggedIn().isDarkMode()) this.screen.getStylesheets().add("style/dark_theme.css");

        DirectoryChooser directoryChooser = new DirectoryChooser();
        directoryChooser.setInitialDirectory(new File("src"));
        chargeInformation();

        //textfields novos-coluna da direita
        this.submeterButton.setOnMouseClicked(e -> {
            Metadata newMetadata = this.media.getMetadata();
            if(a2.getText().length()>0) newMetadata.setArtist(this.a2.getText());
            if(t2.getText().length()>0) newMetadata.setTitle(this.t2.getText());
            if(al2.getText().length()>0) newMetadata.setAlbum(this.al2.getText());
            if(ano2.getText().length()>0) newMetadata.setYear(this.ano2.getText());
            if(gen2.getText().length()>0) newMetadata.setGenre(this.gen2.getText());
            Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
            alert.setTitle("Confirmation Dialog");
            alert.setHeaderText(null);
            alert.setContentText("Do you wanna proceed?");

            Optional<ButtonType> result = alert.showAndWait();
            if (result.get() == ButtonType.OK){
                this.mediaCenter.changeMetadata(this.media);
                this.media.setMetadata(newMetadata);
                chargeInformation();
                this.a2.clear();
                this.t2.clear();
                this.al2.clear();
                this.ano2.clear();
                this.gen2.clear();
            }
        });
        this.downButton.setOnMouseClicked(e -> {
            try {
                if(this.media.getUploaderID().equals(this.mediaCenter.getUserLoggedInKey())){
                    File selectedDirectory = directoryChooser.showDialog(this.screen.getScene().getWindow());
                    if(this.media.isMusic()){
                        if(selectedDirectory != null) {
                            String path = selectedDirectory.getAbsolutePath() + "\\" + this.media.getMetadata().getTitle() + ".mp3";
                            FileUtils.copyFile(new File(this.media.getPath()),new File(path));
                            if(path.endsWith("mp3")){
                                Mp3File mp3file = new Mp3File(path);
                                if(mp3file.hasId3v2Tag()) {
                                    ID3v2 id3v2Tag = mp3file.getId3v2Tag();
                                    id3v2Tag.setTrack(this.media.getMetadata().getTrack());
                                    id3v2Tag.setArtist(this.media.getMetadata().getArtist());
                                    id3v2Tag.setAlbum(this.media.getMetadata().getAlbum());
                                    id3v2Tag.setTitle(this.media.getMetadata().getTitle());
                                    id3v2Tag.setYear(this.media.getMetadata().getYear());
                                    //id3v2Tag.setGenreDescription(this.media.getMetadata().getGenre());
                                    mp3file.setId3v2Tag(id3v2Tag);
                                }
                            }
                        }
                    } else {
                        String path = selectedDirectory.getAbsolutePath() + "\\" + this.media.getMetadata().getTitle() + ".mp4";
                        FileUtils.copyFile(new File(this.media.getPath()),new File(path));
                    }
                } else {
                    Alert alert = new Alert(Alert.AlertType.INFORMATION);
                    alert.setTitle("Information Dialog");
                    alert.setHeaderText(null);
                    alert.setContentText("You are not the owner of this song!");
                    alert.showAndWait();
                }
            } catch (IOException e1) {
                e1.printStackTrace();
            } catch (InvalidDataException e1) {
            } catch (UnsupportedTagException e1) {
            }
        });
        this.goBack.setOnMouseClicked(e -> {
            try {
                if(parent.equals("library")){
                    FXMLLoader loaderLibrary = new FXMLLoader(getClass().getResource("Library.fxml"));
                    LibraryController libraryController = new LibraryController();
                    loaderLibrary.setController(libraryController);
                    this.screen.getChildren().clear();
                    this.screen.getChildren().add(0,loaderLibrary.load());
                    libraryController.initData(this.mediaCenter, this.soundBarController, false);
                } else if(parent.equals("search")){
                    FXMLLoader loaderSearch = new FXMLLoader(getClass().getResource("search.fxml"));
                    SearchController searchController = new SearchController();
                    loaderSearch.setController(searchController);
                    this.screen.getChildren().clear();
                    this.screen.getChildren().add(0,loaderSearch.load());
                    searchController.initData(this.mediaCenter, this.soundBarController, false);
                }
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        });

        this.remover.setOnMouseClicked(e -> {
            if(this.media.getUploaderID().equals(this.mediaCenter.getUserLoggedInKey())){
                Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
                alert.setTitle("Confirmation Dialog");
                alert.setHeaderText(null);
                alert.setContentText("Do you wanna proceed?");

                Optional<ButtonType> result = alert.showAndWait();
                if (result.get() == ButtonType.OK){
                    this.mediaCenter.removeContent(this.media);
                }
            } else {
                Alert alert = new Alert(Alert.AlertType.INFORMATION);
                alert.setTitle("Information Dialog");
                alert.setHeaderText(null);
                alert.setContentText("You don't own this file!");
                alert.showAndWait();
            }
        });
    }

    public void chargeInformation(){
        this.nf1.setText(this.media.getMetadata().getTrack());
        this.nf1.setEditable(false);
        this.a1.setText(this.media.getMetadata().getArtist());
        this.a1.setEditable(false);
        this.t1.setText(this.media.getMetadata().getTitle());
        this.t1.setEditable(false);
        this.al1.setText(this.media.getMetadata().getAlbum());
        this.al1.setEditable(false);
        this.ano1.setText(this.media.getMetadata().getYear());
        this.ano1.setEditable(false);
        this.gen1.setText(this.media.getMetadata().getGenre());
        this.gen1.setEditable(false);
    }
}
