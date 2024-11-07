
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

import java.io.*;

/**
 * This is an abstract class for a Shape. It implements Serializable interface to allow shapes to be saved and loaded.
 * It has properties for color, width, and height. It also has an enum for shape types.
 */
public abstract class Shape implements Serializable
{
    protected transient Color color; //transient preventing default serialization of color
    protected double w; //width of the shape
    protected double h; //height of the shape
    private static final long serialVersionUID = 1L;

    /**
     * Constructor for Shape
     * @param w width of the shape
     * @param h height of the shape
     * @param color color of the shape
     */
    public Shape(double w, double h, Color color)
    {
        this.w = w;
        this.h = h;
        this.color = color;
    }

    /**
     * Setter for color
     * @param color new color of the shape
     */
    public void setColor(Color color)
    {
        this.color = color;
    }

    /**
     * Calculates the distance between two points
     * @param x1 x-coordinate of the first point
     * @param y1 y-coordinate of the first point
     * @param x2 x-coordinate of the second point
     * @param y2 y-coordinate of the second point
     * @return the distance between the two points
     */
    public static double getDistanceBetweenTwoPoints(double x1,double y1,double x2,double y2)
    {
        return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
    }

    /**
     * Enum for shape types
     */
    public enum shapeType
    {
        rectangle,
        triangle,
        circle
    }

    /**
     * Factory method to create a Shape of a specific type
     * @param type type of the shape to create
     * @param x x-coordinate of the shape
     * @param y y-coordinate of the shape
     * @param color color of the shape
     * @return a new Shape of the specified type
     */
    public static Shape createShape(Shape.shapeType type, double x, double y, Color color)
    {
        switch (type)
        {
            case rectangle:
                return new RectangleFX(x, y, 80, 40, color);
            case triangle:
                return new TriangleFX(x, y, 70, 35 * Math.sqrt(3), color);
            case circle:
                double radius = 20.0;
                return new CircleFx(x - radius, y - radius, radius * 2, radius * 2, color);
            default:
                return null;
        }
    }

    /**
     * Custom serialization method
     * @param out ObjectOutputStream to write to
     * @throws IOException if an I/O error occurs
     */
    @Serial
    private void writeObject(ObjectOutputStream out) throws IOException
    {
        out.defaultWriteObject();
        out.writeDouble(color.getRed());
        out.writeDouble(color.getGreen());
        out.writeDouble(color.getBlue());
        out.writeDouble(color.getOpacity());
    }

    /**
     * Custom deserialization method
     * @param in ObjectInputStream to read from
     * @throws IOException if an I/O error occurs
     * @throws ClassNotFoundException if the class of a serialized object cannot be found
     */
    @Serial
    private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException
    {
        in.defaultReadObject();
        double red = in.readDouble();
        double green = in.readDouble();
        double blue = in.readDouble();
        double opacity = in.readDouble();
        this.color = new Color(red, green, blue, opacity);
    }

    /**
     * Abstract method to check if a point is within the shape
     * @param x x-coordinate of the point
     * @param y y-coordinate of the point
     * @return true if the point is within the shape, false otherwise
     */
    public abstract boolean isHit(double x, double y);

    /**
     * Abstract method to move the shape
     * @param mouseX new x-coordinate of the shape
     * @param mouseY new y-coordinate of the shape
     */
    public abstract void move(double mouseX, double mouseY);

    /**
     * Abstract method to scale the shape
     * @param scaleStep factor to scale the shape by
     */
    public abstract void scale(double scaleStep);

    /**
     * Abstract method to select the shape
     * @param gc GraphicsContext to draw on
     */
    public abstract void select(GraphicsContext gc);

    /**
     * Abstract method to draw the shape
     * @param gc GraphicsContext to draw on
     */
    public abstract void draw(GraphicsContext gc);

    /**
     * Abstract method to rotate the shape
     * @param angle angle to rotate the shape by
     */
    public abstract void rotate(double angle);

}
