import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Scanner;

public class Main {
    public static void main (String[] args) {
        try {Scanner keyboard = new Scanner(System.in);
            gestaoGeral g = null;
            System.out.println("1-carregar Ficheiro estado ou 2 - Ler de uma log file ");
            String fich = keyboard.nextLine();
            if (fich.equals("1")) {
                g = gestaoGeral.carregaEstado("src/estado.bak");
            }else{
                String file = "src/logs.bak";
                BufferedReader br = new BufferedReader(new FileReader(file));
                g = gestaoGeral.fromLogFile(br);
            }
            Menu.menu(g);
        }catch (Exception e){
            e.printStackTrace();
        }

    }
}