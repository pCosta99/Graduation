import java.io.Serializable;

public class Exception_PasswordErrada extends Exception implements Serializable {
    public Exception_PasswordErrada()
    {
        super();
    }

    public Exception_PasswordErrada(String s)
    {
        super(s);
    }
}
