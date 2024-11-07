
import Shape.Shape;
import java.io.*;
import java.util.ArrayList;

/**
 * This class provides methods for saving and loading shapes to and from a file.
 * It uses Java's built-in serialization mechanism to write and read objects.
 */
public class SavingManager
{
    /**
     * Saves a list of shapes to a file.
     * @param file the file to save the shapes to
     * @param shapes the list of shapes to save
     */
    public static void saveFiguresToFile(File file, ArrayList<Shape> shapes)
    {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(file)))
        {
            oos.writeObject(shapes);
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }

    /**
     * Loads a list of shapes from a file.
     * @param file the file to load the shapes from
     * @return the list of shapes loaded from the file
     */
    public static ArrayList<Shape> loadFiguresFromFile(File file)
    {
        ArrayList<Shape> s = new ArrayList<>();
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file)))
        {
            Object obj = ois.readObject();
            if (obj instanceof ArrayList<?>)
            {
                s.addAll((ArrayList<? extends Shape>) obj);
            }
        }
        catch (IOException | ClassNotFoundException e)
        {
            e.printStackTrace();
        }
        return s;
    }
}
