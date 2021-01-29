package exceptions;

public class InvalidCustomerException extends Exception{
    public InvalidCustomerException(){
        super("O código de cliente inserido não é válido.");
    }
    public InvalidCustomerException(String errorMessage) {
        super(errorMessage);
    }
}
