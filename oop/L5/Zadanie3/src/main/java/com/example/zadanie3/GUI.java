package com.example.zadanie3;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.FlowPane;
import javafx.stage.Stage;

public class GUI extends Application
{

    @Override
    public void start(Stage primaryStage)
    {
        Label inputL = new Label("Enter parameters of the program");

        TextField inputTF = new TextField();

        TextArea outputTA = new TextArea();
        outputTA.setEditable(false);

        Button drawB = new GUIButton("Launch cpp", inputTF, outputTA);

        FlowPane menuFP = new GUIFlowPlane(inputL, inputTF, drawB);

        BorderPane root = new BorderPane();
        root.setTop(menuFP);
        root.setCenter(outputTA);

        Scene scene = new Scene(root, 600, 400);
        scene.getStylesheets().add("/Stylesheet.css");

        primaryStage.setScene(scene);
        primaryStage.setTitle("Pascal Triangle Gui");
        primaryStage.show();
    }

    public static void main(String[] args)
    {
        launch(args);
    }
}