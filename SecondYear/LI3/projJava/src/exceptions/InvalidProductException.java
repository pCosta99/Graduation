package exceptions;

public class InvalidProductException extends Exception {
    public InvalidProductException(){
        super("O código de produto inserido não é válido.");
    }
    public InvalidProductException(String errorMessage) {
        super(errorMessage);
    }
}
