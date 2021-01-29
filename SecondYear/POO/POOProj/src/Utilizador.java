import java.io.Serializable;
import java.time.LocalDate;
import java.util.Date;
import java.util.Random;

/**
 * Utilizador do sistema.
 */
public class Utilizador implements Serializable {
    private int userNum;
    private String email;
    private String nif;
    private String password;
    private String morada;
    private String nome;
    private LocalDate dataNascimento;

    private static int userCounter = 0;

    /**
     * Construtor vazio
     */
    public Utilizador(){
        this.userNum = getUserCounter();
        this.email = null;
        this.nif = null;
        this.password = "1234";
        this.morada = null;
        this.nome = null;
        this.randomizer();
    }

    /**
     * Construtor parametrizado
     */
    public Utilizador(String email, String username, String password,
                      String morada, String nome, LocalDate dataNascimento){
        this.userNum = getUserCounter();
        this.email = email;
        this.nif = username;
        this.password = password;
        this.morada = morada;
        this.nome = nome;
        this.dataNascimento = dataNascimento;
    }

    /**
     * Construtor de cópia
     */
    public Utilizador(Utilizador user){
        this.userNum = user.getUserNum();
        this.email = user.getEmail();
        this.nif = user.getNif();
        this.password = user.getPassword();
        this.morada = user.getMorada();
        this.nome = user.getNome();
        this.dataNascimento = user.getDataNascimento();
    }
    public void randomizer(){
        Random random = new Random();
        int minDay = (int) LocalDate.of(1950,1,1).toEpochDay();
        int maxDay = (int) LocalDate.of(1999,12,31).toEpochDay();
        long randomDay = minDay + random.nextInt(maxDay - minDay);
        LocalDate randomDate = LocalDate.ofEpochDay(randomDay);
        this.setDataNascimento(randomDate);
    }
    /**
     * Devolve o número atual de utilizadores registados.
     *
     * @return número atual de utilizadores registados.
     */
    public static int getUserCounter(){
        userCounter++;
        return (userCounter-1);
    }

    /**
     * Devolve o número identificador do utilizador.
     *
     * @return email do utilizador.
     */
    public int getUserNum(){
        return this.userNum;
    }

    /**
     * Devolve o email do utilizador.
     *
     * @return email do utilizador.
     */
    public String getEmail(){
        return this.email;
    }

    /**
     * Devolve o nif do utilizador.
     *
     * @return nif do utilizador.
     */
    public String getNif(){
        return this.nif;
    }

    /**
     * Devolve a password do utilizador.
     *
     * @return password do utilizador.
     */
    public String getPassword(){
        return this.password;
    }

    /**
     * Devolve a morada do utilizador.
     *
     * @return morada do utilizador.
     */
    public String getMorada(){
        return this.morada;
    }

    /**
     * Devolve o nome do utilizador.
     *
     * @return nome do utilizador.
     */
    public String getNome(){
        return this.nome;
    }

    /**
     * Devolve a data de nascimento do utilizador.
     *
     * @return data de nascimento do utilizador.
     */
    public LocalDate getDataNascimento(){
        return this.dataNascimento;
    }

    /**
     * Atualiza o número identificador do utilizador.
     *
     * @param userNum novo número
     */
    public void setUserNum(int userNum){
        this.userNum = userNum;
    }

    /**
     * Atualiza o email do utilizador.
     *
     * @param email novo email
     */
    public void setEmail(String email){
        this.email = email;
    }

    /**
     * Atualiza o nif do utilizador.
     *
     * @param nif novo nif
     */
    public void setNif(String nif){
        this.nif = nif;
    }

    /**
     * Atualiza a password do utilizador.
     *
     * @param password nova password
     */
    public void setPassword(String password){
        this.password = password;
    }

    /**
     * Atualiza a morada do utilizador.
     *
     * @param morada nova morada
     */
    public void setMorada(String morada){
        this.morada = morada;
    }

    /**
     * Atualiza o nome do utilizador.
     *
     * @param nome novo nome
     */
    public void setNome(String nome){
        this.nome = nome;
    }

    /**
     * Atualiza a data de nascimento do utilizador.
     *
     * @param dataNascimento nova data de nascimento
     */
    public void setDataNascimento(LocalDate dataNascimento){
        this.dataNascimento = dataNascimento;
    }

    /**
     * Implementação do método de clonagem de um Utilizador
     *
     * @return objeto do tipo Utilizador
     */
    public Utilizador clone(){
        return new Utilizador(this);
    }

    /**
     * Implementação do método de igualdade entre dois utilizadores
     * Redefinição do método equals de Object.
     *
     //* @param  umUtilizador utilizador a ser comparado com o recetor
     * @return true ou false
     */
    public boolean equals(Object o){
        if (this == o)
            return true;

        if ((o == null) || (this.getClass() != o.getClass()))
            return false;

        Utilizador user = (Utilizador) o;
        return(this.email.equals(user.getEmail()) &&
                this.nif.equals(user.getNif()) &&
                this.password.equals(user.getPassword()) &&
                this.morada.equals(user.getMorada()) &&
                this.nome.equals(user.getNome()) &&
                this.dataNascimento.equals(user.getDataNascimento()));
    }

    /**
     * Implementação do método que calcula o código de hash do utilizador
     *
     * @return código hash do utilizador
     */
    public int hashCode(){
        return this.userNum;
    }
}

