package exceptions;

public class InvalidMonthException extends Exception {
    public InvalidMonthException(){
        super("O mês inserido não é válido.");
    }
    public InvalidMonthException(String errorMessage) {
        super(errorMessage);
    }
}
