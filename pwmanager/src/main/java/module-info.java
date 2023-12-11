module sacuzel.personal {
    requires javafx.controls;
    requires javafx.fxml;

    opens sacuzel.personal to javafx.fxml;
    exports sacuzel.personal;
}
