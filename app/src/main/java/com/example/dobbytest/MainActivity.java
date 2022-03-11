package com.example.dobbytest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("dobby"); //libdobby.so
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        String stringFromJNI = stringFromJNI();
        String getHello = getHello();
        int meaningOfTheUniverse = meaningOfTheUniverse();

        tv.setText("stringFromJNI:"+stringFromJNI+"\ngetHello:"+getHello+"\nmeaningOfTheUniverse:"+meaningOfTheUniverse);

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public static native String getHello();
    public static native int meaningOfTheUniverse();
}