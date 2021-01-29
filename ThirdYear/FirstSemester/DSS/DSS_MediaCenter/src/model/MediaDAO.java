package model;

import DAO.DAOconnection;

import java.sql.*;
import java.util.*;

public class MediaDAO {
    private static MediaDAO bibInst = null;

    /* Maneira para ter vários maps numa só biblioteca */

    /* Apenas uma tabela para a biblioteca do genero de:
    _____________________________________
    | MEDIA_ID | NOME | ... | OWNER_ID  |
    | A        |  B   | ... |  geral    | -> Para a biblioteca geral
    | A        |  B   | ... |    U1     |
    | A        |  B   | ... |    U2     |
    | A        |  B   | ... |    U3     |
    | D        |  B   | ... |    U1     |
    | D        |  B   | ... |    U2     |
    | D        |  B   | ... |    U3     |
    _____________________________________
     */

    /* Construtor */
    private MediaDAO(){
        try{
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            throw new NullPointerException(e.getMessage());
        }
    }

    /* Devolve a instância da base de dados de conteúdo. Este método é usado na inicialização da base de dados. */
    public static MediaDAO getInstance(){
        bibInst = new MediaDAO();
        bibInst.createMediaTables();
        return bibInst;
    }

    /* Devolve a instância da base de dados de conteúdo. Este método é usado quando se cria um novo utilizador. */
    public static MediaDAO getInstance(String userKey){
        bibInst.replicateTable(userKey);
        return bibInst;
    }

