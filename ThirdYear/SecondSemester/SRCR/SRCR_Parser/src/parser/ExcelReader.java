package parser;

import model.*;
import org.apache.poi.openxml4j.exceptions.InvalidFormatException;
import org.apache.poi.ss.usermodel.*;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class ExcelReader {
    public static void parse_adjacencias(Adjacencias adjacencias, Paragens paragens, String adjacencias_path){
        try {
            boolean ignoreHeader = true;
            XSSFWorkbook workbook = new XSSFWorkbook(new File(adjacencias_path));
            DataFormatter formatter = new DataFormatter();
            /* Iterate over sheets */
            for (Sheet sheet : workbook) {
                /* Parse the paragens into a list to later make the correct pairs */
                List<Integer> gids = new ArrayList<>();
                String carreira = sheet.getSheetName();
                for (Row row : sheet) {
                    /* Ignore the header or grab the gid */
                    if(ignoreHeader) ignoreHeader = false;
                    else gids.add(Integer.valueOf(formatter.formatCellValue(row.getCell(0))));
                }
                for(int i = 0; i < gids.size()-1; i++){
                    Adjacencia adj = new Adjacencia(paragens.getParagem(gids.get(i)), paragens.getParagem(gids.get(i+1)), carreira);
                    adjacencias.addAdjacencia(carreira, adj);
                }
                ignoreHeader = true;
            }
            workbook.close();
        } catch (IOException | InvalidFormatException e) {
            e.printStackTrace();
            System.out.println("Excel file not found!");
        }
    }
}
