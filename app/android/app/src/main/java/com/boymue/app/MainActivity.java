package com.boymue.app;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import com.boymue.app.core.port.SkiaPort;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_main);
        SkiaPort.init(this);
    }
}
