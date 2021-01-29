import controller.GestVendasController;
import controller.IGestVendasController;

import java.io.IOException;
import java.util.Arrays;

public class GestVendasAppMVC {
    public static void main(String[] args) throws IOException {
        IGestVendasController controller;
        if(args.length == 0){
            controller = new GestVendasController();
        } else {
            controller = new GestVendasController(args[0]);
        }

        controller.startController();
    }
}
