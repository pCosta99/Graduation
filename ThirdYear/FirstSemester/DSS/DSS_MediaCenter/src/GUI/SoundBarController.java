package GUI;

import javafx.fxml.FXML;
import javafx.scene.control.Slider;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.AnchorPane;
import javafx.scene.text.Text;
import model.Media;
import logic.MediaCenter;
import uk.co.caprica.vlcj.player.base.MediaPlayer;
import uk.co.caprica.vlcj.player.component.AudioPlayerComponent;

import javax.swing.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class SoundBarController {

    /* JavaFX Components */
    @FXML private AnchorPane screen;
    @FXML private Slider soundSlider;
    @FXML private Text timeElapsed;
    @FXML private ImageView fastForward;
    @FXML private ImageView play_pause;
    @FXML private ImageView rewind;
    @FXML private Text playingCurrently;
    @FXML private ImageView nextButton;

    /* Model components */
    private MediaCenter mediaCenter;
    private AudioPlayerComponent audioPlayerComponent;
    private List<Media> mediaList;

    private static final int SKIP_TIME_MS = 10 * 1000;
    private static final int INITIAL_VOLUME = 50;
    private final ScheduledExecutorService executorService = Executors.newSingleThreadScheduledExecutor();
    private Media mediaPlaying;

    public void initData(MediaCenter mediaCenter, boolean guestLoggedIn){
        this.mediaCenter = mediaCenter;
        this.mediaPlaying = null;
        if(!guestLoggedIn && this.mediaCenter.getUserLoggedIn().isDarkMode()) this.screen.getStylesheets().add("style/dark_theme.css");
        this.mediaList = new ArrayList<>();

        this.audioPlayerComponent = new AudioPlayerComponent() {
            @Override
            public void finished(MediaPlayer player){
                nextMedia();
            }
            @Override
            public void error(MediaPlayer mediaPlayer) {
                System.out.println("Failed to play media");
                System.exit(1);
            }
        };

        executorService.scheduleAtFixedRate(new UpdateRunnable(this.audioPlayerComponent.mediaPlayer()), 0L, 1L, TimeUnit.SECONDS);
        this.audioPlayerComponent.mediaPlayer().audio().setVolume(INITIAL_VOLUME);
    }

    public void addOneToQueue(Media media, boolean restart){
        if(!restart && this.audioPlayerComponent.mediaPlayer().status().isPlaying()){
            this.mediaList.add(this.mediaList.size(), media);
        } else {
            setMediaPlaying(media);
        }
    }

    public void nextMedia(){
        if(this.mediaList.size() > 0) {
            setMediaPlaying(this.mediaList.get(0));
            this.mediaList.remove(0);
        }
    }

    public void cleanQueue(){
        this.mediaList.clear();
    }

    public void playClicked() {
        if(this.mediaPlaying != null){
            if(!this.audioPlayerComponent.mediaPlayer().status().isPlaying()){
                this.audioPlayerComponent.mediaPlayer().controls().start();
                this.play_pause.setImage(new Image("icons/pause.png"));
            } else{
                this.audioPlayerComponent.mediaPlayer().controls().pause();
                this.play_pause.setImage(new Image("icons/play.png"));
            }
        }
    }

    public void forward(){
        if(this.mediaPlaying != null && this.audioPlayerComponent.mediaPlayer().status().isPlaying()){
            if((this.audioPlayerComponent.mediaPlayer().status().length() - this.audioPlayerComponent.mediaPlayer().status().time()) > SKIP_TIME_MS){
                this.audioPlayerComponent.mediaPlayer().controls().skipTime(SKIP_TIME_MS);
            }
        }
    }

    public void rewind(){
        if(this.mediaPlaying != null && this.audioPlayerComponent.mediaPlayer().status().isPlaying()){
            this.audioPlayerComponent.mediaPlayer().controls().skipTime(-SKIP_TIME_MS);
        }
    }

    public void slideSoundBar(){
        this.audioPlayerComponent.mediaPlayer().audio().setVolume((int) soundSlider.getValue());
    }

    public void changePlayingState(boolean play){
        if(play) this.play_pause.setImage(new Image("icons/pause.png"));
        else {
            this.play_pause.setImage(new Image("icons/play.png"));
        }
    }

    private final class UpdateRunnable implements Runnable {

        private final MediaPlayer mediaPlayer;

        private UpdateRunnable(MediaPlayer mediaPlayer) {
            this.mediaPlayer = mediaPlayer;
        }

        @Override
        public void run() {
            final long time = mediaPlayer.status().time();

            // Updates to user interface components must be executed on the Event
            // Dispatch Thread
            SwingUtilities.invokeLater(() -> {
                if(mediaPlayer.status().isPlaying()) {
                    updateTime(time);
                    if(this.mediaPlayer.status().time() > this.mediaPlayer.status().length()-1000){
                        changePlayingState(false);
                    }
                }
            });
        }
    }

    private void updateTime(long millis) {
        long currentTime = this.audioPlayerComponent.mediaPlayer().status().length();
        String s = String.format("%02d:%02d:%02d", TimeUnit.MILLISECONDS.toHours(millis), TimeUnit.MILLISECONDS.toMinutes(millis) - TimeUnit.HOURS.toMinutes(TimeUnit.MILLISECONDS.toHours(millis)), TimeUnit.MILLISECONDS.toSeconds(millis) - TimeUnit.MINUTES.toSeconds(TimeUnit.MILLISECONDS.toMinutes(millis)));
        String s1 = String.format("%02d:%02d:%02d", TimeUnit.MILLISECONDS.toHours(currentTime), TimeUnit.MILLISECONDS.toMinutes(currentTime), TimeUnit.MILLISECONDS.toSeconds(currentTime) - TimeUnit.MINUTES.toSeconds(TimeUnit.MILLISECONDS.toMinutes(currentTime)));
        timeElapsed.setText(s + "/" + s1);
    }

    public void release(){
        this.audioPlayerComponent.release();
    }

    public void setMediaPlaying(Media media){
        if(this.audioPlayerComponent.mediaPlayer().status().isPlaying()) this.audioPlayerComponent.mediaPlayer().controls().stop();
        this.audioPlayerComponent.mediaPlayer().media().play(media.getPath());
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        this.mediaPlaying = media;
        this.play_pause.setImage(new Image("icons/pause.png"));
        this.playingCurrently.setText(media.getMetadata().getArtist() + "\n" + media.getMetadata().getTitle());
    }

    public boolean isPlaying(){
        return this.audioPlayerComponent.mediaPlayer().status().isPlaying();
    }
}
