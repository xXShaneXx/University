
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

import java.io.IOException;

public class MainGui extends Application
{
    /**
     * Project mady by
     @author Paweł Grzegory
     */

    @Override
    public void start(Stage stage) throws IOException
    {
        BorderPane root = new BorderPane();

        Menu menu = new Menu();
        root.setTop(menu);

        ShapesModel shapesModel = new ShapesModel();

        ButtonController buttonController = menu.createButtonController(stage);
        buttonController.setShapesModel(shapesModel);

        PaintPlane paintPane = new PaintPlane(600, 400, shapesModel, buttonController);
        root.setCenter(paintPane);

        // Add a resize listener to update PaintPane size
        stage.widthProperty().addListener((observable, oldValue, newValue) ->
        {
            double newWidth = newValue.doubleValue();
            paintPane.setWidth(newWidth);
        });

        stage.heightProperty().addListener((observable, oldValue, newValue) ->
        {
            double newHeight = newValue.doubleValue();
            paintPane.setHeight(newHeight);
        });

        Scene scene = new Scene(root, 600, 400);

        stage.setScene(scene);
        stage.setTitle("Paint");
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }
}
