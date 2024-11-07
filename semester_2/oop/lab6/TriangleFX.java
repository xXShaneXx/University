
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

import java.io.Serializable;

public class TriangleFX extends Shape implements Serializable
{
    private final SerializablePolygon triangle;

    public TriangleFX(double x, double y, double w, double h, Color color)
    {
        super(w, h, color);
        triangle = new SerializablePolygon(x, y, x + w / 2, y - h, x + w, y);
    }


    @Override
    public boolean isHit(double x, double y)
    {
        return triangle.contains(x, y);
    }

    @Override
    public void move(double mouseX, double mouseY)
    {
        double deltaX = mouseX - triangle.getCenterX();
        double deltaY = mouseY - triangle.getCenterY();

        triangle.move(deltaX, deltaY);
    }

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
            triangle.scale(scaleX, scaleY);
            w = newWidth;
            h = newHeight;
        }
    }

    @Override
    public void select(GraphicsContext gc)
    {
        gc.setStroke(Color.RED);
        gc.setLineWidth(2);
        gc.strokePolygon(triangle.getPointsX(), triangle.getPointsY(), triangle.getPoints().size() / 2);

        gc.setStroke(Color.BLACK);
        gc.setLineWidth(1);
    }

    @Override
    public void draw(GraphicsContext gc)
    {
        gc.setFill(color);
        gc.fillPolygon(triangle.getPointsX(), triangle.getPointsY(), triangle.getPoints().size() / 2);
    }

    @Override
    public void rotate(double angle)
    {
        triangle.rotate(angle);
    }
}
