package com.example.zadanie3;

import javafx.event.ActionEvent;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;

public class GUIButton extends Button
{
    public GUIButton(String name, TextField textField, TextArea textArea)
    {
        super(name);

        setOnAction((ActionEvent event) ->
        {
            textArea.setText(Launcher.launchExe(textField.getText()));
        });
    }
}
