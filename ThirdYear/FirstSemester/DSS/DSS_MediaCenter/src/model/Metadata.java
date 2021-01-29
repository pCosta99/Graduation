package model;

public class Metadata {
    private String track;
    private String artist;
    private String title;
    private String album;
    private String year;
    private String genre;

    public Metadata(String track, String artist, String title, String album, String year, String genre) {
        this.track = track;
        this.artist = artist;
        this.title = title;
        this.album = album;
        this.year = year;
        this.genre = genre;
    }

    public String getTrack() {
        return track;
    }

    public void setTrack(String track) {
        this.track = track;
    }

    public String getArtist() {
        return artist;
    }

    public void setArtist(String artist) {
        this.artist = artist;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getAlbum() {
        return album;
    }

    public void setAlbum(String album) {
        this.album = album;
    }

    public String getYear() {
        return year;
    }

    public void setYear(String year) {
        this.year = year;
    }

    public String getGenre() {
        return genre;
    }

    public void setGenre(String genre) {
        this.genre = genre;
    }

    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("Track: ");
        sb.append(this.track);
        sb.append("\nArtist: ");
        sb.append(this.artist);
        sb.append("\nTitle: ");
        sb.append(this.title);
        sb.append("\nAlbum: ");
        sb.append(this.album);
        sb.append("\nYear: ");
        sb.append(this.year);
        sb.append("\nGenre: ");
        sb.append(this.genre);
        sb.append("\n");
        return sb.toString();
    }
}
