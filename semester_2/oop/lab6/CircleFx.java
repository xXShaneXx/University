
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

import java.io.Serial;
import java.io.Serializable;

public class CircleFx extends Shape implements Serializable
{
    @Serial
    private static final long serialVersionUID = 1L;
    private double x;
    private double y;
    public CircleFx(double x, double y, double w, double h, Color color)
    {
        super(w, h, color);
        this.x = x;
        this.y = y;
    }

    @Override
    public boolean isHit(double hitX, double hitY)
    {
        double radius = w / 2;
        double centerX = x + radius;
        double centerY = y + radius;
        double distanceHitFromCenter = Shape.getDistanceBetweenTwoPoints(centerX, centerY, hitX, hitY);
        return distanceHitFromCenter <= radius;
    }

    @Override
    public void move(double mouseX, double mouseY)
    {
        double deltaX = mouseX - x;
        double deltaY = mouseY - y;

        x += deltaX - w / 2;
        y += deltaY - h / 2;
    }

    @Override
    public void scale(double scaleStep)
    {
        // Calculate the new width and height
        double newWidth = w + scaleStep;
        double newHeight = h + scaleStep;

        if(newWidth >= 2.0)
        {
            // Calculate the amount to move the circle to keep its center fixed
            double deltaX = (newWidth - w) / 2;
            double deltaY = (newHeight - h) / 2;

            // Update the position and size of the circle
            x -= deltaX;
            y -= deltaY;
            w = newWidth;
            h = newHeight;
        }
    }

    @Override
    public void select(GraphicsContext gc)
    {
        gc.setStroke(Color.RED);
        gc.setLineWidth(2);
        gc.strokeOval(x, y, w, h);

        gc.setStroke(Color.BLACK);
        gc.setLineWidth(1);
    }

    @Override
    public void draw(GraphicsContext gc)
    {
        gc.setFill(color);
        gc.fillOval(x, y, w, h);
    }

    @Override
    public void rotate(double angle)
    {

    }
}
