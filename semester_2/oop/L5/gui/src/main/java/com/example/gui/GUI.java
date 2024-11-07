package com.example.gui;

import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.Region;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class GUI extends Application
{

    @Override
    public void start(Stage primaryStage)
    {
        Label inputL = new Label("Enter height of Pascal Triangle");

        TextField inputTF = new TextField();

        VBox outputVB = new VBox();
        outputVB.setMinWidth(Region.USE_PREF_SIZE); //set width of it's content's(parent) width
        outputVB.setAlignment(Pos.CENTER);

        ScrollPane scrollPane = new ScrollPane(outputVB);
        scrollPane.setFitToHeight(true);
        scrollPane.setFitToWidth(true);

        Button drawB = new GUIButton("Draw Triangle", inputTF, outputVB);

        FlowPane menuFP = new GUIFlowPlane(inputL, inputTF, drawB);

        BorderPane root = new BorderPane();
        root.setTop(menuFP);
        root.setCenter(scrollPane);

        Scene scene = new Scene(root, 600, 400);
        scene.getStylesheets().add("/Stylesheet.css");
        outputVB.getStyleClass().add("vBox");

        primaryStage.setScene(scene);
        primaryStage.setTitle("Pascal Triangle Gui");
        primaryStage.show();
    }

    public static void main(String[] args)
    {
        launch(args);
    }
}
