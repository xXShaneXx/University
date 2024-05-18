
import Shape.Shape;
import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.scene.control.*;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.util.ArrayList;

public class ButtonController
{
    private final ArrayList<ToggleButton> figuresButtons;
    private final ToggleButton selectButton;
    private final ColorPicker colorPicker;
    private final Button clearButton;
    private final MenuButton menuButton;
    private final Stage primaryStage;
    private Shape.shapeType figureType;
    private ShapesModel shapesModel;

    public ButtonController(ArrayList<ToggleButton> figuresButtons, ToggleButton selectButton, Button clearButton, ColorPicker colorPicker,
                            MenuButton menuButton, Stage primaryStage)
    {
        this.figuresButtons = figuresButtons;
        this.selectButton = selectButton;
        this.clearButton = clearButton;
        this.colorPicker = colorPicker;
        this.menuButton = menuButton;
        this.primaryStage = primaryStage;

        initButtons();

        clearButton.setOnAction(this::clearShapes);
    }

    public void setShapesModel(ShapesModel shapesModel)
    {
        this.shapesModel = shapesModel;
    }

    public void initButtons()
    {
        ToggleGroup toggleGroup = new ToggleGroup();

        for (ToggleButton button : figuresButtons)
        {
            button.setToggleGroup(toggleGroup);
            button.setOnAction(new FigureSelectionHandler());
        }

        selectButton.setToggleGroup(toggleGroup);
        selectButton.setOnAction(actionEvent -> figureType = null);

        menuButton.getItems().get(1).setOnAction(e -> saveShapes());
        menuButton.getItems().get(0).setOnAction(e -> loadShapes());
    }

    private void saveShapes()
    {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Save File");
        File selectedFile = fileChooser.showSaveDialog(primaryStage);
        if (selectedFile != null)
            SavingManager.saveFiguresToFile(selectedFile, shapesModel.getShapes());
    }

    private void clearShapes(Event event)
    {
        shapesModel.clearShapes();
    }

    private void loadShapes()
    {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Open File");
        File selectedFile = fileChooser.showOpenDialog(primaryStage);
        if (selectedFile != null)
        {
            ArrayList<Shape> loadedShapes = SavingManager.loadFiguresFromFile(selectedFile);
            if (loadedShapes != null)
                shapesModel.setShapes(loadedShapes);
        }
    }

    public Shape.shapeType getFigureType()
    {
        return figureType;
    }

    public javafx.scene.paint.Color getColor()
    {
        return colorPicker.getValue();
    }

    private class FigureSelectionHandler implements EventHandler<ActionEvent>
    {
        @Override
        public void handle(ActionEvent event)
        {
            ToggleButton selectedButton = (ToggleButton) event.getSource();
            if (selectedButton.isSelected())
            {
                switch (selectedButton.getId())
                {
                    case "Circle":
                        figureType = Shape.shapeType.circle;
                        break;
                    case "Rectangle":
                        figureType = Shape.shapeType.rectangle;
                        break;
                    case "Triangle":
                        figureType = Shape.shapeType.triangle;
                        break;
                }
            }
            else
            {
                figureType = null;
            }
        }
    }
}
