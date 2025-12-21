package com.example.gui;

import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.FlowPane;


public class GUIFlowPlane extends FlowPane
{
    public GUIFlowPlane(Label label, TextField textField, Button button)
    {
        super();

        setAlignment(Pos.CENTER);
        setPrefHeight(60);
        setVgap(10);
        setHgap(10);
        getChildren().addAll(label, textField, button);
    }
}
