
import javafx.event.Event;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.*;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Polygon;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;

import java.util.ArrayList;

public class Menu extends FlowPane
{
    private ToggleButton selectButton;
    private Button clearButton;
    private ArrayList<ToggleButton> figuresButtons;
    private MenuButton fileMenuButton;
    private ColorPicker colorPicker;
    private Button infoButton;

    public Menu()
    {
        super();
        setAlignment(Pos.TOP_LEFT);
        setPrefHeight(50);
        setPadding(new Insets(0, 0, 0, 0));
        setHgap(10.0);

        addButtons();

        colorPicker = new ColorPicker();
        colorPicker.setMinHeight(25);

        HBox figuresMenu = new HBox(10);
        figuresMenu.getChildren().addAll(figuresButtons);
        figuresMenu.setMinHeight(80);
        figuresMenu.setMaxHeight(80);

        HBox selectClearMenu = new HBox(10);
        selectClearMenu.getChildren().addAll(selectButton, clearButton);
        selectClearMenu.setMinHeight(80);
        selectClearMenu.setMaxHeight(80);

        VBox colorSelectMenu = new VBox(10);
        colorSelectMenu.getChildren().addAll(colorPicker, selectClearMenu);
        colorSelectMenu.setMinHeight(80);
        colorSelectMenu.setMaxHeight(80);

        fileMenuButton = new MenuButton("File");
        MenuItem openMenuItem = new MenuItem("Open");
        MenuItem saveMenuItem = new MenuItem("Save");
        fileMenuButton.getItems().addAll(openMenuItem, saveMenuItem);

        infoButton = new Button("Info");
        infoButton.setMinWidth(fileMenuButton.getWidth());

        infoButton.setOnAction(this::handleInfoButtonClick);

        HBox fileAndInfoButtons = new HBox(2);
        fileAndInfoButtons.setAlignment(Pos.TOP_LEFT);
        fileAndInfoButtons.setPrefHeight(50);
        fileAndInfoButtons.setPadding(new Insets(0, 0, 0, 0));
        fileAndInfoButtons.getChildren().addAll(fileMenuButton, infoButton);

        FlowPane buttonsLayout = new FlowPane();
        buttonsLayout.setAlignment(Pos.TOP_LEFT);
        buttonsLayout.setPrefHeight(50);
        buttonsLayout.setPadding(new Insets(10, 0, 0, 10));
        buttonsLayout.setHgap(10.0);
        buttonsLayout.getChildren().addAll(figuresMenu, colorSelectMenu);

        VBox mainMenu = new VBox(2);
        mainMenu.setAlignment(Pos.TOP_LEFT);
        mainMenu.setPadding(new Insets(0, 0, 0, 0));
        mainMenu.getChildren().addAll(fileAndInfoButtons, buttonsLayout);

        getChildren().add(mainMenu);
    }

    private void addButtons()
    {
        figuresButtons = new ArrayList<>();

        ToggleButton circleButton = new ToggleButton();
        circleButton.setId("Circle");
        circleButton.setGraphic(createCircleShape(25));
        circleButton.setMinSize(60, 60);
        circleButton.setMaxSize(60, 60);

        ToggleButton triangleButton = new ToggleButton();
        triangleButton.setId("Triangle");
        triangleButton.setGraphic(createTriangleShape(45));
        triangleButton.setMinSize(60, 60);
        triangleButton.setMaxSize(60, 60);

        ToggleButton rectangleButton = new ToggleButton();
        rectangleButton.setId("Rectangle");
        rectangleButton.setGraphic(createRectangleShape(45, 30));
        rectangleButton.setMinSize(60, 60);
        rectangleButton.setMaxSize(60, 60);

        figuresButtons.add(circleButton);
        figuresButtons.add(triangleButton);
        figuresButtons.add(rectangleButton);

        selectButton = new ToggleButton("Select");
        clearButton = new Button("Clear");
    }

    // Method to create a circle shape
    private Circle createCircleShape(double radius)
    {
        Circle circle = new Circle(radius);
        circle.setFill(Color.TRANSPARENT);
        circle.setStroke(Color.BLACK);
        return circle;
    }

    // Method to create a triangle shape
    private Polygon createTriangleShape(double size)
    {
        Polygon triangle = new Polygon();
        triangle.getPoints().addAll(0.0, size, size, size, size / 2, 0.0);
        triangle.setFill(Color.TRANSPARENT);
        triangle.setStroke(Color.BLACK);
        return triangle;
    }

    // Method to create a rectangle shape
    private Rectangle createRectangleShape(double width, double height)
    {
        Rectangle rectangle = new Rectangle(width, height);
        rectangle.setFill(Color.TRANSPARENT);
        rectangle.setStroke(Color.BLACK);
        return rectangle;
    }

    public ButtonController createButtonController(Stage primaryStage)
    {
        return new ButtonController(figuresButtons, selectButton, clearButton , colorPicker, fileMenuButton, primaryStage);
    }

    private void handleInfoButtonClick(Event event)
    {
        Alert infoDialog = new Alert(Alert.AlertType.INFORMATION);
        infoDialog.setTitle("Information");
        infoDialog.setHeaderText("Paint Application");
        infoDialog.setContentText(
                "Welcome to the Paint Application, made by Paweł Grzegory!\n\n" +
                        "This application allows you to create and manipulate shapes with ease.\n\n" +
                        "Here's how to use it:\n" +
                        "- To create a figure, first, select the desired figure from the menu. Then, press the left mouse button to place the figure on the canvas.\n" +
                        "- To select a figure that's already on the canvas, click the 'Select' button and then click on the desired figure.\n" +
                        "- To delete a figure, select the desired figure. Then press delete button on keyboard.\n" +
                        "- You can change the color of a selected figure by choosing a color from the palette and right-clicking on the selected figure.\n" +
                        "- To resize a selected figure, use the scroll wheel on your mouse.\n" +
                        "- To rotate a figure, hold down the 'Ctrl' key and use the scroll wheel on your mouse.\n" +
                        "- To clear the canvas(remove all figures), click clear button.\n" +
                        "- To save a file, select file next click save.\n" +
                        "- To open a file, select file next click open and choose the file.\n\n" +
                        "Enjoy unleashing your creativity with the Paint Application!"
        );
        infoDialog.showAndWait();
    }
}
