
import Shape.Shape;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.input.*;

import java.util.Observable;
import java.util.Observer;


/**
 * This class extends the Canvas class and implements the Observer interface.
 * It represents a paint plane where shapes can be drawn, selected, moved, and scaled.
 * It also handles mouse and keyboard events.
 */
public class PaintPlane extends Canvas implements Observer
{
    private final GraphicsContext gc;
    private final ShapesModel shapesModel;
    private Shape selectedShape;
    private final ButtonController buttonController;

    /**
     * Constructor for PaintPlane
     * @param width the width of the canvas
     * @param height the height of the canvas
     * @param shapesModel the model that stores the shapes
     * @param buttonController the controller for the buttons
     */
    public PaintPlane(double width, double height, ShapesModel shapesModel, ButtonController buttonController)
    {
        super(width, height);
        this.gc = this.getGraphicsContext2D();
        this.shapesModel = shapesModel;
        this.buttonController = buttonController;

        shapesModel.addObserver(this);

        this.addEventHandler(MouseEvent.MOUSE_CLICKED, this::handleMouseClicked);
        this.addEventHandler(MouseEvent.MOUSE_DRAGGED, this::handleMouseDragged);
        this.addEventHandler(ScrollEvent.SCROLL, this::handleScroll);
        this.addEventHandler(KeyEvent.KEY_PRESSED, this::handleDelPressed);

        // Make canvas focusable
        setFocusTraversable(true);
    }

    private void handleMouseClicked(MouseEvent event)
    {
        // Request focus on canvas when it is clicked
        requestFocus();

        double x = event.getX();
        double y = event.getY();

        if (event.getButton() == MouseButton.PRIMARY)
            handlePrimaryClick(x, y);
        else if (event.getButton() == MouseButton.SECONDARY)
            handleSecondaryClick();
    }

    private void handlePrimaryClick(double x, double y)
    {
        Shape.shapeType figureType = buttonController.getFigureType();
        if (figureType != null)
            createNewShape(figureType, x, y);
        else
            selectShapeAt(x, y);
    }

    private void handleSecondaryClick()
    {
        if (selectedShape != null)
        {
            selectedShape.setColor(buttonController.getColor());
            redraw();
        }
    }

    private void createNewShape(Shape.shapeType type, double x, double y)
    {
        Shape newShape = Shape.createShape(type, x, y, buttonController.getColor());
        if (newShape != null)
        {
            shapesModel.addShape(newShape);
            selectedShape = null;
            redraw();
        }
    }

    private void selectShapeAt(double x, double y)
    {
        selectedShape = findShapeAt(x, y);
        if (selectedShape != null)
            moveShapeToTop(selectedShape);
        redraw();
    }

    private Shape findShapeAt(double x, double y)
    {
        for (Shape shape : shapesModel.getShapes())
        {
            if (shape.isHit(x, y))
                return shape;
        }
        return null;
    }

    private void moveShapeToTop(Shape shape)
    {
        shapesModel.getShapes().remove(shape);
        shapesModel.getShapes().add(shape);
    }

    private void handleScroll(ScrollEvent event)
    {
        if (selectedShape != null)
        {
            double delta = event.getDeltaY();
            if (event.isControlDown())
                selectedShape.rotate(delta * 0.7);
            else
                selectedShape.scale(delta * 0.7);
            redraw();
        }
    }

    private void handleMouseDragged(MouseEvent event)
    {
        if (selectedShape != null && event.getButton() == MouseButton.PRIMARY)
        {
            double deltaX = event.getX();
            double deltaY = event.getY();
            selectedShape.move(deltaX, deltaY);
            redraw();
        }
    }

    private void handleDelPressed(KeyEvent event)
    {
        if(selectedShape != null && event.getCode() == KeyCode.DELETE)
        {
            shapesModel.removeItem(selectedShape);
            selectedShape = null;
        }

    }

    private void redraw()
    {
        gc.clearRect(0, 0, getWidth(), getHeight());
        for (Shape shape : shapesModel.getShapes())
        {
            shape.draw(gc);
        }

        if(selectedShape != null)
            selectedShape.select(gc);
    }

    /**
     * This method is called when the observed object is changed.
     * It deselects the currently selected shape and redraws the canvas.
     * @param observable the observed object
     * @param o an argument passed to the notifyObservers method
     */
    @Override
    public void update(Observable observable, Object o)
    {
        selectedShape = null;
        redraw();
    }
}
