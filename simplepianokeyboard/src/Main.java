import com.fazecast.jSerialComm.SerialPort;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.stage.Stage;

import java.util.Arrays;
import java.util.concurrent.atomic.AtomicInteger;

public class Main extends Application {
    private static SerialPort serialPort;
    private static final KeyCode[] matchedCode = new KeyCode[]{
            KeyCode.A, KeyCode.S, KeyCode.D,KeyCode.F, KeyCode.G,
            KeyCode.H, KeyCode.J, KeyCode.K, KeyCode.L,KeyCode.T,
            KeyCode.Y};

    private static final String[] chordNames = new String[]{
            "C4","D4","E4","F4","G4","A4","B4","C5","D5","E5","F5"
    };

    private static final byte[][] dataTo8051 = new byte[][]{
            {'a'},{'s'},{'d'},{'f'},{'g'},{'h'},{'j'},{'k'},{'l'},{'t'},{'y'}
    };

    public static void main(String[] args){
        launch();
        if (serialPort.closePort()) {
            System.out.println("Port is closed :)");
        } else {
            System.out.println("Failed to close port :(");
        }
    }

    @Override
    public void start(Stage stage){
        stage.setTitle("PIANO");
        StackPane root = new StackPane();
        Text text = new Text("Let's Rock");
        root.getChildren().add(text);

        serialPort = SerialPort.getCommPort("COM2");
        serialPort.setBaudRate(9600);
        if(serialPort.openPort()){
            text.setText("Port COM2 is opened");
        }else {
            text.setText("Failed to open COM2 port");
        }

        Scene scene = new Scene(root,400,400, Color.CYAN);
        stage.setScene(scene);
        stage.show();

        AtomicInteger lastSent= new AtomicInteger(-1);
        scene.setOnKeyPressed(keyEvent -> {
            for (int i = 0; i < 11; i++) {
                if (keyEvent.getCode()==matchedCode[i]){
                    text.setText(chordNames[i]+" Is Playing");
                    if (i!= lastSent.get()){
                        serialPort.writeBytes(dataTo8051[i],1 );
                        System.out.println("Sent "+ Arrays.toString(dataTo8051[i]));
                        lastSent.set(i);
                    }

                }
            }
        });

        scene.setOnKeyReleased(keyEvent -> {
            text.setText("Let's Rock");
            serialPort.writeBytes(new byte[]{'q'},1);
            lastSent.set(-1);
        });
    }
}