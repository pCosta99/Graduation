package parser;

import model.Paragem;
import model.Paragens;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.concurrent.atomic.AtomicInteger;

public class CSVReader {
    public static void parse_info_paragens(Paragens paragens, String path_to_paragens) {
        AtomicInteger counter = new AtomicInteger();
        try {
            Files.lines(Paths.get(path_to_paragens)).skip(1).map(l -> l.split(";")).forEach(array -> {
                Paragem p = new Paragem();
                for(int i = 0; i < array.length; i++) {
                    if (i == 0 && array[i].length() > 0) p.setGid(Integer.parseInt(array[i]));
                    if (i == 1 && array[i].length() > 0) p.setLatitude(Float.parseFloat(array[i]));
                    if (i == 2 && array[i].length() > 0) p.setLongitude(Float.parseFloat(array[i]));
                    if (i == 3 && array[i].length() > 0) p.setEstadoDeConservacao(array[i]);
                    if (i == 4 && array[i].length() >0) p.setTipoDeAbrigo(array[i]);
                    if (i == 5 && array[i].length() > 0 && array[i].toLowerCase().equals("yes")) p.setComPublicidade(true);
                    else if(i == 5 && array[i].length() > 0) p.setComPublicidade(false);
                    if (i == 6 && array[i].length() > 0) p.setOperadora(array[i]);
                    if (i == 7 && array[i].length() > 0) p.setCarreira(array[i].split(","));
                    if (i == 8 && array[i].length() > 0) p.setCodigoDeRua(Integer.parseInt(array[i]));
                    if (i == 9 && array[i].length() > 0) p.setNomeDaRua(array[i]);
                    if (i == 10 && array[i].length() > 0) p.setFreguesia(array[i]);
                    paragens.addParagem(p);
                    counter.getAndIncrement();
                }
            });
        } catch (Exception e) {
            System.out.println("Blew up on line " + counter);
            System.out.println(e.getMessage());
        }
    }
}
