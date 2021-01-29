package model;

public class Utilizador {
    private String email; // key
    private String username;
    private String password;
    private String profilePicture;
    private boolean isDarkMode;
    private boolean isAdmin; // false -> Normal; true -> Admin

    private final String defaultProfilePicture = "icons/defaultUserPicture.jpg";

    public Utilizador(String email, String username, String password, boolean isDarkMode, boolean isAdmin){
        this.email = email;
        this.username = username;
        this.password = password;
        this.profilePicture = defaultProfilePicture;
        this.isDarkMode = isDarkMode;
        this.isAdmin = isAdmin;
    }

    public Utilizador(String email, String username, String password, String profilePicture, boolean isDarkMode, boolean isAdmin){
        this.email = email;
        this.username = username;
        this.password = password;
        this.profilePicture = profilePicture;
        this.isDarkMode = isDarkMode;
        this.isAdmin = isAdmin;
    }

    public String getEmail(){
        return this.email;
    }

    public String getUsername(){
        return this.username;
    }

    public String getPassword(){
        return this.password;
    }

    public String getProfilePicture(){
        return this.profilePicture;
    }

    public boolean isDarkMode(){
        return this.isDarkMode;
    }

    public boolean isAdmin(){
        return this.isAdmin;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public void setAdmin(boolean admin) {
        isAdmin = admin;
    }

    public void setDarkMode(boolean darkMode) {
        isDarkMode = darkMode;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setProfilePicture(String profilePicture) {
        this.profilePicture = profilePicture;
    }
}
