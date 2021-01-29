package DAO;

import java.sql.*;

public class DAOconnection {
    private static final String DB_INITIAL_URL = "jdbc:mysql://localhost:3306/?useTimezone=true&serverTimezone=UTC";
    private static final String DB_URL = "jdbc:mysql://localhost:3306/mediacenter?useTimezone=true&serverTimezone=UTC";
    private static final String DB_USERNAME = "root";
    private static final String DB_PASSWORD = "password";

    /* Devolve conexão à base de dados */
    public static Connection getConnection() throws SQLException{
        return DriverManager.getConnection(DB_URL, DB_USERNAME, DB_PASSWORD);
    }

    /* Cria base de dados mediacenter */
    public static void createDB(){
        try(Connection conn = DriverManager.getConnection(DB_INITIAL_URL, DB_USERNAME, DB_PASSWORD)){
            Statement stm = conn.createStatement();
            String sql = "CREATE DATABASE IF NOT EXISTS MEDIACENTER";
            stm.executeUpdate(sql);
            stm.close();
        } catch (Exception e) { e.printStackTrace(); }
    }

    /* Comando genérico para criar tabelas */
    public static void createTables(String sql){
        try(Connection conn = DriverManager.getConnection(DB_URL, DB_USERNAME, DB_PASSWORD)){
            Statement stmt = conn.createStatement();
            stmt.executeUpdate(sql);
        } catch (Exception e) {throw new NullPointerException(e.getMessage());}
    }

    /* Comando para apagar base de dados mediacenter */
    public static void deleteDB(){
        try(Connection conn = DriverManager.getConnection(DB_INITIAL_URL, DB_USERNAME, DB_PASSWORD)){
            Statement stm = conn.createStatement();
            String sql = "DROP DATABASE MEDIACENTER";
            stm.executeUpdate(sql);
            stm.close();
        } catch (Exception e) { e.printStackTrace(); }
    }
}
