import java.io.Serializable;

public class Exception_ClienteInexistente extends Exception implements Serializable {
    public Exception_ClienteInexistente()
    {
        super();
    }

    public Exception_ClienteInexistente(String s)
    {
        super(s);
    }
}
