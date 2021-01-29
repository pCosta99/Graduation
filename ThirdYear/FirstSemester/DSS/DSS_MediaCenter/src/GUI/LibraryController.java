package GUI;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Pos;
import javafx.scene.control.*;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.HBox;
import model.Media;
import logic.MediaCenter;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import static javafx.scene.input.MouseButton.PRIMARY;
import static javafx.scene.input.MouseButton.SECONDARY;

public class LibraryController {
    @FXML private AnchorPane anchorPane;
    @FXML private Tab artistas;
    @FXML private TreeView<Object> artistasTreeView;
    @FXML private Tab random;
    @FXML private ListView<Media> randomListView;
    @FXML private Tab musicasTreeView;
    @FXML private TreeView<Object> musicTreeView;
    @FXML private Tab genre;
    @FXML private TreeView<Object> genreTreeView;
    @FXML private Tab video;
    @FXML private TreeView<Object> videoTreeView;

    private MediaCenter mediaCenter;
    private SoundBarController soundBarController;
    private boolean isGuestLoggedIn;

    public void initData(MediaCenter mediaCenter, SoundBarController soundBarController, boolean isGuestLoggedIn){
        this.mediaCenter = mediaCenter;
        this.soundBarController = soundBarController;
        this.isGuestLoggedIn = isGuestLoggedIn;

        if(!this.isGuestLoggedIn && this.mediaCenter.getUserLoggedIn().isDarkMode()) this.anchorPane.getStylesheets().add("style/dark_theme.css");

        /* Ajustar a modelo de dados */
        List<Media> currentMedia = new ArrayList<>(this.mediaCenter.getAllContentFromUser());
        this.randomListView.setCellFactory(lv -> {
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

                        if(media.isMusic()) {
                            hBox.getChildren().add(new Label(media.getMetadata().getTitle() + " (M)"));
                        }
                        else hBox.getChildren().add(new Label(media.getMetadata().getTitle() + " (V)"));

                        // Set the HBox as the display
                        setGraphic(hBox);
                    }
                }
            };
            cell.setOnMouseClicked(e -> {
                if (!cell.isEmpty()) {
                    if(!this.isGuestLoggedIn && e.getButton().equals(SECONDARY)){
                        rightClickOnMedia(cell.getItem());
                    } else if(e.getButton().equals(PRIMARY) && cell.getItem().isMusic()) this.soundBarController.setMediaPlaying(cell.getItem());
                    else if(e.getButton().equals(PRIMARY) && !cell.getItem().isMusic()) {
                        try {
                            if(this.soundBarController.isPlaying()) this.soundBarController.playClicked();
                            ProcessBuilder pb = new ProcessBuilder("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe", cell.getItem().getPath());
                            pb.start();
                        } catch (IOException e1) {
                            e1.printStackTrace();
                        }
                    }
                }
            });
            return cell;
        });
        ObservableList<Media> observableContent = FXCollections.observableArrayList();
        observableContent.addAll(currentMedia);
        //These items are for configuring the ListArea
        this.randomListView.setItems(observableContent);
        this.randomListView.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
        /* ----------------------------------------------------------------------- */
        /* Create roots */
        TreeItem<Object> rootGenre = new TreeItem<>("Género");
        rootGenre.setExpanded(true);
        TreeItem<Object> rootA = new TreeItem<>("Artistas");
        rootA.setExpanded(true);
        TreeItem<Object> root = new TreeItem<>("Músicas");
        root.setExpanded(true);
        TreeItem<Object> rootVideos = new TreeItem<>("Vídeos");
        rootVideos.setExpanded(true);
        /* Sort the media list accordingly */
        List<Media> musicList = new ArrayList<>(this.mediaCenter.getAllContentFromUser()).stream().filter(Media::isMusic).collect(Collectors.toList());
        List<Media> videoList = new ArrayList<>(this.mediaCenter.getAllContentFromUser()).stream().filter(m -> !m.isMusic()).collect(Collectors.toList());
        /* Initialize the cell factories and the on mouse clicked events */
        createCellFactory(this.artistasTreeView);
        createCellFactory(this.musicTreeView);
        createCellFactory(this.videoTreeView);
        createCellFactory(this.genreTreeView);
        /* ----------------------------------------------------------------------- */
        // Fill the artist tree view
        Map<String, List<Media>> mapArtist = musicList.stream().collect(Collectors.groupingBy(m -> m.getMetadata().getArtist()));
        for(Map.Entry<String,List<Media>> media : mapArtist.entrySet()){
            TreeItem<Object> subRoot = new TreeItem<>(media.getKey());
            subRoot.setExpanded(true);
            Map<String, List<Media>> mapAlbum = media.getValue().stream().collect(Collectors.groupingBy(m -> m.getMetadata().getAlbum()));
            for(Map.Entry<String,List<Media>> media1 : mapAlbum.entrySet()){
                TreeItem<Object> subSubRoot = new TreeItem<>(media1.getKey());
                subSubRoot.setExpanded(true);
                List<Media> orderedByTrack = media1.getValue().stream().sorted(Comparator.comparingInt(m -> Integer.valueOf(m.getMetadata().getTrack()))).collect(Collectors.toList());
                for(Media input : orderedByTrack){
                    subSubRoot.getChildren().add(new TreeItem<>(input));
                }
                subRoot.getChildren().add(subSubRoot);
            }
            rootA.getChildren().add(subRoot);
        }
        this.artistasTreeView.setRoot(rootA);
        /* ----------------------------------------------------------------------- */
        // Fill the music tree view
        Map<String, List<Media>> mapMusic = musicList.stream().collect(Collectors.groupingBy(m -> m.getMetadata().getTitle().substring(0,1).toUpperCase()));
        for(Map.Entry<String,List<Media>> media : mapMusic.entrySet()){
            TreeItem<Object> subRoot = new TreeItem<>(media.getKey());
            subRoot.setExpanded(true);
            for(Media input : media.getValue()){
                subRoot.getChildren().add(new TreeItem<>(input));
            }
            root.getChildren().add(subRoot);
        }
        this.musicTreeView.setRoot(root);
        /* ----------------------------------------------------------------------- */
        // Fill the video tree view
        Map<String, List<Media>> mapVideo = videoList.stream().collect(Collectors.groupingBy(m -> m.getMetadata().getTitle().substring(0,1).toUpperCase()));
        for(Map.Entry<String,List<Media>> media : mapVideo.entrySet()){
            TreeItem<Object> subRoot = new TreeItem<>(media.getKey());
            subRoot.setExpanded(true);
            for(Media input : media.getValue()){
                subRoot.getChildren().add(new TreeItem<>(input));
            }
            rootVideos.getChildren().add(subRoot);
        }
        this.videoTreeView.setRoot(rootVideos);
        /* ----------------------------------------------------------------------- */
        // Fill the genre tree view
        Map<String, List<Media>> mapGenre = musicList.stream().collect(Collectors.groupingBy(m -> m.getMetadata().getGenre()));
        for(Map.Entry<String,List<Media>> media : mapGenre.entrySet()){
            TreeItem<Object> subRoot = new TreeItem<>(media.getKey());
            subRoot.setExpanded(true);
            for(Media input : media.getValue()){
                subRoot.getChildren().add(new TreeItem<>(input));
            }
            rootGenre.getChildren().add(subRoot);
        }
        this.genreTreeView.setRoot(rootGenre);
    }

    private void createCellFactory(TreeView value){
        value.setCellFactory(lv -> {
            TreeCell<Object> cell = new TreeCell<>() {
                @Override
                protected void updateItem(Object media, boolean empty) {
                    super.updateItem(media, empty);

                    if (empty) {
                        setGraphic(null);
                    } else {
                        // Create a HBox to hold our displayed value
                        HBox hBox = new HBox(5);
                        hBox.setAlignment(Pos.CENTER_LEFT);

                        if(media instanceof Media) hBox.getChildren().add(new Label(((Media) media).getMetadata().getTitle()));
                        else hBox.getChildren().add(new Label((String) media));


                        // Set the HBox as the display
                        setGraphic(hBox);
                    }
                }
            };
            cell.setOnMouseClicked(e -> {
                if (!cell.isEmpty() && cell.getItem() instanceof Media) {
                    Media media = (Media) cell.getItem();
                    if(!this.isGuestLoggedIn && e.getButton().equals(SECONDARY)){
                        rightClickOnMedia(media);
                    } else if (e.getButton().equals(PRIMARY) && media.isMusic()) this.soundBarController.setMediaPlaying(media);
                    else if(e.getButton().equals(PRIMARY) && !media.isMusic()) {
                        try {
                            if(this.soundBarController.isPlaying()) this.soundBarController.playClicked();
                            ProcessBuilder pb = new ProcessBuilder("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe", media.getPath());
                            pb.start();
                        } catch (IOException e1) {
                            e1.printStackTrace();
                        }
                    }
                } else if(!cell.isEmpty() && cell.getItem() instanceof String){
                    this.soundBarController.cleanQueue();
                    boolean first = true;
                    for(TreeItem<Object> media : cell.getTreeItem().getChildren()){
                        if(((Media) media.getValue()).isMusic()){
                            if(first) {this.soundBarController.addOneToQueue((Media) media.getValue(), first); first = false;}
                            else this.soundBarController.addOneToQueue((Media) media.getValue(), false);
                        } else {
                            if (first) {
                                try {
                                    this.soundBarController.playClicked();
                                    ProcessBuilder pb = new ProcessBuilder("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe", ((Media) media.getValue()).getPath());
                                    pb.start();
                                    first = false;
                                } catch (IOException e1) {
                                    e1.printStackTrace();
                                }
                            }
                        }
                    }
                }
            });
            return cell;
        });
    }

    public void rightClickOnMedia(Media media) {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("Metadados.fxml"));
            MetadadosController metadadosController = new MetadadosController();
            loader.setController(metadadosController);
            this.anchorPane.getChildren().set(0,loader.load());
            metadadosController.initData(this.mediaCenter, media, this.soundBarController, "library");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
