package logic;

import DAO.DAOconnection;
import com.mpatric.mp3agic.ID3v2;
import com.mpatric.mp3agic.InvalidDataException;
import com.mpatric.mp3agic.Mp3File;
import com.mpatric.mp3agic.UnsupportedTagException;
import model.*;

import java.io.IOException;
import java.util.*;

public class MediaCenter {
    private MediaDAO bibGeral;
    private UtilizadorDAO users;
    private String userLoggedIn;
    private int mediaCounter;

    /* Constructors */
    public MediaCenter(boolean cleanData){
        // Inicializar a base de dados
        DAOconnection.createDB();
        this.bibGeral = MediaDAO.getInstance();
        this.users = UtilizadorDAO.getInstance();
        if(cleanData) this.clearDB();
        this.userLoggedIn = null;
        this.mediaCounter = this.bibGeral.getAllFromUser("geral").size();
    }

    /* User wise */
    public void newUser(String email, String username, String password, boolean isDarkMode, boolean isAdmin){
        if(this.users.get(email) == null){
            this.users.put(new Utilizador(email, username, password, isDarkMode, isAdmin));
            MediaDAO.getInstance(email);
        }
    }

    public void newUser(Utilizador u){
        String email = u.getEmail();
        if(this.users.get(email) == null){
            this.users.put(u);
            MediaDAO.getInstance(email);
        }
    }

    public void logUser(String userKey){
        this.userLoggedIn = userKey;
    }

    public String getUserLoggedInKey(){
        return this.userLoggedIn;
    }

    public Utilizador getUserLoggedIn(){
        return this.users.get(this.userLoggedIn);
    }

    public Utilizador getUser(String userKey){
        return this.users.get(userKey);
    }

    public Set<String> getUserKeys(){
        return this.users.getKeySet();
    }

    public Collection<Utilizador> getUsers(){
        return this.users.getAllUsers();
    }

    /* Content wise */
    public void putContent(Media m){
        this.bibGeral.put(m, this.mediaCounter++,true, this.users.getKeySet());
    }

    public void putContent(String path){
        try {
            Mp3File mp3file = new Mp3File(path);
            if(mp3file.hasId3v2Tag()) {
                ID3v2 id3v2Tag = mp3file.getId3v2Tag();
                Metadata md = new Metadata(id3v2Tag.getTrack(), id3v2Tag.getArtist(), id3v2Tag.getTitle(), id3v2Tag.getAlbum(), id3v2Tag.getYear(), id3v2Tag.getGenreDescription());
                Media m = new Media(md, path, path.endsWith(".mp3"), this.userLoggedIn);
                this.bibGeral.put(m, this.mediaCounter++, true, this.users.getKeySet());
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (UnsupportedTagException e) {
            e.printStackTrace();
        } catch (InvalidDataException e) {
            e.printStackTrace();
        }
    }

    public void removeContent(Media media){
        this.bibGeral.removeMedia(media);
    }

    public void putCollectionOfContent(Collection<Media> mediaCollection){
        for(Media m : mediaCollection)
            this.bibGeral.put(m, this.mediaCounter++, true, this.users.getKeySet());
    }

    public void putCollectionOfContent(List<String> pathsSet){
        for(String path : pathsSet)
            putContent(path);
    }

    public Media getContent(int contentId) throws MediaNotFoundException {
        return this.bibGeral.get(contentId, this.userLoggedIn);
    }

    public Map<String, Media> getContentFromAllUsers(int contentId){
        return this.bibGeral.getFromAllUsers(contentId);
    }

    public Collection<Media> getAllContentFromUser(){
        return this.bibGeral.getAllFromUser(this.userLoggedIn);
    }

    public Collection<Media> getAllContentFromUser(String userKey){
        return this.bibGeral.getAllFromUser(userKey);
    }

    /* Utility wise */
    public void clearDB(){
        this.bibGeral.clear();
        this.users.clear();
    }

    /* Apaga tabelas na base de dados */
    public void clearTables(){
        this.bibGeral.clearContentTable();
        this.users.clearUserTable();
    }

    public void changeMetadata(Media media){
        this.bibGeral.update(media,this.userLoggedIn);
    }

    public void editUser(Utilizador user){
        this.users.delete(user);
        this.users.put(user);
    }

    public void updateMediaCounter(){
        this.mediaCounter = this.bibGeral.size()+1;
    }
}
