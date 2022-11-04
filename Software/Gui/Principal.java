import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

public class Principal extends Application {
    static Stage stage;

    @Override
    public void start(Stage primaryStage) throws Exception {
        Stage stage = Principal.stage = primaryStage;
        Parent root = FXMLLoader.load(getClass().getResource("src/gui.fxml"));
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.getIcons().add(new Image("file:Software/Gui/src/robot.png"));
        stage.initStyle(StageStyle.UNDECORATED);
        stage.setFullScreen(true);
        stage.setFullScreenExitHint("");
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
