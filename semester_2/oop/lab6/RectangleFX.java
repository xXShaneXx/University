
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

import java.io.Serializable;

/**
 * This class extends the Shape class and implements Serializable interface.
 * It represents a rectangle that can be drawn on a JavaFX canvas.
 * It has methods for checking if a point is within the rectangle, moving the rectangle, scaling the rectangle, selecting the rectangle, drawing the rectangle, and rotating the rectangle.
 */
public class RectangleFX extends Shape implements Serializable
{
    private final SerializablePolygon rectangle; // The polygon that represents the rectangle

    /**
     * Constructor for RectangleFX
     * @param x the x-coordinate of the top left corner of the rectangle
     * @param y the y-coordinate of the top left corner of the rectangle
     * @param w the width of the rectangle
     * @param h the height of the rectangle
     * @param color the color of the rectangle
     */
    public RectangleFX(double x, double y, double w, double h, Color color)
    {
        super(w, h, color);
        rectangle = new SerializablePolygon(x, y, x + w, y, x + w, y + h, x, y + h);
    }

    /**
     * Checks if a point is within the rectangle
     * @param x the x-coordinate of the point
     * @param y the y-coordinate of the point
     * @return true if the point is within the rectangle, false otherwise
     */
    @Override
    public boolean isHit(double x, double y)
    {
        return rectangle.contains(x, y);
    }

    /**
     * Moves the rectangle to a new location
     * @param mouseX the new x-coordinate of the rectangle
     * @param mouseY the new y-coordinate of the rectangle
     */
    @Override
    public void move(double mouseX, double mouseY)
    {
        double deltaX = mouseX - rectangle.getCenterX();
        double deltaY = mouseY - rectangle.getCenterY();

        rectangle.move(deltaX, deltaY);
    }

    /**
     * Scales the rectangle by a given scale step
     * @param scaleStep the scale step to scale the rectangle by
     */
    @Override
    public void scale(double scaleStep)
    {
        double scaleFactor = 1 + scaleStep * 0.015;
        double newWidth = w * scaleFactor;
        double newHeight = h * scaleFactor;

        if (newWidth >= 2.0 && newHeight >= 2.0)
        {
            double scaleX = newWidth / w;
            double scaleY = newHeight / h;
            rectangle.scale(scaleX, scaleY);
            w = newWidth;
            h = newHeight;
        }
    }

    /**
     * Selects the rectangle by drawing a red border around it
     * @param gc the GraphicsContext to draw on
     */
    @Override
    public void select(GraphicsContext gc)
    {
        gc.setStroke(Color.RED);
        gc.setLineWidth(2);
        gc.strokePolygon(rectangle.getPointsX(), rectangle.getPointsY(), 4);

        gc.setStroke(Color.BLACK);
        gc.setLineWidth(1);
    }

    /**
     * Draws the rectangle on a GraphicsContext
     * @param gc the GraphicsContext to draw on
     */
    @Override
    public void draw(GraphicsContext gc)
    {
        gc.setFill(color);
        gc.fillPolygon(rectangle.getPointsX(), rectangle.getPointsY(), 4);
    }

    /**
     * Rotates the rectangle by a given angle
     * @param angle the angle to rotate the rectangle by
     */
    @Override
    public void rotate(double angle)
    {
        rectangle.rotate(angle);
    }
}
