
import javafx.geometry.Point2D;
import javafx.scene.shape.Polygon;
import javafx.scene.transform.Rotate;
import javafx.scene.transform.Scale;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * This class extends the Polygon class and implements Serializable interface.
 * It represents a polygon that can be serialized and deserialized.
 * It has methods for moving, scaling, and rotating the polygon.
 */

public class SerializablePolygon extends Polygon implements Serializable
{
    @Serial
    private static final long serialVersionUID = 1L;
    private double rotationAngle;

    /**
     * Constructor for SerializablePolygon
     * @param coordinates the x and y coordinates of the polygon's vertices
     */
    public SerializablePolygon(double... coordinates)
    {
        super(coordinates);
        this.rotationAngle = 0;
    }

    /**
     * Getter for the x-coordinates of the polygon's vertices
     * @return an array of x-coordinates
     */
    public double[] getPointsX()
    {
        double[] pointsX = new double[getPoints().size() / 2];

        for (int i = 0; i < getPoints().size(); i += 2)
        {
            pointsX[i / 2] = getPoints().get(i);
        }

        return pointsX;
    }

    /**
     * Getter for the y-coordinates of the polygon's vertices
     * @return an array of y-coordinates
     */
    public double[] getPointsY()
    {
        double[] pointsY = new double[getPoints().size() / 2];

        for (int i = 1; i < getPoints().size(); i += 2)
        {
            pointsY[i / 2] = getPoints().get(i);
        }

        return pointsY;
    }

    /**
     * Moves the polygon by a given delta x and delta y
     * @param deltaX the amount to move in the x direction
     * @param deltaY the amount to move in the y direction
     */
    public void move(double deltaX, double deltaY)
    {
        for (int i = 0; i < getPoints().size(); i += 2)
        {
            double x = getPoints().get(i) + deltaX;
            double y = getPoints().get(i + 1) + deltaY;
            getPoints().set(i, x);
            getPoints().set(i + 1, y);
        }
    }

    /**
     * Scales the polygon by a given scale factor in the x and y directions
     * @param scaleX the scale factor in the x direction
     * @param scaleY the scale factor in the y direction
     */
    public void scale(double scaleX, double scaleY)
    {
        Scale scale = new Scale(scaleX, scaleY, getCenterX(), getCenterY());
        this.getTransforms().add(scale);

        List<Double> scaledPoints = new ArrayList<>();
        for (int i = 0; i < getPoints().size(); i += 2)
        {
            double x = getPoints().get(i);
            double y = getPoints().get(i + 1);
            Point2D scaledPoint = scale.transform(x, y);
            scaledPoints.add(scaledPoint.getX());
            scaledPoints.add(scaledPoint.getY());
        }
        getPoints().setAll(scaledPoints);
    }

    /**
     * Rotates the polygon by a given angle
     * @param angle the angle to rotate the polygon by
     */
    public void rotate(double angle)
    {
        this.rotationAngle += angle;
        Rotate rotate = new Rotate(angle, getCenterX(), getCenterY());//
        this.getTransforms().add(rotate);

        List<Double> rotatedPoints = new ArrayList<>();
        for (int i = 0; i < getPoints().size(); i += 2)
        {
            double x = getPoints().get(i);
            double y = getPoints().get(i + 1);
            Point2D rotatedPoint = rotate.transform(x, y);
            rotatedPoints.add(rotatedPoint.getX());
            rotatedPoints.add(rotatedPoint.getY());
        }
        getPoints().setAll(rotatedPoints);
    }

    /**
     * Getter for the center x-coordinate of the polygon
     * @return the center x-coordinate
     */
    public double getCenterX()
    {
        double sum = 0;
        List<Double> points = this.getPoints();
        for (int i = 0; i < points.size(); i += 2)
        {
            sum += points.get(i);
        }

        return sum / (points.size() / 2);
    }

    /**
     * Getter for the center y-coordinate of the polygon
     * @return the center y-coordinate
     */
    public double getCenterY()
    {
        double sum = 0;
        List<Double> points = this.getPoints();
        for (int i = 1; i < points.size(); i += 2)
        {
            sum += points.get(i);
        }

        return sum / (points.size() / 2);
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
        List<Double> points = this.getPoints();
        out.writeInt(points.size());
        for (Double point : points)
        {
            out.writeDouble(point);
        }
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
        int size = in.readInt();
        List<Double> points = new ArrayList<>(size);
        for (int i = 0; i < size; i++)
        {
            points.add(in.readDouble());
        }
        this.getPoints().setAll(points);

        if (rotationAngle != 0)
        {
            Rotate rotate = new Rotate(rotationAngle, getCenterX(), getCenterY());
            this.getTransforms().add(rotate);
        }
    }
}
