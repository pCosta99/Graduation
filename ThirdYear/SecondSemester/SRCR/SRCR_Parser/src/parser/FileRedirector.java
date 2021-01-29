package parser;

import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.file.Paths;

public class FileRedirector {
    public static void redirectToFile(String path) throws IOException {
        /* If the query save file doesn't exists, create it */
        File file =  new File(path);
        file.createNewFile();

        // Creating a File object that represents the disk file.
        PrintStream o = new PrintStream(file);

        // Assign o to output stream
        System.setOut(o);

    }
}
