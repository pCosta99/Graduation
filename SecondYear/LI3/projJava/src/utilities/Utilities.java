package utilities;

public class Utilities {
    /**
     * Cria um caminho de um ficheiro para um dado sistema operativo
     * @param path : Caminho para fazer append
     * @param folder : pasta para fazer append
     * @param file : ficheiro para fazer append
     * @return : caminho absoluto
     */
    public static String makePath(String path, String folder, String file){
        String[] os = System.getProperty("os.name").split(" ");
        String fullPath = null;
        switch (os[0]){
            case "Windows" : fullPath = path + "\\" + folder + "\\" + file;
                break;
            default     : fullPath = path + "/" + folder + "/" + file;
                break;
        }
        return fullPath;
    }

    public static String makePath(String path, String folder){
        String[] os = System.getProperty("os.name").split(" ");
        String fullPath = null;
        switch (os[0]){
            case "Windows" : fullPath = path + "\\" + folder + "\\";
                break;
            default     : fullPath = path + "/" + folder + "/";
                break;
        }
        return fullPath;
    }

    public static String makePathToFile(String path, String file){
        String[] os = System.getProperty("os.name").split(" ");
        String fullPath = null;
        switch (os[0]){
            case "Windows" : fullPath = path + "\\" + file;
                break;
            default     : fullPath = path + "/" + file;
                break;
        }
        return fullPath;
    }
}
