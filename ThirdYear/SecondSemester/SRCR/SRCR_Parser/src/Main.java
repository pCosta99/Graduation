import model.Adjacencias;
import model.Paragens;
import parser.CSVReader;
import parser.ExcelReader;
import parser.FileRedirector;

import java.io.IOException;

public class Main {
    /* Path to the files we are interested in parsing */
    private static final String paragens_path = "E:\\UM\\3\\SRCR\\TP2\\SRCR\\SRCR_Parser\\src\\db\\paragem_autocarros_oeiras_processado_4.csv";
    private static final String adjacencias_path = "E:\\UM\\3\\SRCR\\TP2\\SRCR\\SRCR_Parser\\src\\db\\lista_adjacencias_paragens.xlsx";
    private static final String dotpl = "E:\\UM\\3\\SRCR\\TP2\\SRCR\\SRCR_Parser\\src\\db\\knowledge.pl";

    public static void main(String[] args) throws IOException {
        /* Init the data we want to fill */
        Paragens paragens = new Paragens();
        Adjacencias adjacencias = new Adjacencias();

        /* Call the parser to fill them up */
        CSVReader.parse_info_paragens(paragens, paragens_path);
        ExcelReader.parse_adjacencias(adjacencias, paragens, adjacencias_path);

        /* Redirect stdout to .pl file */
        FileRedirector.redirectToFile(dotpl);

        /* Flush data to .pl file */
        paragens.printParagens();
        adjacencias.printAdjacencias();
    }
}
