package model;

import DAO.DAOconnection;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;

public class UtilizadorDAO {
    private static UtilizadorDAO userInst = null;

    /* Construtor */
    private UtilizadorDAO(){
        try{
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            throw new NullPointerException(e.getMessage());
        }
    }

    /* Devolve a instância da base de dados de conteúdo. Este método é usado quando se cria um novo utilizador. */
    public static UtilizadorDAO getInstance(){
        userInst = new UtilizadorDAO();
        userInst.createUserTables();
        return userInst;
    }

    /* Limpa tabela de utilizadores. */
    public void clear () {
        try (Connection conn = DAOconnection.getConnection()) {
            Statement stm = conn.createStatement();
            stm.executeUpdate("DELETE FROM USERS");
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Apaga tabela de utilizadores */
    public void clearUserTable(){
        try (Connection conn = DAOconnection.getConnection()) {
            Statement stm = conn.createStatement();
            stm.executeUpdate("DROP TABLE USERS");
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Comando usado para criar as tabelas dos utilizadores na base de dados. */
    public void createUserTables(){
        StringBuilder sql = new StringBuilder("CREATE TABLE IF NOT EXISTS USERS ");
        sql.append("(email VARCHAR(255), ");
        sql.append(" username VARCHAR(255), ");
        sql.append(" password VARCHAR(255), ");
        sql.append(" isDarkMode BOOL, ");
        sql.append(" isAdmin BOOL, ");
        sql.append(" PRIMARY KEY ( email ))");

        DAOconnection.createTables(sql.toString());
    }

    public void delete(Utilizador u){
        try (Connection conn = DAOconnection.getConnection()){
            String query = "DELETE FROM USERS WHERE EMAIL = ?";
            PreparedStatement preparedStmt = conn.prepareStatement(query);
            preparedStmt.setString(1, u.getEmail());
            preparedStmt.execute();
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Introduz um utilizador na tabela apropriada */
    public void put(Utilizador u){
        try (Connection conn = DAOconnection.getConnection()) {
            String query = " INSERT INTO USERS (email, username, password, isDarkMode, isAdmin)"
                    + " VALUES (?, ?, ?, ?, ?)";
            // create the mysql insert preparedstatement
            PreparedStatement stm = conn.prepareStatement(query);
            stm.setString (1, u.getEmail());
            stm.setString (2, u.getUsername());
            stm.setString (3, u.getPassword());
            stm.setBoolean (4, u.isDarkMode());
            stm.setBoolean (5, u.isAdmin());
            stm.executeUpdate();
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Devolve um utilizador com base no seu email (userKey) */
    public Utilizador get(String userKey){
        try (Connection conn = DAOconnection.getConnection()){
            Utilizador u = null;
            Statement stm = conn.createStatement();
            String sql = "SELECT * FROM USERS WHERE email='"+userKey+"'";
            ResultSet rs = stm.executeQuery(sql);
            if (rs.next()){
                u = new Utilizador(rs.getString("email"),rs.getString("username"),rs.getString("password"), rs.getBoolean("isDarkMode"), rs.getBoolean("isAdmin"));
            }
            return u;
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Devolveuma lista com todos os utilizadores presentes na base de dados */
    public Collection<Utilizador> getAllUsers(){
        try (Connection conn = DAOconnection.getConnection()){
            Collection<Utilizador> col = new ArrayList<>();
            Statement stm = conn.createStatement();
            String sql = "SELECT * FROM USERS";
            ResultSet rs = stm.executeQuery(sql);
            while (rs.next()){
                col.add(new Utilizador(rs.getString("email"),rs.getString("username"),rs.getString("password"), rs.getBoolean("isDarkMode"), rs.getBoolean("isAdmin")));
            }
            return col;
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Devolve o keySet dos utilizadores presentes na tabela */
    public Set<String> getKeySet(){
        try (Connection conn = DAOconnection.getConnection()){
            Set<String> keySet = new HashSet<>();
            Statement stm = conn.createStatement();
            String sql = "SELECT * FROM USERS";
            ResultSet rs = stm.executeQuery(sql);
            while (rs.next()){
                keySet.add(rs.getString("email"));
            }
            return keySet;
        }
        catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }
}
