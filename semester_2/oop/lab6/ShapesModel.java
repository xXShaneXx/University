
import Shape.Shape;

import java.util.ArrayList;
import java.util.Observable;

/**
 * This class extends Observable and represents a model for storing shapes.
 * It provides methods for adding, removing, and clearing shapes, as well as getting the list of shapes.
 * It also notifies observers when the list of shapes is changed.
 */
public class ShapesModel extends Observable
{
    private ArrayList<Shape> shapes; // The list of shapes

    /**
     * Constructor for ShapesModel
     * Initializes the list of shapes
     */
    public  ShapesModel()
    {
        shapes = new ArrayList<>();
    }

    /**
     * Returns the list of shapes
     * @return the list of shapes
     */
    public ArrayList<Shape> getShapes()
    {
        return shapes;
    }

    /**
     * Sets the list of shapes and notifies observers
     * @param shapes the new list of shapes
     */
    public void setShapes(ArrayList<Shape> shapes)
    {
        this.shapes = shapes;
        setChanged(); // Indicate that the model has changed
        notifyObservers(); // Notify observers
    }

    /**
     * Adds a shape to the list and notifies observers
     * @param shape the shape to add
     */
    public void addShape(Shape shape)
    {
        shapes.add(shape);
        setChanged(); // Indicate that the model has changed
        notifyObservers(); // Notify observers
    }

    /**
     * Removes a shape from the list and notifies observers
     * @param item the shape to remove
     */
    public void removeItem(Shape item)
    {
        shapes.remove(item);
        setChanged(); // Indicate that the model has changed
        notifyObservers(); // Notify observers
    }

    /**
     * Clears the list of shapes and notifies observers
     */
    public void clearShapes()
    {
        shapes.clear();
        setChanged(); // Indicate that the model has changed
        notifyObservers(); // Notify observers
    }
}
