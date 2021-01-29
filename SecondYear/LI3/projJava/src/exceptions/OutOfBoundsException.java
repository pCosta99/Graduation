package exceptions;

public class OutOfBoundsException extends Exception {
    public OutOfBoundsException(){
        super("O valor inserido não é válido.");
    }
    public OutOfBoundsException(String errorMessage) {
        super(errorMessage);
    }
}