    /* Limpa tabela de contéudo. */
    public void clear () {
        try (Connection conn = DAOconnection.getConnection()) {
            Statement stm = conn.createStatement();
            stm.executeUpdate("DELETE FROM BIB");
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Apaga tabela de conteúdo */
    public void clearContentTable(){
        try (Connection conn = DAOconnection.getConnection()) {
            Statement stm = conn.createStatement();
            stm.executeUpdate("DROP TABLE BIB");
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Comando usado para criar as tabelas do conteúdo na base de dados. */
    public void createMediaTables(){
        /* Ajeitar consoante as tabelas que formos ter */
        StringBuilder sql = new StringBuilder("CREATE TABLE IF NOT EXISTS BIB ");
        sql.append("(id INTEGER not NULL, ");
        sql.append(" track INTEGER not NULL, ");
        sql.append(" artist VARCHAR(255), ");
        sql.append(" title VARCHAR(255), ");
        sql.append(" album VARCHAR(255), ");
        sql.append(" yearReleased VARCHAR(255), ");
        sql.append(" genre VARCHAR(255), ");
        sql.append(" path VARCHAR(255), ");
        sql.append(" isMusic BOOL, ");
        sql.append(" uploaderID VARCHAR(255), ");
        sql.append(" ownerID VARCHAR(255), ");
        sql.append(" PRIMARY KEY ( id, ownerID ))");

        DAOconnection.createTables(sql.toString());
    }

    /* Replica o conteúdo existente até ao momento (na bib. geral) para o novo utilizador. */
    public void replicateTable(String userKey){
        Set<String> uniqueKeySet = new HashSet<>();
        uniqueKeySet.add(userKey);
        for(Media m : this.getAllFromUser("geral"))
            this.put(m, m.getId(), false, uniqueKeySet);
    }

    /* Insere conteúdo na tabela apropriada */
    public void put(Media value, int contentId, boolean putInGeral, Set<String> keySet){
        value.setId(contentId);
        System.out.println(value.toString());
        try (Connection conn = DAOconnection.getConnection()) {
            String query = " INSERT INTO BIB (id, track, artist, title, album, yearReleased, genre, path, isMusic, uploaderID, ownerID)"
                    + " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
            // create the mysql insert preparedstatement
            PreparedStatement stm = conn.prepareStatement(query);
            stm.setInt (1, contentId);
            stm.setInt (2, Integer.valueOf(value.getMetadata().getTrack()));
            stm.setString(3, value.getMetadata().getArtist());
            stm.setString (4, value.getMetadata().getTitle());
            stm.setString(5, value.getMetadata().getAlbum());
            stm.setString(6, value.getMetadata().getYear());
            stm.setString(7, value.getMetadata().getGenre());
            stm.setString(8, value.getPath());
            stm.setBoolean (9, value.isMusic());
            stm.setString(10, value.getUploaderID());
            if(putInGeral) {
                stm.setString(11, "geral");
                stm.executeUpdate();
            }
            for(String key : keySet){
                stm.setString (11, key);
                stm.executeUpdate();
            }
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    public int size(){
        try (Connection conn = DAOconnection.getConnection()) {
            Statement stm = conn.createStatement();
            String query = "SELECT MAX(ID) FROM BIB";
            ResultSet rs = stm.executeQuery(query);
            if(rs.next()) return rs.getInt(1);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return -1;
    }

    /* Devolve um objeto cujo ID corresponde a contentID e pertence ao utilizador com chave userKey */
    public Media get(int contentId, String userKey) throws MediaNotFoundException {
        if(contentId == -1) throw new MediaNotFoundException("Couldn't find media with id " + contentId + ".");
        try (Connection conn = DAOconnection.getConnection()){
            Media m = null;
            Statement stm = conn.createStatement();
            String sql = "SELECT * FROM BIB WHERE id='"+contentId+"' AND ownerID='"+userKey+"'";
            ResultSet rs = stm.executeQuery(sql);
            if (rs.next()){
                boolean isMusic = rs.getBoolean("isMusic");
                String path = rs.getString("path");
                Metadata md = new Metadata(String.valueOf(rs.getInt("track")), rs.getString("artist"), rs.getString("title"),
                                           rs.getString("album"), rs.getString("yearReleased"), rs.getString("genre"));
                m = new Media(contentId, md, path, isMusic, rs.getString("uploaderID"));
            }
            return m;
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Devolve o objeto identificado por contentID dentro de cada utilizador diferente */
    public Map<String, Media> getFromAllUsers(int contentId){
        try (Connection conn = DAOconnection.getConnection()){
            Map<String, Media> mediaList = new HashMap<>();
            Statement stm = conn.createStatement();
            String sql = "SELECT * FROM BIB WHERE id='"+contentId+"'";
            ResultSet rs = stm.executeQuery(sql);
            while (rs.next()){
                boolean isMusic = rs.getBoolean("isMusic");
                String path = rs.getString("path");
                Metadata md = new Metadata(rs.getString("track"), rs.getString("artist"), rs.getString("title"),
                        rs.getString("album"), rs.getString("yearReleased"), rs.getString("genre"));
                mediaList.put(rs.getString("ownerID"), new Media(contentId, md, path, isMusic, rs.getString("uploaderID")));
            }
            return mediaList;
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Devolve o conteúdo de um utilizador */
    public Collection<Media> getAllFromUser(String userKey){
        try (Connection conn = DAOconnection.getConnection()){
            Collection<Media> mediaCollection = new HashSet<>();
            Statement stm = conn.createStatement();
            String sql = "SELECT * FROM BIB WHERE ownerID='"+userKey+"'";
            ResultSet rs = stm.executeQuery(sql);
            while (rs.next()){
                boolean isMusic = rs.getBoolean("isMusic");
                String path = rs.getString("path");
                Metadata md = new Metadata(rs.getString("track"), rs.getString("artist"), rs.getString("title"),
                        rs.getString("album"), rs.getString("yearReleased"), rs.getString("genre"));
                mediaCollection.add(new Media(rs.getInt("id"), md, path, isMusic, rs.getString("uploaderID")));
            }
            return mediaCollection;
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    public void removeMedia(Media media){
        try (Connection conn = DAOconnection.getConnection()){
            String query = "DELETE FROM BIB WHERE ID = ?";
            PreparedStatement preparedStmt = conn.prepareStatement(query);
            preparedStmt.setInt(1, media.getId());
            preparedStmt.execute();
        }catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    public void update(Media media, String ownerID){
        try (Connection conn = DAOconnection.getConnection()){
            Metadata md = media.getMetadata();
            String query = "UPDATE BIB SET TRACK = ?, ARTIST = ?, TITLE = ?, ALBUM = ?, YEARRELEASED = ?, GENRE = ? WHERE ID = ? AND OWNERID = ?;";
            PreparedStatement preparedStatement = conn.prepareStatement(query);
            preparedStatement.setString(1, md.getTrack());
            preparedStatement.setString(2, md.getArtist());
            preparedStatement.setString(3, md.getTitle());
            preparedStatement.setString(4, md.getAlbum());
            preparedStatement.setString(5, md.getYear());
            preparedStatement.setString(6, md.getGenre());
            preparedStatement.setInt(7, media.getId());
            preparedStatement.setString(8, ownerID);
            preparedStatement.execute();
        } catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }
}

